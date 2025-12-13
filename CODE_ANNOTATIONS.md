# Cub3D Code Annotations - Function-by-Function Analysis

This document provides detailed annotations for each function in the raycasting engine, explaining the exact purpose and mathematical reasoning behind every line of code.

---

## 1. Main Game Loop

### File: `src/game_bonus/game.c`

#### `game_loop(void *param)`

This is called every frame by MLX's event loop.

```c
int game_loop(void *param)
{
    t_game_data *data = (t_game_data *)param;
    
    // ═══════════════════════════════════════════════════════════════════
    // DELTA TIME CALCULATION
    // ═══════════════════════════════════════════════════════════════════
    double now = get_now_seconds();
    double dt = now - data->last_time;   // Time since last frame
    data->last_time = now;
    
    /*
     * WHY DELTA TIME?
     * ---------------
     * Without dt, game speed would depend on frame rate:
     * - 60 FPS: door opens in 1 second
     * - 30 FPS: door opens in 2 seconds (WRONG!)
     * 
     * With dt, we scale all time-based values:
     * - door->progress += door->speed * dt
     * - Now it takes 1 second regardless of FPS
     */
    
    // ═══════════════════════════════════════════════════════════════════
    // UPDATE GAME STATE
    // ═══════════════════════════════════════════════════════════════════
    bool moved = false;
    door_update(data, dt);              // Animate doors
    sprite_update_all(data, dt);        // Update AI + animations
    set_moved_flag(data, &moved);       // Process input, check if player moved
    
    // ═══════════════════════════════════════════════════════════════════
    // RENDER (only if something changed)
    // ═══════════════════════════════════════════════════════════════════
    if (moved || data->sprite_count > 0)
        redraw_map(data);
    
    return (0);
}
```

---

## 2. The Core Raycasting Loop

### File: `src/game_bonus/raycast.c`

#### `render_3d_view(t_game_data *data, int start_x, int view_width, int view_height)`

This is the heart of the raycasting engine.

```c
void render_3d_view(t_game_data *data, int start_x, int view_width, int view_height)
{
    int x;
    
    // Load textures once (lazy initialization)
    tex_ready(&g_textures_ready, &g_textures, data);
    
    // Allocate z-buffer for sprite occlusion
    ensure_z_buffer(data, start_x + view_width);
    
    // ═══════════════════════════════════════════════════════════════════
    // MAIN RAYCASTING LOOP - One iteration per screen column
    // ═══════════════════════════════════════════════════════════════════
    x = 0;
    while (x < view_width)
    {
        /*
         * STEP 1: Calculate camera_x
         * ══════════════════════════
         * Maps screen column to range [-1, +1]
         * 
         * x = 0       → camera_x = -1  (left edge of screen)
         * x = WIDTH/2 → camera_x = 0   (center)
         * x = WIDTH-1 → camera_x ≈ +1  (right edge)
         */
        data->rc.camera_x = 2.0 * x / (double)view_width - 1.0;
        
        /*
         * STEP 2: Calculate ray direction
         * ═══════════════════════════════
         * ray = dir + plane * camera_x
         * 
         * This creates a fan of rays across the camera plane:
         * 
         *     plane * (-1)     dir     plane * (+1)
         *           \          |          /
         *            \         |         /
         *             \        |        /
         *              \       |       /
         *               \      |      /
         *                \     |     /
         *                 \    |    /
         *                  \   |   /
         *                   \  |  /
         *                    \ | /
         *                     \|/
         *                      P (player)
         */
        set_ray_dir(data);
        
        /*
         * STEP 3: Initialize DDA starting position
         * ════════════════════════════════════════
         * map_x, map_y = integer grid cell containing player
         */
        set_player_position(data);
        
        /*
         * STEP 4: Calculate step directions
         * ═══════════════════════════════════
         * step_x = +1 if ray goes right (positive X)
         *        = -1 if ray goes left (negative X)
         * Similarly for step_y
         */
        set_steps(data);
        
        /*
         * STEP 5: Calculate delta distances
         * ═══════════════════════════════════
         * delta_dist_x = |1 / ray_dir_x|
         *              = distance ray travels to cross one X grid line
         * 
         * If ray_dir_x = 0.5:
         *   delta_dist_x = 2.0 (ray must travel 2 units to move 1 unit in X)
         * 
         * If ray_dir_x = 1.0:
         *   delta_dist_x = 1.0 (pure horizontal ray)
         */
        set_ray_dir_xy(data);
        
        /*
         * STEP 6: Calculate initial side distances
         * ════════════════════════════════════════
         * side_dist_x = distance from player to first vertical grid line
         * side_dist_y = distance from player to first horizontal grid line
         * 
         * Example: Player at (2.3, 1.7), ray going right (+X) and down (+Y)
         * 
         *   side_dist_x = (3 - 2.3) * delta_dist_x = 0.7 * delta_dist_x
         *   side_dist_y = (2 - 1.7) * delta_dist_y = 0.3 * delta_dist_y
         */
        set_horizontal_line_dist(data);
        
        /*
         * STEP 7: Reset hit detection
         */
        init_hit_data(data);
        
        /*
         * STEP 8: Run DDA algorithm
         * ═════════════════════════
         * Repeatedly step to the next grid line until we hit something
         */
        dda(data);
        
        /*
         * STEP 9: Select appropriate texture
         * ═══════════════════════════════════
         * Based on:
         *   - Hit type (wall or door)
         *   - Wall side (N/S/E/W)
         */
        set_current_tex(data, &g_textures);
        
        /*
         * STEP 10: Draw the vertical stripe
         * ═════════════════════════════════
         * Calculate wall height, sample texture, draw pixels
         */
        draw_walls(data, get_infos(start_x, x, view_height), g_textures.current_tex);
        
        /*
         * STEP 11: Store distance for sprite occlusion
         */
        data->z_buffer[start_x + x] = data->rc.perp_wall_dist;
        
        x++;
    }
}
```

---

## 3. DDA Algorithm Implementation

### File: `src/game_bonus/dda_n_helpers.c`

#### `dda(t_game_data *data)`

```c
void dda(t_game_data *data)
{
    t_door  *door;
    double  dist;
    double  tex_u;

    while (!data->rc.hit)
    {
        // ═══════════════════════════════════════════════════════════════════
        // BOUNDARY CHECK
        // ═══════════════════════════════════════════════════════════════════
        if (check_bounds(data))
            break;  // Ray escaped the map
        
        // ═══════════════════════════════════════════════════════════════════
        // STEP TO NEXT GRID LINE
        // ═══════════════════════════════════════════════════════════════════
        /*
         * Compare side_dist_x and side_dist_y.
         * Step in the direction with the smaller value.
         * 
         * Visualization:
         * 
         *   If side_dist_x < side_dist_y:
         *   We'll hit a vertical grid line first.
         *   
         *       │        │
         *   ────┼────────┼────
         *       │ P──────●     ← ray hits vertical line
         *   ────┼────────┼────
         *       │        │
         *   
         *   If side_dist_y < side_dist_x:
         *   We'll hit a horizontal grid line first.
         *   
         *       │        │
         *   ────┼────────┼────
         *       │ P      │
         *   ────┼──│─────┼────  ← ray hits horizontal line
         *       │  ●     │
         *       │        │
         */
        set_next_line(data);
        
        if (check_bounds(data))
            break;
        
        // ═══════════════════════════════════════════════════════════════════
        // CHECK FOR WALL
        // ═══════════════════════════════════════════════════════════════════
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == '1')
            hit_wall(data);  // Sets hit = 1, kind = HIT_WALL
        
        // ═══════════════════════════════════════════════════════════════════
        // CHECK FOR DOOR (BONUS)
        // ═══════════════════════════════════════════════════════════════════
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == 'D')
        {
            door = find_door(data, data->rc.map_y, data->rc.map_x);
            
            // Skip if door is fully open (progress >= 0.99)
            if (!door || door->progress >= 0.99)
                continue;
            
            // Perform ray-segment intersection test
            if (!door_ray_intersection(data, door, &dist, &tex_u))
                continue;  // Ray missed the door segment
            
            // Door hit! Store intersection data
            set_door_data(data, door, dist, tex_u);
        }
    }
}
```

#### `set_next_line(t_game_data *data)`

```c
void set_next_line(t_game_data *data)
{
    /*
     * This is the core DDA stepping logic.
     * 
     * We always step to the CLOSER grid line.
     * This ensures we never skip a cell.
     */
    if (data->rc.side_dist_x < data->rc.side_dist_y)
    {
        /*
         * Vertical grid line is closer.
         * 
         * 1. Add delta_dist_x to side_dist_x
         *    (now side_dist_x = distance to NEXT vertical line)
         * 
         * 2. Move map_x by step_x (+1 or -1)
         *    (now in the next column)
         * 
         * 3. Set side = 0 (we hit a vertical wall, i.e., East or West face)
         */
        data->rc.side_dist_x += data->rc.delta_dist_x;
        data->rc.map_x += data->rc.step_x;
        data->rc.side = 0;
    }
    else
    {
        /*
         * Horizontal grid line is closer.
         * 
         * 1. Add delta_dist_y to side_dist_y
         * 2. Move map_y by step_y
         * 3. Set side = 1 (we hit a horizontal wall, i.e., North or South face)
         */
        data->rc.side_dist_y += data->rc.delta_dist_y;
        data->rc.map_y += data->rc.step_y;
        data->rc.side = 1;
    }
}
```

---

## 4. Distance and Height Calculation

### File: `src/game_bonus/raycast_helper.c`

#### `get_perp_wall_distance(t_game_data *data)`

```c
void get_perp_wall_distance(t_game_data *data)
{
    if (data->rc.kind == HIT_DOOR)
        return;  // Doors already have their distance calculated
    
    /*
     * FISHEYE CORRECTION
     * ══════════════════
     * 
     * Problem with Euclidean distance:
     * ────────────────────────────────
     * Rays at screen edges travel further to reach the same wall.
     * This makes walls appear curved (fisheye effect).
     * 
     *   Euclidean:                 Perpendicular:
     *   
     *      Wall                        Wall
     *   ────────────                ────────────
     *    ╲        ╱                  │        │
     *     ╲      ╱                   │        │
     *      ╲    ╱                    │        │
     *       ╲  ╱                     │        │
     *        \/                      │        │
     *        P                       P
     *   
     *   CURVED (wrong!)             FLAT (correct!)
     * 
     * Solution:
     * ─────────
     * Use the perpendicular distance to the camera plane,
     * not the Euclidean distance to the player.
     * 
     * Formula:
     * ────────
     * If side == 0 (vertical wall, East/West):
     *   perp_dist = (map_x - pos_x + (1 - step_x) / 2) / ray_dir_x
     * 
     * If side == 1 (horizontal wall, North/South):
     *   perp_dist = (map_y - pos_y + (1 - step_y) / 2) / ray_dir_y
     * 
     * The "(1 - step) / 2" term adjusts for which side of the cell we hit:
     *   - If step = +1: (1 - 1) / 2 = 0   → hit the near edge
     *   - If step = -1: (1 - (-1)) / 2 = 1 → hit the far edge
     */
    
    if (data->rc.side == 0)  // Vertical wall
    {
        data->rc.perp_wall_dist = (data->rc.map_x - data->player.pos.x 
            + (1 - data->rc.step_x) / 2.0) / data->rc.ray_dir_x;
    }
    else  // Horizontal wall
    {
        data->rc.perp_wall_dist = (data->rc.map_y - data->player.pos.y 
            + (1 - data->rc.step_y) / 2.0) / data->rc.ray_dir_y;
    }
}
```

### File: `src/game_bonus/raycast_helper_4.c`

#### `set_line_height(t_game_data *data, int view_height)`

```c
void set_line_height(t_game_data *data, int view_height)
{
    /*
     * PERSPECTIVE PROJECTION
     * ══════════════════════
     * 
     * Objects appear smaller as they get further away.
     * 
     * Formula: line_height = view_height / perp_wall_dist
     * 
     * Examples:
     * ─────────
     * view_height = 1080 pixels
     * 
     * Distance = 1.0 units:  line_height = 1080 / 1.0 = 1080 (fills screen)
     * Distance = 2.0 units:  line_height = 1080 / 2.0 = 540  (half screen)
     * Distance = 5.0 units:  line_height = 1080 / 5.0 = 216  (small wall)
     * Distance = 0.5 units:  line_height = 1080 / 0.5 = 2160 (larger than screen!)
     * 
     * When line_height > view_height, the wall is clipped at top/bottom.
     */
    data->rc.line_height = (int)(view_height / data->rc.perp_wall_dist);
}
```

---

## 5. Texture Mapping

### File: `src/game_bonus/raycast_helper_3.c`

#### `set_texture_coordinations(t_game_data *data)`

```c
void set_texture_coordinations(t_game_data *data)
{
    if (data->rc.kind == HIT_DOOR)
        return;
    
    /*
     * CALCULATING wall_x (Texture U Coordinate)
     * ═══════════════════════════════════════════
     * 
     * We need to find WHERE on the wall the ray hit.
     * This determines which column of the texture to use.
     * 
     * Method:
     * ───────
     * 1. Calculate the exact hit point using:
     *    hit_point = player_pos + perp_wall_dist * ray_dir
     * 
     * 2. Take the fractional part (the position within the cell [0, 1))
     * 
     * For vertical walls (side == 0):
     *   The Y coordinate varies along the wall, so use Y.
     *   wall_x = pos.y + perp_wall_dist * ray_dir_y
     * 
     * For horizontal walls (side == 1):
     *   The X coordinate varies along the wall, so use X.
     *   wall_x = pos.x + perp_wall_dist * ray_dir_x
     * 
     * 3. Get fractional part: wall_x -= floor(wall_x)
     * 
     * Example:
     * ────────
     * Player at (2.3, 1.5), ray hits wall at Y = 2.7
     * wall_x = 2.7 - floor(2.7) = 2.7 - 2.0 = 0.7
     * 
     * For a 64-pixel wide texture:
     * tex_x = 0.7 * 64 = 44.8 → pixel column 44
     */
    
    if (data->rc.side == 0)
        data->rc.wall_x = data->player.pos.y + data->rc.perp_wall_dist 
            * data->rc.ray_dir_y;
    else
        data->rc.wall_x = data->player.pos.x + data->rc.perp_wall_dist 
            * data->rc.ray_dir_x;
    
    data->rc.wall_x -= floor(data->rc.wall_x);
}
```

### File: `src/game_bonus/raycast_helper_2.c`

#### `draw(t_game_data *data, t_texture *tex, t_infos infos)`

```c
void draw(t_game_data *data, t_texture *tex, t_infos infos)
{
    int             y;
    unsigned int    color;
    double          tex_step;
    double          tex_pos;

    /*
     * TEXTURE SCALING
     * ═══════════════
     * 
     * tex_step = how many texture pixels per screen pixel
     * 
     * Example:
     * ────────
     * Texture height = 64 pixels
     * Wall on screen = 200 pixels
     * tex_step = 64 / 200 = 0.32
     * 
     * For each screen pixel, advance 0.32 texture pixels.
     * This stretches the 64-pixel texture to cover 200 screen pixels.
     */
    tex_step = (double)tex->height / data->rc.line_height;
    
    /*
     * STARTING TEXTURE POSITION
     * ═════════════════════════
     * 
     * If the wall is taller than the screen (line_height > view_height),
     * draw_start will be 0 but we need to start in the middle of the texture.
     * 
     * tex_pos = (draw_start - screen_center + wall_center) * tex_step
     * 
     * Example:
     * ────────
     * view_height = 600, line_height = 800
     * draw_start = 0 (clipped)
     * tex_pos = (0 - 300 + 400) * (64/800) = 100 * 0.08 = 8
     * 
     * We start at texture pixel 8, skipping the top 8 rows of texture.
     */
    tex_pos = (data->rc.draw_start - infos.view_height / 2.0 
        + data->rc.line_height / 2.0) * tex_step;
    
    /*
     * DRAW LOOP
     * ═════════
     */
    y = data->rc.draw_start;
    while (y <= data->rc.draw_end)
    {
        // Current texture Y coordinate
        tex->tex_y = (int)tex_pos;
        
        // Clamp to valid range (safety check)
        if (tex->tex_y < 0)
            tex->tex_y = 0;
        else if (tex->tex_y >= tex->height)
            tex->tex_y = tex->height - 1;
        
        // Advance to next texture pixel
        tex_pos += tex_step;
        
        /*
         * SAMPLE TEXTURE COLOR
         * ════════════════════
         * 
         * Address = base_addr + (y * line_len) + (x * bytes_per_pixel)
         * 
         * tex->line_len: bytes per row (may include padding)
         * tex->bpp: bits per pixel (usually 32)
         */
        color = *(unsigned int *)(tex->addr 
            + tex->tex_y * tex->line_len 
            + tex->tex_x * (tex->bpp / 8));
        
        put_pixel(&data->mlx, infos.start_x + infos.x, y, color);
        y++;
    }
}
```

---

## 6. Door Ray Intersection

### File: `src/game_bonus/raycast.c`

#### `door_ray_intersection(...)`

```c
int door_ray_intersection(t_game_data *data, t_door *door, 
                          double *hit_dist, double *u)
{
    t_vec2  origin;
    t_vec2  dir;
    t_vec2  span;
    t_vec2  diff;
    double  denom, t, s;

    if (!door || !door->has_geom)
        return (0);
    
    // Get ray origin and direction
    set_origin_dir(data, &origin, &dir);
    
    // Get the current door span (rotated based on progress)
    span = door_current_span(door);
    
    /*
     * 2D LINE-LINE INTERSECTION USING CROSS PRODUCT
     * ═══════════════════════════════════════════════
     * 
     * Ray:  P(t) = origin + t * dir         (t >= 0)
     * Door: Q(s) = pivot + s * span         (s ∈ [0, 1])
     * 
     * Setting P(t) = Q(s):
     *   origin + t * dir = pivot + s * span
     * 
     * Rearranging:
     *   t * dir - s * span = pivot - origin = diff
     * 
     * Using Cramer's rule with 2D cross product:
     *   
     *   t = (diff × span) / (dir × span)
     *   s = (diff × dir) / (dir × span)
     * 
     * Where 2D cross product: a × b = a.x * b.y - a.y * b.x
     * 
     * Geometric interpretation:
     * ─────────────────────────
     *   - denom = dir × span = sin(angle between ray and door) * |dir| * |span|
     *   - If denom ≈ 0, ray is parallel to door (no intersection)
     *   - t = distance along ray to intersection
     *   - s = position along door segment [0, 1]
     */
    
    denom = vec2_cross(dir, span);
    if (fabs(denom) < 1e-8)
        return (0);  // Parallel - no intersection
    
    diff = vec2_sub(door->pivot, origin);
    t = vec2_cross(diff, span) / denom;
    s = vec2_cross(diff, dir) / denom;
    
    /*
     * VALIDITY CHECKS
     * ═══════════════
     * 
     * t <= 1e-4: Intersection behind or very close to player
     * s < 0:     Intersection before door pivot
     * s > 1:     Intersection after door end
     */
    if (t <= 1e-4 || s < 0.0 || s > 1.0)
        return (0);
    
    if (hit_dist) *hit_dist = t;  // Distance to door
    if (u) *u = s;                // Texture coordinate
    return (1);
}
```

---

## 7. Sprite Camera Transform

### File: `src/game_bonus/sprite_helper.c`

#### `sprite_camera_transform(...)`

```c
void sprite_camera_transform(t_game_data *data, t_sprite *sprite)
{
    t_vec2  rel;
    double  det;
    double  inv_det;

    /*
     * WORLD TO CAMERA SPACE TRANSFORMATION
     * ════════════════════════════════════
     * 
     * We need to convert sprite world position to camera-relative coordinates:
     *   cam_x: horizontal position (for screen X)
     *   cam_z: depth (for perspective and occlusion)
     * 
     * Camera basis vectors:
     *   - dir: points forward (viewing direction)
     *   - plane: points right (perpendicular to dir)
     * 
     * Together they form a 2x2 transformation matrix:
     *   
     *   C = | plane.x  dir.x |
     *       | plane.y  dir.y |
     * 
     * To go FROM world TO camera, we need the inverse:
     *   
     *   C^(-1) = (1/det) * |  dir.y   -dir.x  |
     *                      | -plane.y  plane.x |
     * 
     * Where det = plane.x * dir.y - dir.x * plane.y
     */
    
    rel = vec2_sub(sprite->position, data->player.pos);
    
    det = data->player.plane.x * data->player.dir.y 
        - data->player.dir.x * data->player.plane.y;
    
    if (fabs(det) < 1e-9)
    {
        sprite->visible = false;
        return;
    }
    
    inv_det = 1.0 / det;
    
    /*
     * Apply the inverse transformation:
     * 
     * | cam_x |            | dir.y   -dir.x  |   | rel.x |
     * | cam_z | = inv_det *| -plane.y plane.x| * | rel.y |
     * 
     * cam_x = inv_det * (dir.y * rel.x - dir.x * rel.y)
     * cam_z = inv_det * (-plane.y * rel.x + plane.x * rel.y)
     * 
     * Result:
     *   cam_x > 0: sprite is to the right
     *   cam_x < 0: sprite is to the left
     *   cam_z > 0: sprite is in front of camera
     *   cam_z < 0: sprite is behind camera (invisible)
     */
    
    sprite->cam_x = inv_det * (data->player.dir.y * rel.x 
                             - data->player.dir.x * rel.y);
    sprite->cam_z = inv_det * (-data->player.plane.y * rel.x 
                             + data->player.plane.x * rel.y);
    
    set_cam_z(sprite, rel);  // Set visibility and distance
}
```

#### `project_to_screen(...)`

```c
void project_to_screen(t_sprite *sprite, int start_x, int v_w, int v_h)
{
    /*
     * 3D TO 2D PROJECTION
     * ═══════════════════
     * 
     * screen_x = center + (cam_x / cam_z) * (width / 2)
     *          = center + cam_x * inv_z * (width / 2)
     * 
     * This is perspective projection:
     *   - Objects further away (large cam_z) appear closer to center
     *   - Objects to the right (positive cam_x) appear on right side of screen
     * 
     * sprite_height = view_height / cam_z
     *               = view_height * inv_z
     * 
     * Same formula as walls - closer sprites are larger.
     * 
     * sprite_width = sprite_height (square sprite)
     */
    
    sprite->draw.screen_x = start_x + (int)((v_w / 2.0) 
        * (1.0 + sprite->cam_x * sprite->draw.inv_z));
    
    sprite->draw.sprite_height = (int)fabs(v_h * sprite->draw.inv_z);
    sprite->draw.sprite_width = sprite->draw.sprite_height;
}
```

---

## 8. Movement Physics

### File: `src/game_bonus/movement_2.c`

#### `strafe_move(...)`

```c
void strafe_move(t_game_data *data, int direction)
{
    double  step_x;
    double  step_y;

    /*
     * PERPENDICULAR MOVEMENT (STRAFING)
     * ═══════════════════════════════════
     * 
     * To move sideways, we need a vector perpendicular to the view direction.
     * 
     * For a 2D vector (x, y), the perpendicular vectors are:
     *   - (y, -x)   rotated 90° clockwise
     *   - (-y, x)   rotated 90° counter-clockwise
     * 
     * If dir = (dx, dy), then strafe direction = (dy, -dx)
     * 
     * Example:
     * ────────
     * Facing North: dir = (0, -1)
     * Strafe right: perpendicular = (-1, 0) → move West... wait, that's wrong!
     * 
     * Actually, we use (dir.y, -dir.x):
     * Facing North: dir = (0, -1)
     * (dir.y, -dir.x) = (-1, 0) → moves left (West)
     * 
     * So direction = +1 is left, direction = -1 is right.
     */
    
    step_x = direction * data->player.dir.y * data->player.move_speed;
    step_y = direction * -data->player.dir.x * data->player.move_speed;
    apply_move_with_slide(data, step_x, step_y);
}
```

#### `apply_move_with_slide(...)`

```c
static void apply_move_with_slide(t_game_data *data, double step_x, double step_y)
{
    double  base_x = data->player.pos.x;
    double  base_y = data->player.pos.y;

    /*
     * WALL SLIDING
     * ════════════
     * 
     * When the player walks into a wall at an angle, 
     * we want them to slide along it, not stop completely.
     * 
     * Strategy:
     * ─────────
     * 1. Try the full movement (step_x, step_y)
     * 2. If blocked, try X-only movement
     * 3. If still blocked in X, try Y-only movement
     * 
     * Example:
     * ────────
     * Player walks northeast into a horizontal wall.
     * 
     *   ████████████████████  ← wall
     *        ↗
     *       P
     * 
     * Full movement (NE) is blocked by wall.
     * X-only movement (E) is valid → player slides east along wall.
     */
    
    // Try full movement first
    if (valid_move(data, base_x + step_x, base_y + step_y))
    {
        data->player.pos.x = base_x + step_x;
        data->player.pos.y = base_y + step_y;
        return;
    }
    
    // X-only slide
    if (step_x != 0.0 && valid_move(data, base_x + step_x, base_y))
        base_x += step_x;
    
    // Y-only slide
    if (step_y != 0.0 && valid_move(data, base_x, base_y + step_y))
        base_y += step_y;
    
    data->player.pos.x = base_x;
    data->player.pos.y = base_y;
}
```

#### `valid_move(...)`

```c
int valid_move(t_game_data *data, double new_x, double new_y)
{
    double r = PLAYER_COLLISION_RADIUS;  // 0.2 units

    /*
     * COLLISION RADIUS
     * ════════════════
     * 
     * The player is not a point - they have a circular collision area.
     * We check 9 points around the player position.
     * 
     *     (-r, -r)  (0, -r)  (+r, -r)
     *          ●───────●───────●
     *          │       │       │
     *     (-r, 0)───── P ─────(+r, 0)
     *          │       │       │
     *          ●───────●───────●
     *     (-r, +r)  (0, +r)  (+r, +r)
     * 
     * If ANY of these points is inside a wall, movement is invalid.
     */
    
    if (blocked_at(data, new_x, new_y)) return (0);
    if (blocked_at(data, new_x + r, new_y)) return (0);
    if (blocked_at(data, new_x - r, new_y)) return (0);
    if (blocked_at(data, new_x, new_y + r)) return (0);
    if (blocked_at(data, new_x, new_y - r)) return (0);
    if (blocked_at(data, new_x + r, new_y + r)) return (0);
    if (blocked_at(data, new_x + r, new_y - r)) return (0);
    if (blocked_at(data, new_x - r, new_y + r)) return (0);
    if (blocked_at(data, new_x - r, new_y - r)) return (0);
    
    return (1);
}
```

---

## 9. Rotation Mathematics

### File: `src/game_bonus/movement_3.c`

#### `rotate_player(...)`

```c
void rotate_player(t_game_data *data, double angle)
{
    double old_dir_x = data->player.dir.x;
    double old_plane_x = data->player.plane.x;

    /*
     * 2D ROTATION MATRIX
     * ══════════════════
     * 
     * To rotate a vector (x, y) by angle θ:
     * 
     * | x' |   | cos(θ)  -sin(θ) |   | x |
     * | y' | = | sin(θ)   cos(θ) | * | y |
     * 
     * x' = x * cos(θ) - y * sin(θ)
     * y' = x * sin(θ) + y * cos(θ)
     * 
     * CRITICAL: We must rotate BOTH dir AND plane!
     * ───────────────────────────────────────────
     * 
     * If we only rotate dir, the camera plane would no longer be
     * perpendicular, causing severe distortion.
     * 
     * Before rotation:           After rotating ONLY dir (WRONG):
     *                           
     *     plane                      plane
     *   ←───┼───→                  ←───┼───→
     *       │                          ╲
     *       │ dir                        ╲ dir
     *       ▼                              ↘
     *       P                               P
     *                           
     *   (90° angle)              (angle ≠ 90° - DISTORTED!)
     * 
     * The plane must always be perpendicular to dir.
     */
    
    // Rotate direction vector
    data->player.dir.x = old_dir_x * cos(angle) - data->player.dir.y * sin(angle);
    data->player.dir.y = old_dir_x * sin(angle) + data->player.dir.y * cos(angle);
    
    // Rotate camera plane (same angle to maintain perpendicularity)
    data->player.plane.x = old_plane_x * cos(angle) - data->player.plane.y * sin(angle);
    data->player.plane.y = old_plane_x * sin(angle) + data->player.plane.y * cos(angle);
}
```

---

## 10. Complete Rendering Frame Summary

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                            SINGLE FRAME RENDER                               │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  1. TIMING                                                                   │
│     └── Calculate dt = current_time - last_time                              │
│                                                                              │
│  2. INPUT PROCESSING                                                         │
│     └── Handle keyboard (W/A/S/D, arrows, E for doors)                       │
│     └── Handle mouse (rotation via delta movement)                           │
│                                                                              │
│  3. GAME STATE UPDATE                                                        │
│     └── door_update(dt): progress += speed * dt                              │
│     └── sprite_update_all(dt): AI movement, animation frames                 │
│     └── Player movement: pos += dir * speed (with collision)                 │
│                                                                              │
│  4. WALL RAYCASTING (for each column x = 0 to WIDTH)                         │
│     │                                                                        │
│     ├── Calculate ray direction:                                             │
│     │   camera_x = (2*x / WIDTH) - 1                                         │
│     │   ray = dir + plane * camera_x                                         │
│     │                                                                        │
│     ├── Initialize DDA:                                                      │
│     │   delta_dist = |1 / ray_component|                                     │
│     │   side_dist = distance to first grid line                              │
│     │                                                                        │
│     ├── Run DDA loop:                                                        │
│     │   while (!hit):                                                        │
│     │     step to closer grid line                                           │
│     │     check for wall ('1') or door ('D')                                 │
│     │                                                                        │
│     ├── Calculate perpendicular distance (fisheye correction)                │
│     ├── Calculate wall height: h = screen_height / distance                  │
│     ├── Calculate draw bounds: start = center - h/2, end = center + h/2      │
│     │                                                                        │
│     ├── Texture mapping:                                                     │
│     │   wall_x = fractional hit position [0, 1)                              │
│     │   tex_x = wall_x * texture_width                                       │
│     │   tex_step = texture_height / wall_height                              │
│     │                                                                        │
│     └── Draw vertical stripe:                                                │
│         for y = draw_start to draw_end:                                      │
│           tex_y = accumulated position                                       │
│           color = texture[tex_x][tex_y]                                      │
│           put_pixel(x, y, color)                                             │
│         Store distance in z_buffer[x]                                        │
│                                                                              │
│  5. SPRITE RENDERING                                                         │
│     │                                                                        │
│     ├── Transform all sprites to camera space                                │
│     │   rel = sprite_pos - player_pos                                        │
│     │   cam_x, cam_z = inverse_camera_matrix * rel                           │
│     │                                                                        │
│     ├── Filter: keep only sprites with cam_z > 0 (in front)                  │
│     │                                                                        │
│     ├── Sort by distance (back to front - painter's algorithm)               │
│     │                                                                        │
│     └── For each sprite:                                                     │
│         Project to screen:                                                   │
│           screen_x = center + cam_x/cam_z * (width/2)                        │
│           size = height / cam_z                                              │
│         For each sprite column:                                              │
│           if z_buffer[x] > sprite_z: continue (behind wall)                  │
│           sample from sprite sheet                                           │
│           if not transparent: put_pixel                                      │
│                                                                              │
│  6. OVERLAY RENDERING                                                        │
│     └── gun_render(): Draw weapon overlay                                    │
│     └── draw_env(): Draw minimap                                             │
│                                                                              │
│  7. DISPLAY                                                                  │
│     └── mlx_put_image_to_window(image)                                       │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Key Formulas Reference Card

| Concept | Formula | Code Location |
|---------|---------|---------------|
| Camera X | $\text{camera\_x} = \frac{2x}{W} - 1$ | raycast.c |
| Ray Direction | $\vec{ray} = \vec{dir} + \vec{plane} \cdot \text{camera\_x}$ | raycast_helper_4.c |
| Delta Distance | $\delta_x = \left|\frac{1}{ray_x}\right|$ | raycast_helper.c |
| Perpendicular Dist | $d_{perp} = \frac{map - pos + \frac{1-step}{2}}{ray}$ | raycast_helper.c |
| Wall Height | $h = \frac{H_{screen}}{d_{perp}}$ | raycast_helper_4.c |
| Texture U (wall_x) | $u = \text{frac}(pos + d \cdot ray)$ | raycast_helper_3.c |
| Texture Step | $\text{step} = \frac{H_{tex}}{h_{wall}}$ | raycast_helper_2.c |
| 2D Rotation | $\begin{pmatrix}x'\\ y'\end{pmatrix} = \begin{pmatrix}\cos\theta & -\sin\theta\\ \sin\theta & \cos\theta\end{pmatrix}\begin{pmatrix}x\\ y\end{pmatrix}$ | movement_3.c |
| Door Intersection | $t = \frac{(Q-P)\times S}{D\times S}$ | raycast.c |
| Camera Transform | $\begin{pmatrix}cam_x\\ cam_z\end{pmatrix} = C^{-1} \cdot \vec{rel}$ | sprite_helper.c |
| Sprite Screen X | $x = \frac{W}{2}(1 + \frac{cam_x}{cam_z})$ | sprite_helper.c |

---

*End of annotations document*

# Cub3D Raycasting Engine - Complete Deep Dive

## Table of Contents
1. [Overview & Architecture](#1-overview--architecture)
2. [The Mathematical Foundation](#2-the-mathematical-foundation)
3. [Player Representation](#3-player-representation)
4. [Ray Casting Algorithm](#4-ray-casting-algorithm)
5. [DDA Algorithm](#5-dda-algorithm)
6. [Wall Rendering & Texturing](#6-wall-rendering--texturing)
7. [Door System](#7-door-system)
8. [Sprite System](#8-sprite-system)
9. [Z-Buffer & Occlusion](#9-z-buffer--occlusion)
10. [Movement & Collision](#10-movement--collision)

---

## 1. Overview & Architecture

### What is Raycasting?

Raycasting is a **rendering technique** that simulates a 3D environment by casting rays from the player's viewpoint into a 2D map. Unlike true 3D engines that process polygons, raycasting exploits the constraint that **all walls are vertical** and the floor/ceiling are at fixed heights, allowing us to render each vertical strip of the screen independently.

### The Rendering Pipeline

```
┌─────────────────────────────────────────────────────────────────────┐
│                         GAME LOOP                                    │
├─────────────────────────────────────────────────────────────────────┤
│  1. Update Time (dt)                                                │
│  2. Process Input (keys, mouse)                                     │
│  3. Update Doors (animation)                                        │
│  4. Update Sprites (AI, animation)                                  │
│  5. For each screen column (x = 0 to WIDTH):                        │
│     ├── Calculate ray direction                                     │
│     ├── Run DDA to find wall/door hit                               │
│     ├── Calculate perpendicular distance                            │
│     ├── Calculate wall height on screen                             │
│     ├── Sample texture and draw vertical stripe                     │
│     └── Store distance in z-buffer                                  │
│  6. Render sprites (back-to-front, using z-buffer)                  │
│  7. Render minimap                                                  │
│  8. Put image to window                                             │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 2. The Mathematical Foundation

### 2.1 Coordinate System

The game uses a **2D grid coordinate system** where:
- **X-axis**: Increases to the right (East)
- **Y-axis**: Increases downward (South)
- Each grid cell is a **1×1 unit square**
- Player position is a floating-point coordinate `(pos.x, pos.y)`

```
    0   1   2   3   4   X→
  ┌───┬───┬───┬───┬───┐
0 │ 1 │ 1 │ 1 │ 1 │ 1 │
  ├───┼───┼───┼───┼───┤
1 │ 1 │ 0 │ 0 │ 0 │ 1 │
  ├───┼───┼───┼───┼───┤
2 │ 1 │ 0 │ P │ 0 │ 1 │  P = Player at (2.5, 2.5)
  ├───┼───┼───┼───┼───┤
3 │ 1 │ 0 │ 0 │ 0 │ 1 │
  ├───┼───┼───┼───┼───┤
4 │ 1 │ 1 │ 1 │ 1 │ 1 │
  └───┴───┴───┴───┴───┘
Y↓
```

### 2.2 Vector Library (vec.h / vec.c)

The engine uses a 2D vector library for all geometric calculations:

```c
typedef struct s_vec2
{
    double x;
    double y;
} t_vec2;
```

#### Key Vector Operations

| Operation | Formula | Code | Purpose |
|-----------|---------|------|---------|
| **Addition** | $\vec{a} + \vec{b} = (a_x + b_x, a_y + b_y)$ | `vec2_add(a, b)` | Translate positions |
| **Subtraction** | $\vec{a} - \vec{b} = (a_x - b_x, a_y - b_y)$ | `vec2_sub(a, b)` | Get direction vectors |
| **Scale** | $s \cdot \vec{v} = (s \cdot v_x, s \cdot v_y)$ | `vec2_scale(v, s)` | Movement speed |
| **Dot Product** | $\vec{a} \cdot \vec{b} = a_x b_x + a_y b_y$ | `vec2_dot(a, b)` | Projection, angles |
| **Cross Product (2D)** | $\vec{a} \times \vec{b} = a_x b_y - a_y b_x$ | `vec2_cross(a, b)` | Ray-segment intersection |
| **Length** | $\|\vec{v}\| = \sqrt{v_x^2 + v_y^2}$ | `vec2_length(v)` | Distance calculation |
| **Rotation** | See below | `vec2_rotate(v, θ)` | Camera/door rotation |

#### 2D Rotation Matrix

To rotate a vector by angle θ:

$$
\begin{pmatrix} x' \\ y' \end{pmatrix} = 
\begin{pmatrix} \cos\theta & -\sin\theta \\ \sin\theta & \cos\theta \end{pmatrix}
\begin{pmatrix} x \\ y \end{pmatrix}
$$

```c
t_vec2 vec2_rotate(t_vec2 v, double angle)
{
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    return vec2(
        v.x * cos_a - v.y * sin_a,  // x' = x·cos(θ) - y·sin(θ)
        v.x * sin_a + v.y * cos_a   // y' = x·sin(θ) + y·cos(θ)
    );
}
```

**Visual Impact**: This is used when rotating the player's view (keyboard arrows or mouse). Both the direction vector AND the camera plane must rotate together to maintain perspective.

---

## 3. Player Representation

### 3.1 The Player Structure

```c
typedef struct s_player
{
    t_vec2  pos;           // Position in world coordinates
    t_vec2  dir;           // Direction vector (normalized, length ≈ 1)
    t_vec2  plane;         // Camera plane vector (perpendicular to dir)
    double  move_speed;    // Units per frame
    double  rot_speed;     // Radians per frame
    // ... movement flags
} t_player;
```

### 3.2 The Camera Plane Concept

This is the **key insight** of the raycasting algorithm. The player has:
1. A **direction vector** (dir): Where the player is looking
2. A **camera plane** (plane): A vector perpendicular to dir, defining the visible area

```
                    Camera Plane (plane vector)
                    ←─────────────────────────→
                    ╲         │         ╱
                     ╲        │        ╱
                      ╲       │       ╱
         Left ray ────→╲     │     ╱←──── Right ray
                        ╲    │dir ╱
                         ╲   │   ╱
                          ╲  │  ╱
                           ╲ │ ╱
                            ╲│╱
                             P (Player position)
```

#### Field of View (FOV)

The **FOV** is determined by the ratio of plane length to direction length:

$$FOV = 2 \cdot \arctan\left(\frac{|\text{plane}|}{|\text{dir}|}\right)$$

If `|plane| = 0.66` and `|dir| = 1.0`:
$$FOV = 2 \cdot \arctan(0.66) ≈ 66°$$

### 3.3 Player Initialization

From `main_helper.c` (via orientation parsing):

```c
void init_player(t_game_data *game_data, int row, int col, char orientation)
{
    game_data->player.pos = vec2(col + 0.5, row + 0.5);  // Center of tile
    game_data->player.dir = orientation_dir(orientation); // e.g., (0, -1) for 'N'
    game_data->player.plane = vec2_perpendicular(game_data->player.dir);
    game_data->player.plane = vec2_scale(game_data->player.plane, 0.66);
    game_data->player.move_speed = 0.05;
    game_data->player.rot_speed = 0.05;
}
```

**Why 0.5 offset?** Places player at the **center** of the grid cell, not the corner.

**Why perpendicular plane?** The camera plane must be at 90° to the view direction to create proper perspective. The `0.66` scaling gives approximately 66° FOV.

---

## 4. Ray Casting Algorithm

### 4.1 Overview

For each vertical column of pixels (x = 0 to WIDTH), we:
1. Calculate the ray direction
2. Find where the ray hits a wall (using DDA)
3. Calculate the wall height based on distance
4. Draw the textured vertical stripe

### 4.2 Ray Direction Calculation

```c
// raycast_helper_4.c
void set_ray_dir(t_game_data *data)
{
    data->rc.ray_dir_x = data->player.dir.x + data->player.plane.x * data->rc.camera_x;
    data->rc.ray_dir_y = data->player.dir.y + data->player.plane.y * data->rc.camera_x;
}
```

#### The Math Behind It

For screen column `x` (0 to WIDTH-1):

$$\text{camera\_x} = \frac{2x}{\text{WIDTH}} - 1$$

This maps:
- `x = 0` → `camera_x = -1` (left edge)
- `x = WIDTH/2` → `camera_x = 0` (center)
- `x = WIDTH-1` → `camera_x ≈ +1` (right edge)

The ray direction is then:

$$\vec{ray} = \vec{dir} + \vec{plane} \cdot \text{camera\_x}$$

**Visual Explanation**:
```
camera_x = -1           camera_x = 0           camera_x = +1
    │                        │                       │
    ▼                        ▼                       ▼
←───●────────────────────────●───────────────────────●───→
   plane                    dir                    plane
   (-1)                     (0)                    (+1)
```

Each ray sweeps across the camera plane from left to right.

### 4.3 Delta Distance Calculation

```c
// raycast_helper.c
void set_ray_dir_xy(t_game_data *data)
{
    if (data->rc.ray_dir_x)
        data->rc.delta_dist_x = fabs(1.0 / data->rc.ray_dir_x);
    else
        data->rc.delta_dist_x = 1e30;  // Infinity (ray parallel to Y-axis)
    
    if (data->rc.ray_dir_y)
        data->rc.delta_dist_y = fabs(1.0 / data->rc.ray_dir_y);
    else
        data->rc.delta_dist_y = 1e30;
}
```

#### Mathematical Derivation

**Delta Distance** = How far the ray travels to cross **one grid line**.

Consider a ray with direction $(d_x, d_y)$. To travel 1 unit in the X direction:

$$\Delta_x = \left| \frac{1}{d_x} \right|$$

**Proof**: If the ray moves by vector $(d_x, d_y)$ per unit time, then to move 1 unit in X:
- Time needed: $t = 1/d_x$
- Total distance traveled: $\sqrt{d_x^2 + d_y^2} \cdot t = \frac{\sqrt{d_x^2 + d_y^2}}{d_x}$

But since we use **unit direction vectors** (|dir| ≈ 1), this simplifies to:
$$\Delta_x = \frac{1}{|d_x|}$$

---

## 5. DDA Algorithm (Digital Differential Analyzer)

### 5.1 What is DDA?

DDA is an efficient algorithm to find which grid cells a ray passes through, checking each cell for walls. Instead of testing every point along the ray, we "step" from grid line to grid line.

### 5.2 Step Direction

```c
// raycast_helper_4.c
void set_steps(t_game_data *data)
{
    if (data->rc.ray_dir_x < 0)
        data->rc.step_x = -1;  // Ray going West
    else
        data->rc.step_x = 1;   // Ray going East
    
    if (data->rc.ray_dir_y < 0)
        data->rc.step_y = -1;  // Ray going North
    else
        data->rc.step_y = 1;   // Ray going South
}
```

### 5.3 Initial Side Distance

The **side distance** is the distance from the player to the first grid line in each direction.

```c
// raycast_helper_4.c
void set_vertical_line_dist(t_game_data *data)
{
    if (data->rc.step_x > 0)
        data->rc.side_dist_x = fabs((data->rc.map_x + 1 - data->player.pos.x)
            * data->rc.delta_dist_x);
    else
        data->rc.side_dist_x = fabs((data->player.pos.x - data->rc.map_x)
            * data->rc.delta_dist_x);
}

// dda_n_helpers.c
void set_horizontal_line_dist(t_game_data *data)
{
    if (data->rc.step_y > 0)
        data->rc.side_dist_y = fabs((data->rc.map_y + 1 - data->player.pos.y)
            * data->rc.delta_dist_y);
    else
        data->rc.side_dist_y = fabs((data->player.pos.y - data->rc.map_y)
            * data->rc.delta_dist_y);
}
```

#### Visual Example

```
Player at (2.3, 1.7), ray going right (+X) and down (+Y):

      0     1     2     3     4
    ┌─────┬─────┬─────┬─────┬─────┐
  0 │     │     │     │     │     │
    ├─────┼─────┼─────┼─────┼─────┤
  1 │     │     │  P──┼──→  │     │  side_dist_x = (3 - 2.3) × δx
    ├─────┼─────┼──│──┼─────┼─────┤
  2 │     │     │  ↓  │     │     │  side_dist_y = (2 - 1.7) × δy
    └─────┴─────┴─────┴─────┴─────┘
```

### 5.4 The DDA Loop

```c
// dda_n_helpers.c
void set_next_line(t_game_data *data)
{
    if (data->rc.side_dist_x < data->rc.side_dist_y)
    {
        data->rc.side_dist_x += data->rc.delta_dist_x;
        data->rc.map_x += data->rc.step_x;
        data->rc.side = 0;  // Hit vertical grid line (X-side)
    }
    else
    {
        data->rc.side_dist_y += data->rc.delta_dist_y;
        data->rc.map_y += data->rc.step_y;
        data->rc.side = 1;  // Hit horizontal grid line (Y-side)
    }
}

void dda(t_game_data *data)
{
    t_door  *door;
    double  dist, tex_u;

    while (!data->rc.hit)
    {
        if (check_bounds(data))
            break;
        set_next_line(data);
        if (check_bounds(data))
            break;
        
        // Check for wall hit
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == '1')
            hit_wall(data);
        
        // Check for door hit (bonus)
        if (data->map.grid[data->rc.map_y][data->rc.map_x] == 'D')
        {
            door = find_door(data, data->rc.map_y, data->rc.map_x);
            if (!door || door->progress >= 0.99)
                continue;
            if (!door_ray_intersection(data, door, &dist, &tex_u))
                continue;
            set_door_data(data, door, dist, tex_u);
        }
    }
}
```

#### DDA Visualization

```
Step 1: side_dist_x < side_dist_y → move X
        P→──●     (hit vertical line at x=3)
             
Step 2: side_dist_x > side_dist_y → move Y
        P→────●   (hit horizontal line at y=2)
              │
              ↓
              
Step 3: side_dist_x < side_dist_y → move X
        P→────●   
              │
              └──●  (hit vertical line at x=4)
              
Step 4: Found wall at (4, 2)!
```

---

## 6. Wall Rendering & Texturing

### 6.1 Perpendicular Distance (Fisheye Correction)

```c
// raycast_helper.c
void get_perp_wall_distance(t_game_data *data)
{
    if (data->rc.kind == HIT_DOOR)
        return;  // Doors calculate distance differently
    
    if (data->rc.side == 0)  // Vertical wall (X-side)
    {
        data->rc.perp_wall_dist = (data->rc.map_x - data->player.pos.x 
            + (1 - data->rc.step_x) / 2.0) / data->rc.ray_dir_x;
    }
    else  // Horizontal wall (Y-side)
    {
        data->rc.perp_wall_dist = (data->rc.map_y - data->player.pos.y 
            + (1 - data->rc.step_y) / 2.0) / data->rc.ray_dir_y;
    }
}
```

#### Why Perpendicular Distance?

If we use **Euclidean distance** (straight-line distance), walls would appear curved like a fisheye lens:

```
Euclidean Distance:                Perpendicular Distance:
                                   
    ╭───────────────────╮              ┌───────────────────┐
   ╱                     ╲             │                   │
  │                       │            │                   │
  │                       │            │                   │
   ╲                     ╱             │                   │
    ╰───────────────────╯              └───────────────────┘
    
  WRONG (fisheye effect)              CORRECT (flat walls)
```

**Mathematical Proof**:

The perpendicular distance is the **projection** of the ray onto the view direction:

$$d_{perp} = \frac{\vec{ray} \cdot \vec{dir}}{|\vec{dir}|} = \frac{(\text{wall\_pos} - \text{player\_pos})}{\text{ray\_dir}}$$

This gives the distance to the **camera plane**, not to the player.

### 6.2 Wall Height Calculation

```c
// raycast_helper_4.c
void set_line_height(t_game_data *data, int view_height)
{
    data->rc.line_height = (int)(view_height / data->rc.perp_wall_dist);
}
```

#### The Math

If a wall is at distance $d$, its **projected height** on screen is:

$$h_{screen} = \frac{h_{wall} \cdot h_{view}}{d}$$

Since walls have height 1 (one grid unit), this simplifies to:

$$h_{screen} = \frac{h_{view}}{d}$$

**Visual Impact**: Closer walls → larger `line_height` → taller stripe on screen.

### 6.3 Drawing Boundaries

```c
// raycast_helper_3.c
void set_drawing_ends(t_game_data *data, int view_height)
{
    data->rc.draw_start = -data->rc.line_height / 2 + view_height / 2;
    if (data->rc.draw_start < 0)
        data->rc.draw_start = 0;
    
    data->rc.draw_end = data->rc.line_height / 2 + view_height / 2;
    if (data->rc.draw_end >= view_height)
        data->rc.draw_end = view_height - 1;
}
```

**Why center ± half?** The wall is centered vertically on screen:

```
Screen (view_height = 600):

y = 0   ┌──────────────────┐  ← Ceiling
        │▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│
y = 150 ├──────────────────┤  ← draw_start (300 - 150)
        │▒▒▒▒▒▒ WALL ▒▒▒▒▒▒│
y = 300 │      CENTER      │  ← screen center
        │▒▒▒▒▒▒ WALL ▒▒▒▒▒▒│
y = 450 ├──────────────────┤  ← draw_end (300 + 150)
        │▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│
y = 599 └──────────────────┘  ← Floor
```

### 6.4 Texture Coordinate (wall_x)

```c
// raycast_helper_3.c
void set_texture_coordinations(t_game_data *data)
{
    if (data->rc.kind == HIT_DOOR)
        return;
    
    if (data->rc.side == 0)  // Vertical wall
        data->rc.wall_x = data->player.pos.y + data->rc.perp_wall_dist 
            * data->rc.ray_dir_y;
    else  // Horizontal wall
        data->rc.wall_x = data->player.pos.x + data->rc.perp_wall_dist 
            * data->rc.ray_dir_x;
    
    data->rc.wall_x -= floor(data->rc.wall_x);  // Get fractional part [0, 1)
}
```

#### Explanation

**wall_x** is where the ray hit the wall as a fraction [0, 1):

```
Wall texture (64×64 pixels):
0.0        0.25       0.5        0.75       1.0
 │          │          │          │          │
 ▼          ▼          ▼          ▼          ▼
┌──────────┬──────────┬──────────┬──────────┐
│   ░░░    │    ▓▓    │    ░░    │    ▓▓    │
│   ░░░    │    ▓▓    │    ░░    │    ▓▓    │
│   ░░░    │    ▓▓    │    ░░    │    ▓▓    │
└──────────┴──────────┴──────────┴──────────┘
     0         16         32         48      63
```

If `wall_x = 0.375`, then `tex_x = 0.375 × 64 = 24`.

### 6.5 Texture Flipping

```c
// raycast_helper_2.c
int flip_text_horizontally(t_game_data *data, t_texture *current_tex)
{
    int tex_x;

    tex_x = (int)(data->rc.wall_x * (double)current_tex->width);
    
    // Flip for consistent texture orientation
    if ((data->rc.side == 0 && data->rc.ray_dir_x > 0) 
        || (data->rc.side == 1 && data->rc.ray_dir_y < 0))
    {
        tex_x = current_tex->width - tex_x - 1;
    }
    
    // Clamp to valid range
    if (tex_x < 0)
        tex_x = 0;
    else if (tex_x >= current_tex->width)
        tex_x = current_tex->width - 1;
    
    return tex_x;
}
```

**Why flip?** Textures should appear consistent when viewed from different sides. Without flipping, text would appear backwards on some walls.

### 6.6 Texture Selection (N/S/E/W)

```c
// raycast_helper_3.c
t_texture *get_current_texture(t_game_data *data, t_st *tex)
{
    if (data->rc.side == 0)  // Vertical wall (X-side)
    {
        if (data->rc.ray_dir_x > 0)
            return (&tex->tex_ea);  // Looking East → East texture
        else
            return (&tex->tex_we);  // Looking West → West texture
    }
    else  // Horizontal wall (Y-side)
    {
        if (data->rc.ray_dir_y > 0)
            return (&tex->tex_so);  // Looking South → South texture
        else
            return (&tex->tex_no);  // Looking North → North texture
    }
}
```

### 6.7 Drawing the Vertical Stripe

```c
// raycast_helper_2.c
void draw(t_game_data *data, t_texture *tex, t_infos infos)
{
    int             y;
    unsigned int    color;
    double          tex_step;
    double          tex_pos;

    // How much to step in texture Y per screen pixel
    tex_step = (double)tex->height / data->rc.line_height;
    
    // Starting texture position (handles walls taller than screen)
    tex_pos = (data->rc.draw_start - infos.view_height / 2.0 
        + data->rc.line_height / 2.0) * tex_step;
    
    y = data->rc.draw_start;
    while (y <= data->rc.draw_end)
    {
        tex->tex_y = (int)tex_pos;
        
        // Clamp to valid range
        if (tex->tex_y < 0)
            tex->tex_y = 0;
        else if (tex->tex_y >= tex->height)
            tex->tex_y = tex->height - 1;
        
        tex_pos += tex_step;
        
        // Sample color from texture
        color = *(unsigned int *)(tex->addr 
            + tex->tex_y * tex->line_len 
            + tex->tex_x * (tex->bpp / 8));
        
        put_pixel(&data->mlx, infos.start_x + infos.x, y, color);
        y++;
    }
}
```

#### Texture Mapping Formula

$$\text{tex\_step} = \frac{\text{tex\_height}}{\text{line\_height}}$$

This scales the texture to fit the wall height. If `line_height = 300` and `tex_height = 64`:
- `tex_step = 64/300 ≈ 0.21`
- Each screen pixel advances 0.21 texture pixels

---

## 7. Door System

### 7.1 Door Data Structure

```c
typedef struct s_door
{
    int             map_x;        // Grid X position
    int             map_y;        // Grid Y position
    double          progress;     // 0.0 = closed, 1.0 = open
    double          speed;        // Opening/closing speed
    t_door_state    state;        // CLOSED, OPENING, OPEN, CLOSING
    t_vec2          pivot;        // Hinge position
    t_vec2          span_closed;  // Door span when closed
    int             rot_sign;     // +1 or -1 (rotation direction)
    bool            has_geom;     // Geometry configured?
} t_door;
```

### 7.2 Door as a Rotating Segment

Unlike walls (which are axis-aligned grid lines), doors are **rotating line segments**:

```
Closed (progress = 0):              Open (progress = 1):

    ┌─────────────┐                     ┌─────────────┐
    │      │      │                     │      ╱      │
    │  P   │  D   │                     │  P  ╱   D   │
    │      │      │                     │    ╱        │
    │      │ <─span                     │   ╱         │
    │      │      │                     │  ╱ ← rotated span
    │      │ pivot                      │ ◉ pivot     │
    └─────────────┘                     └─────────────┘
```

### 7.3 Door Geometry Configuration

```c
// door_geo.c
void init_door_geo(t_game_data *data, t_door *door, t_door_geo *geo)
{
    geo->center = vec2(door->map_x + 0.5, door->map_y + 0.5);
    
    // Check which sides are open (walkable)
    geo->vertical_open = 
        map_is_walkable(data, door->map_y - 1, door->map_x) +
        map_is_walkable(data, door->map_y + 1, door->map_x);
    geo->horizontal_open = 
        map_is_walkable(data, door->map_y, door->map_x - 1) +
        map_is_walkable(data, door->map_y, door->map_x + 1);
    
    // Door spans perpendicular to the passage
    if (geo->vertical_open > geo->horizontal_open)
        geo->span_dir = vec2(1.0, 0.0);  // Horizontal door
    else
        geo->span_dir = vec2(0.0, 1.0);  // Vertical door
}
```

**Visual Impact**: The algorithm detects which direction the corridor runs and orients the door accordingly.

### 7.4 Door Rotation

```c
// raycast.c
static t_vec2 door_current_span(const t_door *door)
{
    double angle;
    
    angle = door->rot_sign * door->progress * HALF_PI;  // 0 to ±90°
    return vec2_rotate(door->span_closed, angle);
}
```

As `progress` goes from 0 to 1, the door rotates 90° around its pivot:

$$\vec{span}_{current} = \text{rotate}(\vec{span}_{closed}, \pm 90° \times \text{progress})$$

### 7.5 Ray-Door Intersection

```c
// raycast.c
int door_ray_intersection(t_game_data *data, t_door *door, 
                          double *hit_dist, double *u)
{
    t_vec2 origin, dir, span, diff;
    double denom, t, s;

    if (!door || !door->has_geom)
        return 0;
    
    set_origin_dir(data, &origin, &dir);
    span = door_current_span(door);
    
    // Cross product for intersection (2D line-line)
    denom = vec2_cross(dir, span);
    if (fabs(denom) < 1e-8)
        return 0;  // Parallel lines
    
    diff = vec2_sub(door->pivot, origin);
    t = vec2_cross(diff, span) / denom;  // Distance along ray
    s = vec2_cross(diff, dir) / denom;   // Position on door [0,1]
    
    if (t <= 1e-4 || s < 0.0 || s > 1.0)
        return 0;  // Behind player or missed door
    
    if (hit_dist) *hit_dist = t;
    if (u) *u = s;  // For texture mapping
    return 1;
}
```

#### Mathematical Proof: 2D Line Intersection

Given:
- Ray: $P + t \cdot D$ (origin P, direction D)
- Door segment: $Q + s \cdot S$ (pivot Q, span S, $s \in [0,1]$)

Setting them equal:
$$P + t \cdot D = Q + s \cdot S$$
$$t \cdot D - s \cdot S = Q - P$$

Using Cramer's rule with 2D cross product:
$$t = \frac{(Q - P) \times S}{D \times S}$$
$$s = \frac{(Q - P) \times D}{D \times S}$$

If $D \times S = 0$, the lines are parallel (no intersection).

---

## 8. Sprite System

### 8.1 Sprite Structure

```c
typedef struct s_sprite
{
    int             map_x, map_y;     // Grid position
    t_vec2          position;         // World position
    t_vec2          velocity;         // Movement
    double          speed;            // Movement speed
    double          dist_cam;         // Distance to camera
    double          cam_x, cam_z;     // Position in camera space
    int             frame;            // Animation frame
    int             frame_w, frame_h; // Frame dimensions
    int             frame_count;      // Total frames
    double          frame_time;       // Animation timer
    t_texture       *sp_tex;          // Sprite sheet texture
    bool            visible;          // In front of camera?
    t_draw_sprite   draw;             // Drawing parameters
} t_sprite;
```

### 8.2 Camera Space Transform

Sprites exist in world space but must be transformed to **camera space** for rendering:

```c
// sprite_helper.c
void sprite_camera_transform(t_game_data *data, t_sprite *sprite)
{
    t_vec2  rel;
    double  det, inv_det;

    // Vector from player to sprite
    rel = vec2_sub(sprite->position, data->player.pos);
    
    // Determinant of camera matrix
    det = data->player.plane.x * data->player.dir.y 
        - data->player.dir.x * data->player.plane.y;
    
    if (fabs(det) < 1e-9)
    {
        sprite->visible = false;
        return;
    }
    
    inv_det = 1.0 / det;
    
    // Transform to camera space
    sprite->cam_x = inv_det * (data->player.dir.y * rel.x 
                             - data->player.dir.x * rel.y);
    sprite->cam_z = inv_det * (-data->player.plane.y * rel.x 
                             + data->player.plane.x * rel.y);
    
    set_cam_z(sprite, rel);  // Set visibility
}
```

#### The Camera Transform Matrix

The camera is defined by two vectors: **dir** (forward) and **plane** (right).

To transform world coordinates to camera space, we use the inverse of:

$$C = \begin{pmatrix} plane_x & dir_x \\ plane_y & dir_y \end{pmatrix}$$

The inverse is:

$$C^{-1} = \frac{1}{det(C)} \begin{pmatrix} dir_y & -dir_x \\ -plane_y & plane_x \end{pmatrix}$$

Where $det(C) = plane_x \cdot dir_y - dir_x \cdot plane_y$.

Result:
- **cam_x**: Horizontal position in camera space (for screen X)
- **cam_z**: Depth (distance from camera plane)

### 8.3 Screen Projection

```c
// sprite_helper.c
void project_to_screen(t_sprite *sprite, int start_x, int v_w, int v_h)
{
    sprite->draw.screen_x = start_x + (int)((v_w / 2.0) * (1.0 
        + sprite->cam_x * sprite->draw.inv_z));
    sprite->draw.sprite_height = (int)fabs(v_h * sprite->draw.inv_z);
    sprite->draw.sprite_width = sprite->draw.sprite_height;
}
```

#### Projection Formula

$$screen_x = \frac{width}{2} + \frac{width}{2} \cdot \frac{cam_x}{cam_z}$$

$$sprite\_size = \frac{view\_height}{cam_z}$$

**Visual Impact**: Sprites closer to camera appear larger and centered differently.

### 8.4 Sprite Depth Sorting

Sprites must be drawn **back-to-front** (painter's algorithm):

```c
// sprite_helper1.c
t_sprite **sort_sprites(t_game_data *data)
{
    int         i, j;
    t_sprite    **visible_sprites;

    data->v_sprite_counter = collect_visible_sprites(data, &visible_sprites);
    if (!data->v_sprite_counter || !visible_sprites)
        return NULL;
    
    // Simple selection sort by distance (descending - far to near)
    i = 0;
    while (i < data->v_sprite_counter - 1)
    {
        j = i + 1;
        while (j < data->v_sprite_counter)
        {
            if (visible_sprites[i]->cam_z < visible_sprites[j]->cam_z)
                swap(visible_sprites, i, j);
            j++;
        }
        i++;
    }
    return visible_sprites;
}
```

### 8.5 Z-Buffer Occlusion

Sprites should be hidden behind walls:

```c
// sprite_helper4.c
bool render_sprite_x_incr(t_render_sprite *render_s, 
                          t_game_data *data, t_sprite *sprite)
{
    // Skip this column if:
    // 1. Invalid position
    // 2. Sprite is further than the wall at this column
    if (!data->z_buffer 
        || render_s->x < 0 
        || render_s->x >= data->z_buffer_size
        || data->z_buffer[render_s->x] <= 0.0
        || sprite->cam_z >= data->z_buffer[render_s->x])
    {
        render_s->x++;
        return true;  // Skip this column
    }
    return false;
}
```

```
Z-Buffer visualization:

Screen columns:     0    50   100   150   200   250
Z-Buffer (wall):   [5.0, 4.2, 3.1, 2.8, 3.5, 4.0]
                     │    │    │    │    │    │
Sprite at z=3.5:     ✓    ✓    ×    ×    ✓    ✓
                   drawn drawn hidden hidden drawn drawn
```

### 8.6 Sprite Sheet Animation

```c
// sprite_helper2.c
void sprite_animate(t_sprite *sprite, double dt)
{
    if (!sprite || sprite->frame_count <= 0)
        return;
    
    sprite->frame_time += dt;
    if (sprite->frame_time >= sprite->frame_duration)
    {
        sprite->frame_time -= sprite->frame_duration;
        sprite->frame = (sprite->frame + 1) % sprite->frame_count;
    }
}
```

**Visual Impact**: Sprites cycle through animation frames, creating movement illusion.

```
Sprite sheet (3 frames):
┌─────────┬─────────┬─────────┐
│ Frame 0 │ Frame 1 │ Frame 2 │
│  ╭─╮    │  ╭─╮    │  ╭─╮    │
│  │O│    │  │O│    │  │O│    │
│  ╱ ╲    │ ╱   ╲   │╱     ╲  │  ← Walking animation
└─────────┴─────────┴─────────┘
```

---

## 9. Z-Buffer & Occlusion

### 9.1 What is the Z-Buffer?

The Z-buffer (depth buffer) stores the distance to the nearest surface for each screen column. It enables:
1. **Sprite occlusion**: Hide sprites behind walls
2. **Proper depth ordering**: Multiple sprites at different depths

```c
// raycast_helper_2.c
bool ensure_z_buffer(t_game_data *data, int width)
{
    if (data->z_buffer && data->z_buffer_size == width)
        return true;
    
    free(data->z_buffer);
    data->z_buffer = malloc(sizeof(double) * width);
    data->z_buffer_size = width;
    return (data->z_buffer != NULL);
}
```

### 9.2 Filling the Z-Buffer

During raycasting, each column's wall distance is stored:

```c
// raycast.c - render_3d_view()
data->z_buffer[start_x + x] = data->rc.perp_wall_dist;
```

---

## 10. Movement & Collision

### 10.1 Rotation

```c
// movement_3.c
void rotate_player(t_game_data *data, double angle)
{
    double old_dir_x = data->player.dir.x;
    double old_plane_x = data->player.plane.x;
    
    // Rotate direction vector
    data->player.dir.x = old_dir_x * cos(angle) 
                       - data->player.dir.y * sin(angle);
    data->player.dir.y = old_dir_x * sin(angle) 
                       + data->player.dir.y * cos(angle);
    
    // Rotate camera plane (must stay perpendicular)
    data->player.plane.x = old_plane_x * cos(angle) 
                         - data->player.plane.y * sin(angle);
    data->player.plane.y = old_plane_x * sin(angle) 
                         + data->player.plane.y * cos(angle);
}
```

**Critical**: Both dir AND plane must rotate together. If only dir rotates, the FOV would distort.

### 10.2 Forward/Backward Movement

```c
// movement_2.c
void move_forward(t_game_data *data)
{
    double step_x = data->player.dir.x * data->player.move_speed;
    double step_y = data->player.dir.y * data->player.move_speed;
    apply_move_with_slide(data, step_x, step_y);
}

void move_backwards(t_game_data *data)
{
    double step_x = -(data->player.dir.x * data->player.move_speed);
    double step_y = -(data->player.dir.y * data->player.move_speed);
    apply_move_with_slide(data, step_x, step_y);
}
```

### 10.3 Strafing (Side Movement)

```c
// movement_2.c
void strafe_move(t_game_data *data, int direction)
{
    double step_x = direction * data->player.dir.y * data->player.move_speed;
    double step_y = direction * -data->player.dir.x * data->player.move_speed;
    apply_move_with_slide(data, step_x, step_y);
}
```

**Why (dir.y, -dir.x)?** This is the perpendicular vector:

$$\vec{strafe} = \vec{dir}^{\perp} = (dir_y, -dir_x)$$

```
        dir = (0, 1) (facing South)
             ↓
           ┌─●─┐
   strafe ← │ │ → strafe
   (-1,0)   └───┘   (1,0)
```

### 10.4 Wall Sliding Collision

```c
// movement_2.c
static void apply_move_with_slide(t_game_data *data, double step_x, double step_y)
{
    double base_x = data->player.pos.x;
    double base_y = data->player.pos.y;
    
    // Try full movement first
    if (valid_move(data, base_x + step_x, base_y + step_y))
    {
        data->player.pos.x = base_x + step_x;
        data->player.pos.y = base_y + step_y;
        return;
    }
    
    // Wall slide: try X-only movement
    if (step_x != 0.0 && valid_move(data, base_x + step_x, base_y))
        base_x += step_x;
    
    // Wall slide: try Y-only movement
    if (step_y != 0.0 && valid_move(data, base_x, base_y + step_y))
        base_y += step_y;
    
    data->player.pos.x = base_x;
    data->player.pos.y = base_y;
}
```

**Visual Impact**: When walking into a wall diagonally, you slide along it instead of stopping dead.

### 10.5 Collision Detection with Radius

```c
// movement_2.c
int valid_move(t_game_data *data, double new_x, double new_y)
{
    double r = PLAYER_COLLISION_RADIUS;  // 0.2 units
    
    // Check all 9 points around the player
    if (blocked_at(data, new_x, new_y)) return 0;
    if (blocked_at(data, new_x + r, new_y)) return 0;
    if (blocked_at(data, new_x - r, new_y)) return 0;
    if (blocked_at(data, new_x, new_y + r)) return 0;
    if (blocked_at(data, new_x, new_y - r)) return 0;
    if (blocked_at(data, new_x + r, new_y + r)) return 0;
    if (blocked_at(data, new_x + r, new_y - r)) return 0;
    if (blocked_at(data, new_x - r, new_y + r)) return 0;
    if (blocked_at(data, new_x - r, new_y - r)) return 0;
    
    return 1;
}
```

```
Collision points checked:
      
    ●───●───●
    │       │
    ●   P   ●   ← Player with radius r
    │       │
    ●───●───●
    
All 9 points must be in valid (non-wall) cells
```

---

## Summary: The Complete Rendering Pipeline

```
1. FOR EACH FRAME:
   ├── Calculate delta time (dt)
   ├── Update door animations (progress += speed * dt)
   ├── Update sprite AI and animations
   │
   ├── FOR EACH SCREEN COLUMN (x = 0 to WIDTH):
   │   ├── Calculate camera_x = (2 * x / WIDTH) - 1
   │   ├── Calculate ray_dir = dir + plane * camera_x
   │   ├── Initialize DDA (step directions, delta distances)
   │   │
   │   ├── DDA LOOP:
   │   │   ├── Step to next grid line (X or Y, whichever is closer)
   │   │   ├── Check if wall ('1') → hit_wall(), break
   │   │   └── Check if door ('D') → ray-segment intersection test
   │   │
   │   ├── Calculate perpendicular distance (fisheye correction)
   │   ├── Calculate wall height = screen_height / distance
   │   ├── Calculate draw_start and draw_end (centered)
   │   ├── Calculate texture X coordinate (wall_x)
   │   │
   │   ├── FOR EACH PIXEL IN COLUMN (y = draw_start to draw_end):
   │   │   ├── Calculate texture Y coordinate
   │   │   ├── Sample color from texture
   │   │   └── Put pixel to framebuffer
   │   │
   │   ├── Draw ceiling (y = 0 to draw_start)
   │   ├── Draw floor (y = draw_end to HEIGHT)
   │   └── Store distance in z_buffer[x]
   │
   ├── RENDER SPRITES:
   │   ├── Transform all sprites to camera space
   │   ├── Filter visible sprites (cam_z > 0)
   │   ├── Sort by distance (back to front)
   │   │
   │   └── FOR EACH SPRITE:
   │       ├── Calculate screen position and size
   │       └── FOR EACH SPRITE COLUMN:
   │           ├── Check z_buffer (skip if behind wall)
   │           └── FOR EACH SPRITE PIXEL:
   │               ├── Sample from sprite sheet
   │               ├── Skip transparent pixels
   │               └── Put pixel to framebuffer
   │
   ├── Render gun overlay
   ├── Render minimap
   └── mlx_put_image_to_window()
```

---

## References & Further Reading

1. **Lode's Computer Graphics Tutorial** - The classic raycasting tutorial
   - https://lodev.org/cgtutor/raycasting.html

2. **Permadi's Raycasting Tutorial** - Mathematical foundations
   - https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/

3. **Wolfenstein 3D Black Book** - Detailed analysis of the original engine
   - by Fabien Sanglard

4. **Linear Algebra for Game Developers** - Vector and matrix operations
   - https://www.gamedev.net/tutorials/programming/math-and-physics/

5. **42 Subject: cub3D** - Project specifications
   - https://projects.intra.42.fr/

---

*Document created for cub3D project - December 2025*

# COMPLETE VECTOR GUIDE FOR CUB3D
## Visual Explanations & Geometric Effects

---

## 1. VECTOR BASICS

### What is a Vector?
A vector is an arrow with:
- **Magnitude** (length)
- **Direction** (which way it points)

```
Vector v = (3, 4)

    4 ↑     End point (3,4)
      |    /
      |   /
      |  / 
      | /  length = 5
    0 +−−−→
      0   3
      
Start at origin (0,0), end at (3,4)
```

---

## 2. VECTOR CREATION

### vec2_new(x, y)
Creates a vector from components.

```c
t_vec2 v = vec2_new(3.0, 4.0);
// v.x = 3.0, v.y = 4.0
```

Visual:
```
    y
    ↑
  4 +       • (3,4)
    |      /
  2 |     /
    |    / v
  0 +−−−+−−−→ x
    0   3
```

---

## 3. VECTOR ADDITION

### vec2_add(a, b)
**Formula:** `(ax + bx, ay + by)`

**Geometric meaning:** Place vectors tip-to-tail

```
a = (2, 1)    b = (1, 2)    Result = (3, 3)

      b               
     ↗|               
    / |              • (3,3)
   /  |             /
  +−−→ a      →    / a+b
                  /
                 +
```

**Application in CUB3D:**
```c
// Move player forward
t_vec2 pos = vec2_new(10.0, 10.0);
t_vec2 dir = vec2_new(1.0, 0.0);
t_vec2 new_pos = vec2_add(pos, vec2_scale(dir, 0.5));
// new_pos = (10.5, 10.0) - moved 0.5 units right
```

---

## 4. VECTOR SUBTRACTION

### vec2_sub(a, b)
**Formula:** `(ax - bx, ay - by)`

**Geometric meaning:** Vector FROM b TO a

```
a = (5, 3)    b = (2, 1)    Result = (3, 2)

      • a (5,3)
     /
    / a-b = (3,2)
   /
  • b (2,1)
```

**Application in CUB3D:**
```c
// Get direction to target
t_vec2 player_pos = vec2_new(10.0, 10.0);
t_vec2 enemy_pos = vec2_new(15.0, 12.0);
t_vec2 to_enemy = vec2_sub(enemy_pos, player_pos);
// to_enemy = (5, 2) - vector pointing at enemy
```

---

## 5. SCALAR MULTIPLICATION

### vec2_scale(v, scalar)
**Formula:** `(vx × k, vy × k)`

**Geometric meaning:** Stretch/shrink vector

```
v = (2, 1)

2v = (4, 2)  ←−−−−→  (twice as long)
v  = (2, 1)  ←−−→    (original)
0.5v = (1, 0.5) ←→   (half length)
-v = (-2, -1) ←−−→   (opposite direction)
```

**Application in CUB3D:**
```c
// Control movement speed
t_vec2 dir = vec2_new(1.0, 0.0);
t_vec2 slow = vec2_scale(dir, 0.1);  // Slow movement
t_vec2 fast = vec2_scale(dir, 0.5);  // Fast movement
```

---

## 6. VECTOR MAGNITUDE (LENGTH)

### vec2_magnitude(v)
**Formula:** `√(x² + y²)`

**Geometric meaning:** Length of the arrow

```
v = (3, 4)

    4 •
      |\\
      | \\  length = √(3²+4²) 
      |  \\        = √25 = 5
    0 +−−−•
      0   3
```

**Application in CUB3D:**
```c
// Check distance to target
t_vec2 to_enemy = vec2_sub(enemy_pos, player_pos);
double distance = vec2_magnitude(to_enemy);
if (distance < 2.0)
    // Enemy is close!
```

---

## 7. VECTOR NORMALIZATION

### vec2_normalize(v)
**Result:** Vector with length = 1, same direction

**Formula:** `v / |v| = (x/|v|, y/|v|)`

```
v = (3, 4)     →     v̂ = (0.6, 0.8)
Length = 5            Length = 1

    4 •                   0.8 •
      |\\                     |\\
      | \\  ÷5  →            | \\ 
      |  \\                  |  \\
    0 +−−−•              0  +−−−•
      0   3                 0  0.6
```

**Application in CUB3D:**
```c
// Direction vector should always be normalized
t_vec2 dir = vec2_new(3.0, 4.0);  // Random direction
dir = vec2_normalize(dir);         // Now length = 1
// dir = (0.6, 0.8)
// Moving with this direction is consistent speed
```

---

## 8. DOT PRODUCT

### vec2_dot(a, b)
**Formula:** `ax×bx + ay×by`
**Alternative:** `|a| × |b| × cos(θ)`

**Geometric meaning:** How much vectors align

```
a = (1, 0) →    b = (1, 1) ↗

a · b = 1×1 + 0×1 = 1

Interpretation:
> 0  = pointing in similar direction (angle < 90°)
= 0  = perpendicular (angle = 90°)
< 0  = pointing opposite (angle > 90°)
```

**Visual Examples:**

```
Same direction:          Perpendicular:           Opposite:
a →  b →                a →                      a →  ← b
a·b = large positive    a·b = 0                  a·b = negative
                           ↑ b
```

**Application in CUB3D:**
```c
// Check if player is facing enemy
t_vec2 player_dir = vec2_new(-1.0, 0.0);  // Facing north
t_vec2 to_enemy = vec2_sub(enemy_pos, player_pos);
to_enemy = vec2_normalize(to_enemy);

double facing = vec2_dot(player_dir, to_enemy);
if (facing > 0.7)  // cos(45°) ≈ 0.7
    // Player is looking at enemy (within 45° cone)
```

---

## 9. CROSS PRODUCT (2D)

### vec2_cross(a, b)
**Formula:** `ax×by - ay×bx`

**Geometric meaning:** Which side? Left or right?

```
a = (1, 0) →    b = (1, 1) ↗

a × b = 1×1 - 0×1 = 1 > 0
→ b is to the LEFT of a

a = (1, 0) →    b = (1, -1) ↘

a × b = 1×(-1) - 0×1 = -1 < 0
→ b is to the RIGHT of a
```

**Application in CUB3D:**
```c
// Is enemy on left or right of player?
t_vec2 player_dir = vec2_new(-1.0, 0.0);
t_vec2 to_enemy = vec2_sub(enemy_pos, player_pos);

double side = vec2_cross(player_dir, to_enemy);
if (side > 0)
    printf("Enemy is on your LEFT\n");
else if (side < 0)
    printf("Enemy is on your RIGHT\n");
else
    printf("Enemy is AHEAD or BEHIND\n");
```

---

## 10. VECTOR ROTATION

### vec2_rotate(v, angle)
**Formula:**
```
x' = x×cos(θ) - y×sin(θ)
y' = x×sin(θ) + y×cos(θ)
```

**Geometric meaning:** Spin vector around origin

```
v = (1, 0) →

Rotate 90° CCW:
    ↑ (0, 1)
    |
    +−→ (1, 0)

Rotate 45° CCW:
     ↗ (0.707, 0.707)
    /
   +−→ (1, 0)
```

**Application in CUB3D:**
```c
// Player turns left (A key pressed)
t_vec2 dir = vec2_new(-1.0, 0.0);      // Facing north
t_vec2 plane = vec2_new(0.0, 0.66);    // Camera plane

double rot_speed = 0.05;  // ~3 degrees

// Rotate both direction and camera plane
dir = vec2_rotate(dir, rot_speed);
plane = vec2_rotate(plane, rot_speed);
```

---

## 11. PERPENDICULAR VECTOR

### vec2_perpendicular(v)
**Formula:** `(-y, x)` (90° counterclockwise)

**Geometric meaning:** Turn vector left 90°

```
v = (3, 0) →

perp = (0, 3)
       ↑
       |
       +−→ v
```

**Application in CUB3D:**
```c
// Camera plane is perpendicular to view direction
t_vec2 dir = vec2_new(-1.0, 0.0);
t_vec2 plane = vec2_perpendicular(dir);
plane = vec2_scale(plane, 0.66);  // FOV adjustment
// plane = (0.0, 0.66) - perpendicular to north
```

---

## 12. DISTANCE BETWEEN POINTS

### vec2_distance(a, b)
**Formula:** `|b - a| = √((bx-ax)² + (by-ay)²)`

```
a = (1, 1)    b = (4, 5)

    5 • b
      |\\
      | \\
    1 • a \\  distance = √((4-1)² + (5-1)²)
              \\             = √(9 + 16) = 5
```

**Application in CUB3D:**
```c
// Check if near wall for collision
t_vec2 player = vec2_new(10.5, 10.5);
t_vec2 wall = vec2_new(12.0, 10.0);
double dist = vec2_distance(player, wall);
if (dist < 0.3)
    // Too close to wall, prevent movement
```

---

## 13. ANGLE BETWEEN VECTORS

### vec2_angle_between(a, b)
**Formula:** `θ = arccos((a·b) / (|a|×|b|))`

```
a = (1, 0) →    b = (1, 1) ↗

Angle = arccos(1 / (1 × √2)) = 45°
```

---

## 14. LINEAR INTERPOLATION (LERP)

### vec2_lerp(a, b, t)
**Formula:** `a×(1-t) + b×t`

**Geometric meaning:** Point between a and b

```
a = (0, 0)    b = (4, 0)    t = 0.5

lerp = (2, 0)  (halfway between)

  +−−−•−−−+ 
  a   ↑   b
      result
```

**Application in CUB3D:**
```c
// Smooth camera movement
t_vec2 current_pos = vec2_new(10.0, 10.0);
t_vec2 target_pos = vec2_new(15.0, 10.0);
t_vec2 smooth = vec2_lerp(current_pos, target_pos, 0.1);
// smooth = (10.5, 10.0) - move 10% toward target
```

---

## COMPLETE MOVEMENT EXAMPLE

```c
// Player movement with vectors
typedef struct s_player {
    t_vec2  position;
    t_vec2  direction;   // Normalized
    t_vec2  plane;       // Perpendicular to direction
    double  move_speed;
    double  rot_speed;
} t_player;

// Move forward
void move_forward(t_player *p) {
    t_vec2 velocity = vec2_scale(p->direction, p->move_speed);
    p->position = vec2_add(p->position, velocity);
}

// Rotate left
void rotate_left(t_player *p) {
    p->direction = vec2_rotate(p->direction, p->rot_speed);
    p->plane = vec2_rotate(p->plane, p->rot_speed);
}

// Check collision
int will_collide(t_player *p, char **map) {
    t_vec2 next_pos = vec2_add(p->position, 
                               vec2_scale(p->direction, p->move_speed));
    int grid_x = (int)next_pos.x;
    int grid_y = (int)next_pos.y;
    return (map[grid_y][grid_x] == '1');
}
```

---

## SUMMARY OF OPERATIONS

| Operation | Formula | Geometric Effect | CUB3D Use |
|-----------|---------|------------------|-----------|
| **Add** | `(ax+bx, ay+by)` | Combine movements | Movement |
| **Subtract** | `(ax-bx, ay-by)` | Direction between points | Aiming |
| **Scale** | `(kx, ky)` | Stretch/shrink | Speed control |
| **Magnitude** | `√(x²+y²)` | Length | Distance |
| **Normalize** | `v/|v|` | Unit direction | Consistent speed |
| **Dot** | `ax×bx+ay×by` | Alignment | Facing check |
| **Cross** | `ax×by-ay×bx` | Left/right | Side detection |
| **Rotate** | Matrix | Turn around | Player rotation |
| **Perpendicular** | `(-y,x)` | 90° turn | Camera plane |

---

This library gives you all the tools needed for smooth, vector-based player movement in CUB3D!

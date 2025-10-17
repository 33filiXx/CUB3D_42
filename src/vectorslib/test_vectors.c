#include "vec.h"
#include <stdio.h>

int main(void)
{
	t_vec2	v1, v2, result;
	double	scalar_result;

	printf("=== VECTOR LIBRARY TEST ===\n\n");

	// 1. Vector creation
	v1 = vec2_new(3.0, 4.0);
	v2 = vec2_new(1.0, 2.0);
	printf("v1 = (%.1f, %.1f)\n", v1.x, v1.y);
	printf("v2 = (%.1f, %.1f)\n\n", v2.x, v2.y);

	// 2. Addition
	result = vec2_add(v1, v2);
	printf("v1 + v2 = (%.1f, %.1f)\n\n", result.x, result.y);

	// 3. Subtraction
	result = vec2_sub(v1, v2);
	printf("v1 - v2 = (%.1f, %.1f)\n\n", result.x, result.y);

	// 4. Scaling
	result = vec2_scale(v1, 2.0);
	printf("v1 × 2 = (%.1f, %.1f)\n\n", result.x, result.y);

	// 5. Magnitude
	scalar_result = vec2_magnitude(v1);
	printf("|v1| = %.2f\n\n", scalar_result);

	// 6. Normalization
	result = vec2_normalize(v1);
	printf("normalize(v1) = (%.2f, %.2f)\n", result.x, result.y);
	printf("Length: %.2f\n\n", vec2_magnitude(result));

	// 7. Dot product
	scalar_result = vec2_dot(v1, v2);
	printf("v1 · v2 = %.1f\n\n", scalar_result);

	// 8. Cross product
	scalar_result = vec2_cross(v1, v2);
	printf("v1 × v2 = %.1f\n", scalar_result);
	if (scalar_result > 0)
		printf("  → v2 is LEFT of v1\n\n");
	else if (scalar_result < 0)
		printf("  → v2 is RIGHT of v1\n\n");
	else
		printf("  → v2 is PARALLEL to v1\n\n");

	// 9. Rotation
	result = vec2_rotate(vec2_new(1.0, 0.0), 1.5708); // 90 degrees in radians
	printf("Rotate (1,0) by 90°: (%.2f, %.2f)\n\n", result.x, result.y);

	// 10. Perpendicular
	result = vec2_perpendicular(v1);
	printf("Perpendicular to v1: (%.1f, %.1f)\n", result.x, result.y);
	printf("Dot product (should be 0): %.1f\n\n", vec2_dot(v1, result));

	// 11. Distance
	scalar_result = vec2_distance(v1, v2);
	printf("Distance from v1 to v2: %.2f\n\n", scalar_result);

	// 12. Practical example: Player movement
	printf("=== PLAYER MOVEMENT EXAMPLE ===\n");
	t_vec2 player_pos = vec2_new(10.0, 10.0);
	t_vec2 player_dir = vec2_new(-1.0, 0.0); // Facing north
	double move_speed = 0.5;

	printf("Initial position: (%.1f, %.1f)\n", player_pos.x, player_pos.y);
	printf("Direction: (%.1f, %.1f)\n", player_dir.x, player_dir.y);

	// Move forward
	t_vec2 velocity = vec2_scale(player_dir, move_speed);
	player_pos = vec2_add(player_pos, velocity);
	printf("After moving forward: (%.1f, %.1f)\n", player_pos.x, player_pos.y);

	// Rotate 45 degrees left
	player_dir = vec2_rotate(player_dir, 0.785398); // 45° in radians
	printf("After rotating left: (%.2f, %.2f)\n", player_dir.x, player_dir.y);

	return (0);
}

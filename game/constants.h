#ifndef CONSTANTS_H
#define CONSTANTS_H

// system properties
constexpr double physics_timestep = 0.0143;
constexpr const char * window_title = "Hippos of Significant Hunger";
constexpr int resolution_x = 240;
constexpr int resolution_y = 180;
constexpr int starting_window_scale = 3;

// colors
constexpr int ColorLightBlue = 0x249fde;
constexpr int ColorLightGreen = 0x59c135;

constexpr int clear_color = ColorLightGreen;

// physics properties
constexpr double arena_radius = (resolution_y - 5)/2;

constexpr double player_speed_pixels_per_second = 120;
constexpr double marble_launch_speed_pixels_per_second = 40;

constexpr double player_radius = 2.5;
constexpr double marble_radius = 1.5;
constexpr double player_collision_radius = 10; // the radius that the player uses against the arena

constexpr double player_bite_radius = 7;

constexpr double launcher_fire_rate = 0.7; // seconds per marble
constexpr double launcher_max_angle = 20;  // seconds per marble
constexpr double launcher_sweep_rate = 2;  // seconds for -max to max
constexpr unsigned int launch_yellow_marble_every = 15; // numbers of shots

constexpr double drain_radius = 10;
constexpr double post_radius = 6;

// derived/helper properties
constexpr double player_speed = player_speed_pixels_per_second * physics_timestep;
constexpr double marble_launch_speed = marble_launch_speed_pixels_per_second * physics_timestep;

constexpr double arena_cx = resolution_x / 2;
constexpr double arena_cy = resolution_y / 2;

constexpr double player_bite_distance = player_bite_radius + player_bite_radius * 0.66;

#endif // CONSTANTS_H

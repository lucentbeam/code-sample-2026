#ifndef CONSTANTS_H
#define CONSTANTS_H

// system properties
constexpr double physics_timestep = 0.0143;
constexpr const char * window_title = "Hippos of Significant Hunger";
constexpr int resolution_x = 240;
constexpr int resolution_y = 180;
constexpr int starting_window_scale = 3;

// physics properties
constexpr double arena_radius = (resolution_y - 5)/2;

constexpr double player_speed_pixels_per_second = 80;
constexpr double marble_speed_pixels_per_second = 20;

constexpr double player_radius = 4;
constexpr double marble_radius = 1.5;

constexpr double player_bite_size = 20;

// derived/helper properties
constexpr double player_speed = player_speed_pixels_per_second * physics_timestep;
constexpr double marble_speed = marble_speed_pixels_per_second * physics_timestep;

constexpr double arena_cx = resolution_x / 2;
constexpr double arena_cy = resolution_y / 2;

#endif // CONSTANTS_H

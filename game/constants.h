#ifndef CONSTANTS_H
#define CONSTANTS_H

// system properties
constexpr double physics_timestep = 0.0143;
constexpr const char * window_title = "Hippos of Significant Hunger";
constexpr int resolution_x = 240;
constexpr int resolution_y = 180;
constexpr int starting_window_scale = 3;

// physics properties
constexpr double player_speed_pixels_per_second = 60;
constexpr double marble_speed_pixels_per_second = 20;

// derived/helper properties
constexpr double player_speed = player_speed_pixels_per_second * physics_timestep;
constexpr double marble_speed = marble_speed_pixels_per_second * physics_timestep;

#endif // CONSTANTS_H

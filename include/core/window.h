#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include "utility.h"

struct DrawSettings {
    double scale = 1;

    int tile_id = 0;
    int tiles_wide = 1;
    int tiles_tall = 1;

    Vec2 anchor = Vec2(0.5, 0.5);

    int color = 0xffffff;
    float alpha = 1.0f;

    void setTiled(int id, int width, int height) {
        tile_id = id;
        tiles_wide = width;
        tiles_tall = height;
    }
};

class System;
class Window {
    static bool initialize(std::string window_title, int resolution_x, int resolution_y, double starting_scale);

    static void clear();
    static void update();

    friend System;
public:

    static void setDrawSettings(DrawSettings settings = DrawSettings());

    static void setClearColor(int hex_code);

    static void bind(std::string sprite);
    static void drawBound(double x, double y, double z);

    static void draw(std::string sprite, double x, double y, double z);
    static void print(std::string line, double x, double y, double scale = 1.0);
};

#endif // RENDERER_H

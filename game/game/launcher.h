#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "core/utility.h"

#include "game/marblepool.h"

#include "utils/clock.h"

class Launcher
{
    MarblePool * marbles;

    Vec2 position;
    Vec2 facing;
    Clock timer;

    double accumulator;

    unsigned int shot_counter = 0;

    double angle() const;
public:
    Launcher(MarblePool * pool, Vec2 pos, Vec2 dir);

    void start();
    void stop();

    void update();
    void draw();
};

#endif // LAUNCHER_H

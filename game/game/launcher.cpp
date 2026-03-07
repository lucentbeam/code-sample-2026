#include "launcher.h"

#include "core/window.h"

#include "constants.h"

Launcher::Launcher(MarblePool * pool, Vec2 pos, Vec2 dir) :
    marbles(pool),
    position(pos),
    facing(dir),
    timer(0.3, true)
{

}

void Launcher::start()
{
    timer.reset();
    timer.bind([&](){
        Vec2 f = facing.rot(std::sin(accumulator / 1.0) * 15);
        marbles->create(position + f * 8, f * marble_speed);
    });
}

void Launcher::stop()
{
    timer.bind(nullptr);
}

void Launcher::update()
{
    timer.update(physics_timestep);
    accumulator += physics_timestep;
}

void Launcher::draw()
{
    Window::setDrawSettings();
    Window::draw("launcher", position.x, position.y, 0);
}

#include "launcher.h"

#include "core/window.h"

#include "constants.h"

double Launcher::angle() const
{
    return std::sin(accumulator / launcher_sweep_rate * 3.1415926) * launcher_max_angle;
}

Launcher::Launcher(MarblePool * pool, Vec2 pos, Vec2 dir) :
    marbles(pool),
    position(pos),
    facing(dir),
    timer(launcher_fire_rate, true)
{

}

void Launcher::start()
{
    timer.reset();
    timer.bind([&](){
        shot_counter++;
        Vec2 f = facing.rot(angle());

        bool special = (shot_counter % launch_yellow_marble_every) == 0;
        marbles->create(position + f * 8, f * marble_launch_speed, special ? Marble::Yellow : Marble::Red);

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
    DrawSettings settings;
    settings.rotate(angle());
    Window::setDrawSettings(settings);
    Window::draw("launcher", position.x, position.y, 0);
    Window::setDrawSettings();
}

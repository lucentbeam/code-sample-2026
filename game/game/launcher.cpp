#include "launcher.h"

#include "core/window.h"

#include "constants.h"

Vec2 Launcher::facing() const
{
    double a = std::sin(m_accumulator / launcher_sweep_rate * 3.1415926) * launcher_max_angle;
    return m_facing.rot(a);
}

Launcher::Launcher(MarblePool * pool, Vec2 pos, Vec2 dir) :
    m_marbles(pool),
    m_position(pos),
    m_facing(dir.normalized()),
    m_timer(launcher_fire_rate, true),
    m_shot_counter(rand() % launch_yellow_marble_every)
{

}

void Launcher::update()
{
    m_accumulator += physics_timestep;
    if (m_timer.update(physics_timestep)) {
        m_shot_counter++;
        Vec2 f = facing();

        bool special = (m_shot_counter % launch_yellow_marble_every) == 0;
        m_marbles->create(m_position + f * 8, f * marble_launch_speed, special ? Marble::Yellow : Marble::Red);
    }
}

void Launcher::draw()
{
    DrawSettings settings;
    Vec2 f = facing();
    settings.rotate(float(std::atan2(f.y, f.x)));
    Window::setDrawSettings(settings);
    Window::draw("launcher", m_position.x, m_position.y, 0);
    Window::setDrawSettings();
}

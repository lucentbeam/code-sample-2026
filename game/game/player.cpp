#include "player.h"

#include "core/system.h"
#include "core/window.h"
#include "core/controls.h"

#include "constants.h"

namespace {
    enum AnimationStates : int {
        Idle = 0,
        Bite = 1
    };
}
void Player::init()
{
    m_body = PhysicsBody(arena_cx, arena_cy);
    m_collider.radius = player_radius;

    m_sprite.bind(Idle, Animation());
    m_sprite.bind(Bite, Animation({1,2,3}, 0.24, false), 0);
}

void Player::update()
{
    Vec2 dir = Vec2(Controls::get("horizontal").value(), Controls::get("vertical").value());

    if (Controls::get("bite").pressed()) {
        m_sprite.go(Bite, false);
    }
    m_body.move(dir * player_speed);
}

void Player::draw()
{
    DrawSettings settings;
    settings.setTiled(m_sprite.frame(), 5, 1);
    Window::setDrawSettings(settings);

    Vec2 pos = m_body.getInterpolatedPosition();
    Window::draw("player", pos.x, pos.y, 0);

    Window::setDrawSettings();
}

Circle Player::getCollider() const
{
    m_collider.center = m_body.getPosition() + Vec2(0, 1);
    return m_collider;
}

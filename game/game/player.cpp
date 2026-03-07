#include "player.h"

#include "core/system.h"
#include "core/window.h"
#include "core/controls.h"

#include "constants.h"

void Player::init()
{
    body = PhysicsBody(32, 32);
    collider.radius = player_radius;
}

void Player::update()
{
    Vec2 dir = Vec2(Controls::get("horizontal").value(), Controls::get("vertical").value());

    body.move(dir * player_speed);
}

void Player::draw()
{
    DrawSettings settings;
    settings.setTiled(0, 5, 1);
    Window::setDrawSettings(settings);

    Vec2 pos = body.getInterpolatedPosition();
    Window::draw("player", pos.x, pos.y, 0);

    Window::setDrawSettings();
}

Circle Player::getCollider() const
{
    collider.center = body.getPosition();
    return collider;
}

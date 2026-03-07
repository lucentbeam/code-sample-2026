#include "player.h"

#include "core/system.h"
#include "core/window.h"
#include "core/controls.h"

#include "constants.h"

namespace {
    enum AnimationStates : int {
        Idle = 0,
        Open = 1,
        Bite = 2,
    };
}
void Player::init()
{
    m_body = PhysicsBody(arena_cx, arena_cy);
    m_collider.radius = player_radius;

    m_sprite.bind(Idle, Animation());
    m_sprite.bind(Open, Animation({1}, 0.05, false));
    m_sprite.bind(Bite, Animation({1,2,3,0}, 0.23, false), 0);
}

void Player::update()
{
    Vec2 dir = Vec2(Controls::get("horizontal").value(), Controls::get("vertical").value());

    if (Controls::get("bite").held()) {
        m_sprite.go(Open);
    } else if (Controls::get("bite").released() && m_sprite.currentState() == Open) {
        m_sprite.go(Bite);
    }
    m_body.move(dir * player_speed);
}

void Player::draw()
{
    DrawSettings settings;
    settings.setTiled(m_sprite.frame(), 4, 1);
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

std::vector<Rect> Player::getHitBoxes()
{
    std::vector<Rect> result;
    if (m_sprite.currentState() == Bite && m_sprite.fraction() > 0.4) {
        const Vec2 center = m_body.getPosition();
        constexpr Vec2 offsets[] = {Vec2(0, -1), Vec2(1, 0), Vec2(0, 1), Vec2(-1, 0)};
        constexpr Vec2 bite_half_size = Vec2(0.5,0.5) * player_bite_size;
        constexpr double distance = player_bite_size / 2;
        for(int i = 0; i < 4; ++i) {
            result.push_back(Rect{center + offsets[i]*distance - bite_half_size, center + offsets[i]*player_radius + bite_half_size});
        }
    }
    return result;
}

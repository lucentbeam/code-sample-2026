#include "player.h"

#include "core/system.h"
#include "core/window.h"
#include "core/controls.h"

#include "game/collisionmanager.h"

#include "constants.h"

namespace {
    enum AnimationStates : int {
        Idle = 0,
        Open = 1,
        Bite = 2,
    };
}
void Player::initialize()
{
    m_body = PhysicsBody(arena_cx, arena_cy);

    m_sprite.bind(Idle, Animation());
    m_sprite.bind(Open, Animation({1}, 0.05, false));
    m_sprite.bind(Bite, Animation({1,2,3,0}, 0.23, false), 0);
}

void Player::update()
{
    Vec2 dir = Vec2(Controls::get("horizontal").value(), Controls::get("vertical").value());
    m_body.move(dir.normalized() * player_speed);

    if (Controls::get("bite").held()) {
        m_sprite.go(Open);
    } else if (Controls::get("bite").released() && m_sprite.currentState() == Open) {
        m_sprite.go(Bite);
    }

    CollisionManager::addPost(Circle(m_body.getPosition(), player_radius), m_body);

    bool lifted = m_sprite.currentState() == Bite || m_sprite.currentState() == Open;
    bool chomping = m_sprite.currentState() == Bite && m_sprite.fraction() > 0.4;

    const Vec2 center = m_body.getPosition();
    constexpr Vec2 offsets[] = {Vec2(0, -1), Vec2(1, 0), Vec2(0, 1), Vec2(-1, 0)};

    Circle collider(player_bite_radius);
    for(int i = 0; i < 4; ++i) {
        collider.center = center + offsets[i] * player_bite_distance;
        if (chomping) {
            CollisionManager::addHurtbox(HurtboxType::PlayerChomp, collider);
        } else if (!lifted) {
            CollisionManager::addPost(collider, m_body);
        }
    }
}

void Player::resolveCollisions()
{
    Circle previous(m_body.getPrevious(), player_collision_radius);
    Circle current(m_body.getPosition(), player_collision_radius);


//    CollisionManager::forPostCollisions(current, [&](CollisionInfo info){
//        m_body.setPosition(info.contact_point + info.normal * player_collision_radius);
//    });
    CollisionManager::forWallCollisions(previous, current, [&](CollisionInfo info){
        m_body.setPosition(info.contact_point + info.normal * player_collision_radius);
    });
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

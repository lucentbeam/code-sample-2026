#include "player.h"

#include "core/system.h"
#include "core/window.h"
#include "core/controls.h"
#include "core/audio.h"

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

    m_collider = Circle(player_radius);

    for(int i = 0; i < 4; ++i) {
        m_mouth_colliders[i] = Circle(player_bite_radius);
    }
}

void Player::update()
{
    Vec2 dir = Vec2(Controls::get("horizontal").value(), Controls::get("vertical").value());
    m_body.move(dir.normalized() * player_speed);

    m_collider.center = m_body.getPosition();

    if (Controls::get("action").held()) {
        m_sprite.go(Open);
    } else if (Controls::get("action").released() && m_sprite.currentState() == Open) {
        m_sprite.go(Bite);
        Audio::playSFX("chomp");
    }

    CollisionManager::addDynamic({CollisionType::PlayerObject, &m_collider, &m_body});

    bool lifted = m_sprite.currentState() == Bite || m_sprite.currentState() == Open;
    bool chomping = m_sprite.currentState() == Bite && m_sprite.fraction() > 0.4;

    const Vec2 center = m_body.getPosition();
    constexpr Vec2 offsets[] = {Vec2(0, -1), Vec2(1, 0), Vec2(0, 1), Vec2(-1, 0)};

    for(int i = 0; i < 4; ++i) {
        m_mouth_colliders[i].center = center + offsets[i] * player_bite_distance;
        CollisionObject obj = {CollisionType::PlayerChomp, m_mouth_colliders + i, &m_body};
        if (chomping) {
            CollisionManager::addDynamic(obj);
        } else if (!lifted) {
            obj.type = CollisionType::PlayerObject;
            CollisionManager::addDynamic(obj);
        }
    }
}

void Player::resolveCollisions()
{
    Circle current(m_body.getPosition(), player_collision_radius);
    CollisionObject obj = {CollisionType::PlayerObject, &current, &m_body};

    int collisionMask = CollisionType::LevelObject;

    CollisionManager::forStaticCollisions(obj, collisionMask, [&](const CollisionObject&, CollisionInfo info){
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

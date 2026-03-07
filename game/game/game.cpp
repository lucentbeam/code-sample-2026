#include "game.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

#include "game/player.h"
#include "game/marblepool.h"

#include "game/collisiondetection.h"

#include "constants.h"

namespace {
    Player player;

    MarblePool marbles;

    std::vector<Line> walls;

    bool init = false;
}

void Game::update(FSM &fsm)
{
    if (!init) {
        player.init();

        walls.push_back(Line({10,10},     {230,10}));
        walls.push_back(Line({10,170},   {10,10}));
        walls.push_back(Line({230,10},   {230,170}));
        walls.push_back(Line({230,170}, {10,170}));

        init = true;

        for(int i = 0; i < 1500; ++i) {
            MarblePool::Handle h = marbles.create(40 + (rand() % 160), 40 + (rand() % 100));
            marbles.get(h, [](Marble &m){
                const double speed = 5.0;
                double vx = rand() % 100;
                vx = ((vx / 100) - 0.5) * marble_speed * speed;
                double vy = rand() % 100;
                vy = ((vy / 100) - 0.5) * marble_speed * speed;
                m.body.move(Vec2(vx, vy));
            });
        }
    }
    if (Controls::get("action").pressed()) {
        fsm.go(GameoverScreen);
        return;
    }

    player.update();

    std::vector<Circle> circles;
    circles.push_back(player.getCollider());
    marbles.update(walls, circles);
}

void Game::draw(FSM &)
{
    Window::print("gameplay", 0, 0);

    marbles.draw();
    player.draw();
}

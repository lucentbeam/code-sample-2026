#include "manager.h"

#include "core/controls.h"

#include "title/title.h"
#include "game/game.h"
#include "gameover/gameover.h"

#include "states.h"

namespace {
    FSM fsm = FSM({
        {TitleScreen,       new Title()},
        {GameScreen,        new Game()},
        {GameoverScreen,    new GameOver()}
    });
}

void Manager::initialize()
{
    Controls::bindButton("action", {"kb_return", "pad_south"});
    Controls::bindAxis("horizontal", -1, 1, {"a", "kb_left", "pad_lstick_l", "pad_dpad_l"}, {"d", "kb_right", "pad_lstick_r", "pad_dpad_r"});
    Controls::bindAxis("vertical", -1, 1, {"w", "kb_up"}, {"s", "kb_down"});

    fsm.go(TitleScreen);
}

void Manager::update()
{
    fsm.update();
}

void Manager::draw()
{
    fsm.draw();
}

#include "gameover.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"
#include "constants.h"

void GameOver::update(FSM &fsm)
{
    if (Controls::get("action").pressed()) {
        fsm.go(TitleScreen);
        return;
    }
}

void GameOver::draw(FSM &fsm)
{
    fsm.get(GameScreen)->draw(fsm);
    Window::print("GAMEOVER", resolution_x/2 - 60, resolution_y/2 - 12, 2);
}

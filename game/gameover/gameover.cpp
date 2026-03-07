#include "gameover.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

void GameOver::update(FSM &fsm)
{
    if (Controls::get("action").pressed()) {
        fsm.go(TitleScreen);
        return;
    }
}

void GameOver::draw(FSM &)
{
    Window::print("gameover", 0, 0);
}

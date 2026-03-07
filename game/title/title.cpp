#include "title.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

void Title::update(FSM &fsm)
{
    if (Controls::get("action").pressed()) {
        fsm.go(GameScreen);
        return;
    }
}

void Title::draw(FSM &)
{
    Window::print("Hippos of Significant Hunger", 10, 10);
}

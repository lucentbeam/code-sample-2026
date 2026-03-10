// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "gamestate.h"

#include "core/audio.h"

#include <chrono>
namespace {
}

GameState::Data GameState::s_data;

void GameState::reset()
{
    s_data = Data();
}

void GameState::score(int pts)
{
    s_data.score += pts;

    static std::chrono::steady_clock::time_point last;
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() - last).count() > 0.25) {
        Audio::playSFX("collect");
        last = std::chrono::steady_clock::now();
    }

}

void GameState::damage(int dmg)
{
    s_data.health -= dmg;
    Audio::playSFX("hurt");
}

bool GameState::gameover()
{
    return s_data.health <= 0;
}

int GameState::score()
{
    return s_data.score;
}

int GameState::health()
{
    return s_data.health;
}

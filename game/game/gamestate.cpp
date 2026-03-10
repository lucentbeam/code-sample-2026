#include "gamestate.h"

GameState::Data GameState::s_data;

void GameState::reset()
{
    s_data = Data();
}

void GameState::score(int pts)
{
    s_data.score += pts;
}

void GameState::damage(int dmg)
{
    s_data.health -= dmg;
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

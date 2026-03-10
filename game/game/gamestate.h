#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
    struct Data {
        int score = 0;
        int health = 5;
    } static s_data;

public:
    static void reset();

    static void score(int pts);

    static void damage(int dmg);

    static bool gameover();

    static int score();

    static int health();
};

#endif // GAMESTATE_H

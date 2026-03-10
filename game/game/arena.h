#ifndef ARENA_H
#define ARENA_H

#include <vector>

#include "game/launcher.h"
#include "game/marblepool.h"
#include "launcher.h"

#include "utils/shapes.h"

class Arena
{
    MarblePool marbles;

    std::vector<Launcher> m_launchers;

    std::vector<Circle> m_circles;
    std::vector<size_t> m_drains;
    std::vector<size_t> m_posts;

    std::vector<Line> m_walls;

    void addLauncher(Vec2 at, Vec2 facing);

    void addDrain(Vec2 at);

    void addPost(Vec2 at);

    void startWall(Vec2 at);
    void drawWallTo(Vec2 to);
    void closeWall();

public:
    void initialize();

    void update();

    void resolveCollisions();

    void draw();
};

#endif // ARENA_H

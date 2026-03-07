add_executable(game
	main.cpp
        manager.h manager.cpp
        states.h
        constants.h

        title/title.h title/title.cpp

        game/game.h game/game.cpp
        game/marblepool.h game/marblepool.cpp
        game/wall.h game/wall.cpp
        game/marble.h game/marble.cpp
        game/physicsbody.h game/physicsbody.cpp
        game/player.h game/player.cpp
        game/collisiondetection.h game/collisiondetection.cpp
        game/launcher.h game/launcher.cpp

        gameover/gameover.h gameover/gameover.cpp

        utils/fsm.h utils/fsm.cpp
        utils/animation.h utils/animation.cpp
        utils/clock.h utils/clock.cpp
	)

target_link_libraries(game PRIVATE core)

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/game/
)

add_executable(game
	main.cpp
        manager.h manager.cpp
        states.h
        constants.h

        title/title.h title/title.cpp

        game/game.h game/game.cpp
        game/gamestate.h game/gamestate.cpp
        game/arena.h game/arena.cpp
        game/marblepool.h game/marblepool.cpp
        game/physicsbody.h game/physicsbody.cpp
        game/player.h game/player.cpp
        game/collisiondetection.h game/collisiondetection.cpp
        game/launcher.h game/launcher.cpp
        game/collisionmanager.h game/collisionmanager.cpp

        gameover/gameover.h gameover/gameover.cpp

        utils/fsm.h utils/fsm.cpp
        utils/animation.h utils/animation.cpp
        utils/clock.h utils/clock.cpp
        utils/shapes.h utils/shapes.cpp
        utils/grid.h utils/grid.cpp
	)

target_link_libraries(game PRIVATE core)

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/game/
)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/game/resources"
    $<TARGET_FILE_DIR:game>/resources
)

if(WIN32)
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL_PATH}/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:game>
    )
endif()

set(INCROOT ${CMAKE_SOURCE_DIR}/include/core)

add_library(core
    ${INCROOT}/system.h system.cpp
    ${INCROOT}/window.h window.cpp
    ${INCROOT}/audio.h audio.cpp
    ${INCROOT}/controls.h controls.cpp
    ${INCROOT}/utility.h utility.cpp

    utilities/shaderhelper.h utilities/shaderhelper.cpp
    utilities/texturehelper.h utilities/texturehelper.cpp
    utilities/wavheaderdata.h utilities/wavheaderdata.cpp
    utilities/platformcontrols.h utilities/platformcontrols_sdl.cpp

    utilities/font.h utilities/font.cpp utilities/magofont3.cpp

    shaders/texture.frag shaders/texture.vert

    ${CMAKE_SOURCE_DIR}/external/glad/glad.c
)


if(WIN32)
    target_link_libraries(core "${SDL_PATH}/lib/x64/SDL2.lib")
    target_include_directories(core PRIVATE "${SDL_PATH}/include")
else()
    target_link_libraries(core SDL2)
endif()

target_include_directories(core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(core PRIVATE
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/glad
    ${CMAKE_SOURCE_DIR}/external/KHR
)

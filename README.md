# code-sample-2026
## A lightweight 2D C++ engine and sample game built with SDL2 and OpenGL.
This repository serves as a showcase for my C++ coding style and capabilities.

# Project Goals
This project encompasses both a game engine utilizing SDL2 and OpenGL, and a small game using the same engine as a library.

## Lightweight Engine
This is not meant to be a full-featured engine. Rather, it is meant to be a simple interface that is short and readable. Goals are as follows:
- have a concise interface
- provide all basic game components:
  - system loop
  - rendering (png files)
  - audio (wav files)
  - inputs (keyboard and gamepad)
- "hide" platform details (e.g., SDL versus Nintendo, etc.) in implementations

## Sample Game - Hippos of Significant Hunger
This is a simple game built with the engine, above. Gameplay is simple - eat as many marbles as you can before too many fall into a deep pool. Some interesting code facets are as follows:
- A basic finite state machine interface (utils/fsm.h)
- A space-partitioning grid to help with broad-phase collision detection (utils/grid.h)
- An object pool wrapper for creating & destroying the marbles (game/marblepool.h)

# Building
## Build Requirements
Build requirements are C++11, SDL2, and OpenGL ES 3.0+

## Build System
CMake 3.5 or higher. To build on windows, edit the root directory CMakeLists.txt for SDL_PATH variable: `set(SDL_PATH "C:/SDL/")`
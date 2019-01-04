#include <iostream>
#include "GameEngine.hpp"

const int WIDTH = 960;
const int HEIGHT = 680;

const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

const char* WINDOW_TITLE = "Project Digger";

int main() {

	GameEngine game(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT);

    uint32_t frameStart;
    uint32_t frameTime;

    while ( game.isRunning() ) {

        frameStart = SDL_GetTicks();

        game.handleEvents();
        game.update();
        game.draw();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);

    }

    return 0;
}

#include <iostream>
#include "GameEngine.hpp"

const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

int main() {

    uint32_t frameStart;
    uint32_t frameTime;

    while ( GameEngine::i()->isRunning() ) {

        frameStart = SDL_GetTicks();

		GameEngine::i()->handleEvents();
		GameEngine::i()->update();
		GameEngine::i()->draw();
		GameEngine::i()->drawGUI();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);

    }

	GameEngine::release();

    return 0;
}

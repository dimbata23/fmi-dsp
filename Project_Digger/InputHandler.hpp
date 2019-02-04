#pragma once
#ifdef _MSC_VER
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

enum MouseButtons {
	MB_LEFT = 1,
	MB_RIGHT,
	MB_MIDDLE
};

class InputHandler {

public:

	InputHandler() = delete;
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;
	~InputHandler() = delete;

	// Returns wheter or not a key is being pressed
	static bool keyDown(const SDL_Scancode& sc);

	// Returns wheter or not a mouse button is being pressed
	static bool mouseButton(const MouseButtons& mb);

	// Updates the current keyboard state
	static void update();

private:

	static const Uint8* keyboardStates;

};

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

	static bool keyDown(const SDL_Scancode& sc);
	static bool mouseButton(const MouseButtons& mb);
	static void update();

private:

	static const Uint8* keyboardStates;

};

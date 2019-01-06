#include "InputHandler.hpp"

const Uint8* InputHandler::keyboardStates = nullptr;

bool InputHandler::keyDown(const SDL_Scancode& sc) {
	return keyboardStates[sc];
}

bool InputHandler::mouseButton(const MouseButtons& mb) {
	return false;
}

void InputHandler::update() {
	keyboardStates = SDL_GetKeyboardState(nullptr);
}

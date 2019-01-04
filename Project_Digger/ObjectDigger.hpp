#pragma once

#include "Object.hpp"

enum Direction {
	D_RIGHT,
	D_DOWN,
	D_LEFT,
	D_UP
};

class ObjectDigger : public Object {

public:

	ObjectDigger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);
	virtual ~ObjectDigger();

	virtual void handleEvents(const SDL_Event& event);
	virtual void update();
	virtual void draw();

private:

	Direction dir;

};

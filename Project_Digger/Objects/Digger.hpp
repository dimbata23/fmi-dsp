#pragma once

#include "Object.hpp"

enum Direction {
	D_NONE = -1,
	D_RIGHT,
	D_DOWN,
	D_LEFT,
	D_UP
};

class Digger : public Object {

public:

	Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);
	virtual ~Digger();

	virtual void update();
	virtual void draw();

private:

	Direction dir;

};

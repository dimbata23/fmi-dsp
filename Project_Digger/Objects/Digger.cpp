#include "Digger.hpp"
#include "../InputHandler.hpp"

const int IMAGE_SIZE = 64;
const int SPEED = 1;

Digger::Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) :
	Object(x, y, IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE/2, IMAGE_SIZE/2, texture, renderer, DIGGER),
	dir(D_RIGHT)
{}


Digger::~Digger() {}


void Digger::update() {
	Object::update();

	if (InputHandler::keyDown(SDL_SCANCODE_UP)) {
		y -= SPEED;
		dir = D_UP;
	}
	if (InputHandler::keyDown(SDL_SCANCODE_DOWN)) {
		y += SPEED;
		dir = D_DOWN;
	}
	if (InputHandler::keyDown(SDL_SCANCODE_LEFT)) {
		x -= SPEED;
		dir = D_LEFT;
	}
	if (InputHandler::keyDown(SDL_SCANCODE_RIGHT)) {
		x += SPEED;
		dir = D_RIGHT;
	}

}


void Digger::draw() {
	SDL_RenderCopyEx(renderer, sprite, &srcRect, &destRect, dir * 90.0, &origin, SDL_FLIP_NONE);
}

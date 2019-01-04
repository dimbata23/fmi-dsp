#include "ObjectDigger.hpp"

const int IMAGE_SIZE = 64;
const int SPEED = 5;

ObjectDigger::ObjectDigger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) :
	Object(x, y, IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE/2, IMAGE_SIZE/2, texture, renderer, DIGGER),
	dir(D_RIGHT)
{}


ObjectDigger::~ObjectDigger() {}


void ObjectDigger::handleEvents(const SDL_Event& event) {

	switch (event.type) {

	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			dir = D_LEFT;
			x -= SPEED;
			break;
		case SDLK_UP:
			dir = D_UP;
			y -= SPEED;
			break;
		case SDLK_RIGHT:
			dir = D_RIGHT;
			x += SPEED;
			break;
		case SDLK_DOWN:
			dir = D_DOWN;
			y += SPEED;
			break;
		}
		
		break;

	}

}


void ObjectDigger::update() {
	Object::update();
}


void ObjectDigger::draw() {
	SDL_RenderCopyEx(renderer, sprite, &srcRect, &destRect, dir * 90, &origin, SDL_FLIP_NONE);
}

#include "Digger.hpp"
#include "Dirt.hpp"
#include "../GameEngine.hpp"
#include "../InputHandler.hpp"

const int IMAGE_SIZE = 64;
const int SPEED = 2;

Digger::Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) :
	Object(x, y, IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE/2, IMAGE_SIZE/2, texture, renderer, DIGGER),
	dir(D_RIGHT)
{}


Digger::~Digger() {}


void Digger::update() {
	Object::update();

	Direction moved = D_NONE;

	if (InputHandler::keyDown(SDL_SCANCODE_UP)) {
		if (x % GRID_SIZE == 0) {
			if ((y - GRID_START) % GRID_SIZE == 0) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(TOP_SIDE, true);
				if (y > GRID_START)
					GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE - 1, x / GRID_SIZE)->setPassable(BOTTOM_SIDE, true);
			}
			y -= SPEED;
			dir = D_UP;
		} else {
			if (dir == D_LEFT)
				x -= SPEED;
			else if (dir == D_RIGHT)
				x += SPEED;
		}
		moved = dir;
	}
	else if (InputHandler::keyDown(SDL_SCANCODE_DOWN)) {
		if (x % GRID_SIZE == 0) {
			if ((y - GRID_START) % GRID_SIZE == 0) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(BOTTOM_SIDE, true);
				if (y - GRID_START < (GRID_ROWS-1) * GRID_SIZE)
					GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE + 1, x / GRID_SIZE)->setPassable(TOP_SIDE, true);
			}
			y += SPEED;
			dir = D_DOWN;
		} else {
			if (dir == D_LEFT)
				x -= SPEED;
			else if (dir == D_RIGHT)
				x += SPEED;
		}
		moved = dir;
	}
	else if (InputHandler::keyDown(SDL_SCANCODE_LEFT)) {
		if ((y - GRID_START) % GRID_SIZE == 0) {
			if (x % GRID_SIZE == 0) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(LEFT_SIDE, true);
				if (x > 0)
					GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE - 1)->setPassable(RIGHT_SIDE, true);
			}
			x -= SPEED;
			dir = D_LEFT;
		} else {
			if (dir == D_UP)
				y -= SPEED;
			else if (dir == D_DOWN)
				y += SPEED;
		}
		moved = dir;
	}
	else if (InputHandler::keyDown(SDL_SCANCODE_RIGHT)) {
		if ((y - GRID_START) % GRID_SIZE == 0) {
			if (x % GRID_SIZE == 0) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(RIGHT_SIDE, true);
				if (x < (GRID_ROWS - 1) * GRID_SIZE)
					GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE + 1)->setPassable(LEFT_SIDE, true);
			}
			x += SPEED;
			dir = D_RIGHT;
		}
		else {
			if (dir == D_UP)
				y -= SPEED;
			else if (dir == D_DOWN)
				y += SPEED;
		}
		moved = dir;
	}

	if (y < GRID_START)
		y = GRID_START;
	if (y > GRID_START + (GRID_ROWS * GRID_SIZE) - IMAGE_SIZE)
		y = GRID_START + (GRID_ROWS * GRID_SIZE) - IMAGE_SIZE;
	if (x < 0)
		x = 0;
	if (x > GRID_COLS * GRID_SIZE - IMAGE_SIZE)
		x = GRID_COLS * GRID_SIZE - IMAGE_SIZE;

	if (moved == D_UP || moved == D_LEFT) {
		Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE);
		SDL_Rect* dSrcRect = &d->getSrcRect();
		SDL_Rect* dDestRect = &d->getDestRect();
		if (moved == D_UP && dSrcRect->h > (y - GRID_START) % GRID_SIZE) {
			dDestRect->h = dSrcRect->h = (y - GRID_START) % GRID_SIZE;
		}
		if (moved == D_LEFT && dSrcRect->w > x % GRID_SIZE) {
			dDestRect->w = dSrcRect->w = x % GRID_SIZE;
		}
	}
	else // Somerhing after this doesn't work correctly.... :(
	if (moved == D_DOWN || moved == D_RIGHT) {
		if ((y - GRID_START) / GRID_SIZE < GRID_ROWS) {
			Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE + 1, x / GRID_SIZE);
			SDL_Rect* dSrcRect = &d->getSrcRect();
			SDL_Rect* dDestRect = &d->getDestRect();
			if (moved == D_DOWN && dSrcRect->y < y + GRID_SIZE) {
				dSrcRect->y = y % GRID_SIZE;
				dDestRect->y = y + GRID_SIZE;
				dDestRect->h = GRID_SIZE - dSrcRect->y;
			}
		}
		if (x / GRID_SIZE < GRID_COLS) {
			Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE + 1);
			SDL_Rect* dSrcRect = &d->getSrcRect();
			SDL_Rect* dDestRect = &d->getDestRect();
			if (moved == D_RIGHT && dSrcRect->x < x + GRID_SIZE) {
				dSrcRect->x = x % GRID_SIZE;
				dDestRect->x = x + GRID_SIZE;
				dDestRect->w = GRID_SIZE - dSrcRect->x;
			}
		}
	}

}


void Digger::draw() {
	SDL_RenderCopyEx(renderer, sprite, &srcRect, &destRect, dir * 90.0, &origin, SDL_FLIP_NONE);
}

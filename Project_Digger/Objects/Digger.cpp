#include "Digger.hpp"
#include "Dirt.hpp"
#include "Bag.hpp"
#include "../GameEngine.hpp"
#include "../InputHandler.hpp"

const int	SPEED		= 2;
const float PUSH_SPEED	= 1.5;

const int EMERALD_SCORE	= 25;
const int GOLD_SCORE	= 500;
const int DEFAULT_LIVES = 2;
const int BONUS_LIFE_ON = 20'000;


Digger::Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) :
	Object(x, y, GRID_SIZE / 2, GRID_SIZE / 2, texture, renderer, DIGGER),
	scoreStr("00000"),
	dir(D_RIGHT),
	score(0),
	lives(DEFAULT_LIVES),
	startingX(x),
	startingY(y),
	realX(x),
	canFire(true)
{}


void Digger::update() {

	Bag* bagOnTop = dynamic_cast<Bag*>(GameEngine::i()->getAtPosition(BAG, x + (GRID_SIZE / 2), y - 1));

	Direction moved = movement();

	if (bagOnTop && !bagOnTop->isFalling()) {
		if (moved == D_UP)
			y += SPEED;
		else
			bagOnTop->triggerFall();
	}

	// Fix for moving with uneven speed
	if (moved != D_LEFT && moved != D_RIGHT) {
		realX = round(realX);
		x = realX;
		if (x % 2 != 0)
			++realX;
	}

	if (!bagOnTop) {
		Bag* bag = nullptr;
		switch (moved) {
			case D_LEFT:
				bag = dynamic_cast<Bag*>(GameEngine::i()->getAtPosition(BAG, x, y));
				if (bag) {
					if (bag->canMove(D_LEFT)) {
						bag->move(D_LEFT);
						realX += SPEED - PUSH_SPEED;
					} else {
						realX += SPEED;
					}
				}
			break;
			case D_RIGHT:
				bag = dynamic_cast<Bag*>(GameEngine::i()->getAtPosition(BAG, x + GRID_SIZE - 1, y));
				if (bag) {
					if (bag->canMove(D_RIGHT)) {
						bag->move(D_RIGHT);
						realX -= SPEED - PUSH_SPEED;
					} else {
						realX -= SPEED;
					}
				}
			break;
			case D_DOWN:
				bag = dynamic_cast<Bag*>(GameEngine::i()->getAtPosition(BAG, x, y + GRID_SIZE - 1));
				if (bag && !bag->isFalling())
					y -= SPEED;
			break;
			default:
			break;
		}
	}

	x = round(realX);

	if (x % 2 != 0)
		++x;

	Emerald* em = GameEngine::i()->getEmeraldAt((y + (GRID_SIZE / 2) - GRID_START) / GRID_SIZE, (x + (GRID_SIZE / 2)) / GRID_SIZE);
	if (em) {
		GameEngine::i()->destroyObject(em);
		increaseScore(EMERALD_SCORE);
	}

	Gold* gold = GameEngine::i()->getGoldAt((y + (GRID_SIZE / 2) - GRID_START) / GRID_SIZE, (x + (GRID_SIZE / 2)) / GRID_SIZE);
	if (gold) {
		GameEngine::i()->destroyObject(gold);
		increaseScore(GOLD_SCORE);
		canFire = true;
	}

	Object::update();

}


void Digger::draw() {
	SDL_RenderCopyEx(renderer, sprite, &srcRect, &destRect, dir * 90.0, &origin, SDL_FLIP_NONE);
}


void Digger::increaseScore(size_t points) {
	size_t oldScore = score;
	score += points;
	scoreStr = std::to_string(score);
	scoreStr.insert(scoreStr.begin(), 5 - scoreStr.length(), '0');

	if (oldScore % BONUS_LIFE_ON > score % BONUS_LIFE_ON)
		++lives;
}


void Digger::kill() {
	SDL_Delay(1000);

	if (lives == 0) {
		GameEngine::i()->end();
	} else {
		--lives;
		GameEngine::i()->destroyEnemies();
		realX = startingX;
		x = startingX;
		y = startingY;
	}
}


Direction Digger::movement() {

	Direction moved = D_NONE;

	if (InputHandler::keyDown(SDL_SCANCODE_UP)) {
		if (x % GRID_SIZE == 0) {
			if ((y - GRID_START) % GRID_SIZE == 0 && y > GRID_START) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(TOP_SIDE, true);
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE - 1, x / GRID_SIZE)->setPassable(BOTTOM_SIDE, true);
			}
			y -= SPEED;
			dir = D_UP;
		} else {
			if (dir == D_LEFT)
				realX -= SPEED;
			else if (dir == D_RIGHT)
				realX += SPEED;
		}
		moved = dir;
	}
	else if (InputHandler::keyDown(SDL_SCANCODE_DOWN)) {
		if (x % GRID_SIZE == 0) {
			if ((y - GRID_START) % GRID_SIZE == 0 && y - GRID_START < (GRID_ROWS - 1) * GRID_SIZE) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(BOTTOM_SIDE, true);
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE + 1, x / GRID_SIZE)->setPassable(TOP_SIDE, true);
			}
			y += SPEED;
			dir = D_DOWN;
		} else {
			if (dir == D_LEFT)
				realX -= SPEED;
			else if (dir == D_RIGHT)
				realX += SPEED;
		}
		moved = dir;
	}
	else if (InputHandler::keyDown(SDL_SCANCODE_LEFT)) {
		if ((y - GRID_START) % GRID_SIZE == 0) {
			if (x % GRID_SIZE == 0 && x > 0) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(LEFT_SIDE, true);
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE - 1)->setPassable(RIGHT_SIDE, true);
			}
			realX -= SPEED;
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
			if (x % GRID_SIZE == 0 && x < (GRID_COLS - 1) * GRID_SIZE) {
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE)->setPassable(RIGHT_SIDE, true);
				GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE + 1)->setPassable(LEFT_SIDE, true);
			}
			realX += SPEED;
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
	if (y > GRID_START + (GRID_ROWS * GRID_SIZE) - GRID_SIZE)
		y = GRID_START + (GRID_ROWS * GRID_SIZE) - GRID_SIZE;
	if (realX < 0)
		realX = 0;
	if (realX > GRID_COLS * GRID_SIZE - GRID_SIZE)
		realX = GRID_COLS * GRID_SIZE - GRID_SIZE;

	x = round(realX);
	if (x % 2 != 0)
		++x;


	if (moved == D_UP || moved == D_LEFT) {
		Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE);
		if (!d->isEmpty()) {
			SDL_Rect* dSrcRect = &d->getSrcRect();
			SDL_Rect* dDestRect = &d->getDestRect();
			if (moved == D_UP && dDestRect->y + dDestRect->h > y) {
				dDestRect->h -= SPEED;
				dSrcRect->h -= SPEED;
				if (dDestRect->h <= SPEED)
					d->setEmpty();
			}
			if (moved == D_LEFT && dDestRect-> x + dSrcRect->w > x) {
				dDestRect->w -= SPEED;
				dSrcRect->w -= SPEED;
				if (dDestRect->w <= SPEED)
					d->setEmpty();
			}
		}
	}
	if (moved == D_DOWN) {
		if ((y - GRID_START) / GRID_SIZE < (GRID_ROWS - 1)) {
			Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE + 1, x / GRID_SIZE);
			if (!d->isEmpty()) {
				SDL_Rect* dSrcRect = &d->getSrcRect();
				SDL_Rect* dDestRect = &d->getDestRect();
				if (dDestRect->y < y + GRID_SIZE) {
					dSrcRect->y = (y - GRID_START) % GRID_SIZE;
					dDestRect->y = y + GRID_SIZE;
					dDestRect->h -= SPEED;
					dSrcRect->h -= SPEED;
					if (dDestRect->h <= SPEED)
						d->setEmpty();
				}
			}
		}
	}
	if (moved == D_RIGHT) {
		if (x / GRID_SIZE < GRID_COLS - 1) {
			Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE + 1);
			if (!d->isEmpty()) {
				SDL_Rect* dSrcRect = &d->getSrcRect();
				SDL_Rect* dDestRect = &d->getDestRect();
				if (dSrcRect->x < x % GRID_SIZE) {
					dSrcRect->x = x % GRID_SIZE;
					dDestRect->x = x + GRID_SIZE;
					dDestRect->w -= SPEED;
					dSrcRect->w -= SPEED;
					if (dDestRect->w <= SPEED)
						d->setEmpty();
				}
			}
		}
	}

	return moved;

}

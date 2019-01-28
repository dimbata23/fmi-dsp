#include "Fireball.hpp"
#include "../GameEngine.hpp"

const int SPEED = 3;


Fireball::Fireball(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren, const Direction& dir) :
    Object(x, y, GRID_SIZE/2, GRID_SIZE/2, sprite, ren, FIREBALL),
    dir(dir)
{}


void Fireball::update() {

    switch (dir) {
        case D_LEFT:
            x -= SPEED;
            break;
        case D_RIGHT:
            x += SPEED;
            break;
        case D_UP:
            y -= SPEED;
            break;
        case D_DOWN:
            y += SPEED;
            break;
    }

    Dirt* d = dynamic_cast<Dirt*>(GameEngine::i()->getDirtAt((y + origin.y - GRID_START) / GRID_SIZE, (x + origin.x) / GRID_SIZE));
	if (d) {
		if ((!d->isEmpty()) ||
			(dir == D_RIGHT && !d->isPassable(LEFT_SIDE) && x < d->getX()) ||
			(dir == D_LEFT && !d->isPassable(RIGHT_SIDE) && x > d->getX()) ||
			(dir == D_DOWN && !d->isPassable(TOP_SIDE) && y < d->getY()) ||
			(dir == D_UP && !d->isPassable(BOTTOM_SIDE) && y > d->getY())) 
		{
			GameEngine::i()->destroyObject(this);
		}
	}
    Object* b = GameEngine::i()->getAtPosition(BAG, x + origin.x, y + origin.y);
    if (b)
        GameEngine::i()->destroyObject(this);

    Enemy* e = dynamic_cast<Enemy*>(GameEngine::i()->getAtPosition(ENEMY, x + origin.x, y + origin.y));
    if (e) {
        e->kill();
        GameEngine::i()->destroyObject(this);
    }

    Object::update();

    if (x < 0 || x > GRID_COLS * GRID_SIZE || y < 0 || y > GRID_ROWS * GRID_SIZE)
        GameEngine::i()->destroyObject(this);

}


void Fireball::draw() {
	SDL_RenderCopyEx(renderer, sprite, &srcRect, &destRect, dir * 90.0, &origin, SDL_FLIP_NONE);
}

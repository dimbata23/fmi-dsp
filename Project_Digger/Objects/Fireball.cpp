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
    if (d && !d->isEmpty())
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

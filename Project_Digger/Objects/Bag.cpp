#include "Bag.hpp"
#include "../GameEngine.hpp"

const int DEFAULT_WOBBLE_FRAMES = 60;
const int WOBBLE_SIZE = 4;
const int FALL_SPEED = 3;
//const int MOVE_SPEED = GRID_SIZE / ((GRID_SIZE / 2) + 2.0f);
const int MOVE_SPEED = 2;
const int FALL_STRETCH = 16;
const char* GOLD_SPRITE = "Sprites/gold.png";

Bag::Bag(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) :
    Object(x, y, 0, 0, texture, renderer, BAG),
    moveDir(D_NONE),
    time(DEFAULT_WOBBLE_FRAMES),
    startFallY(y),
    excited(false),
    falling(false)
{}


void Bag::update() {

    Object::update();

    if (excited) {
        --time;
        if (time == 0) {
            falling = true;
            excited = false;
            startFallY = y;
            time = DEFAULT_WOBBLE_FRAMES;
        }
    }

    else if (falling) {
        destRect.h = GRID_SIZE + FALL_STRETCH;
        destRect.x = x + FALL_STRETCH / 2;
        destRect.w = GRID_SIZE - FALL_STRETCH;

        y += FALL_SPEED;

        if (y >= GRID_SIZE * GRID_ROWS + GRID_START - GRID_SIZE) {
            falling = false;
        } else {
            Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE + 1, x / GRID_SIZE);
            if (!d->isEmpty()) {
                if (d->emptyPercent() >= 50)
                    d->setEmpty();
                else
                    falling = false;
            } else {
                d->setPassable(TOP_SIDE, true);
                d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE);
                d->setPassable(BOTTOM_SIDE, true);
            }
        }

        if (!falling) {
            if (y - startFallY >= 2*GRID_SIZE)
                releaseCoins();
        }
    }

    else if (moveDir != D_NONE) {

        if (moveDir == D_LEFT) {
            Bag* b = dynamic_cast<Bag*>(GameEngine::i()->getAtPosition(BAG, x - GRID_SIZE, y));
            if (b)
                b->move(moveDir);
            x -= MOVE_SPEED;
        } else if (moveDir == D_RIGHT) {
            Bag* b = dynamic_cast<Bag*>(GameEngine::i()->getAtPosition(BAG, x + GRID_SIZE, y));
            if (b)
                b->move(moveDir);
            x += MOVE_SPEED;
        }
        
        if (x % GRID_SIZE == 0) {
            moveDir = D_NONE;
            Dirt* d = GameEngine::i()->getDirtAt((y - GRID_START) / GRID_SIZE + 1, x / GRID_SIZE);
            if (d->emptyPercent() >= 50) {
                falling = true;
                d->setEmpty();
            }
        }

    }

    if (!falling) {
        if ((y - GRID_START) % GRID_SIZE != 0)
            y -= (y - GRID_START) % GRID_SIZE;
        destRect.x = x;
        destRect.h = GRID_SIZE;
        destRect.w = GRID_SIZE;
    }

}


void Bag::draw() {

    if (excited && SDL_GetTicks() % 2 == 0)
        destRect.x = x + (rand() % (WOBBLE_SIZE*2)) - WOBBLE_SIZE;
    Object::draw();

}


void Bag::releaseCoins() {

    if (!GameEngine::i()->getGoldAt((y - GRID_START) / GRID_SIZE, x / GRID_SIZE))
        GameEngine::i()->createObject(GOLD, x, y, GOLD_SPRITE);
    GameEngine::i()->destroyObject(this);

}

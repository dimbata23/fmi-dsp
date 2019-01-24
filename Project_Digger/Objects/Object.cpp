#include "Object.hpp"


static size_t currId = 1;


Object::Object(int x, int y, int xOrigin, int yOrigin, SDL_Texture* texture, SDL_Renderer* renderer, const ObjectType& type) :
    id(getNextId()),
    type(type),
	x(x),
    y(y),
    sprite(texture),
    renderer(renderer)
{
    srcRect.w = GRID_SIZE;
    srcRect.h = GRID_SIZE;
    srcRect.x = 0;
    srcRect.y = 0;
    destRect.x = x;
    destRect.y = y;
    destRect.w = GRID_SIZE;
    destRect.h = GRID_SIZE;
    origin.x = xOrigin;
    origin.y = yOrigin;
}


Object::~Object() {}


void Object::update() {
    destRect.x = x;
    destRect.y = y;
}


void Object::draw() {

    SDL_RenderCopy(renderer, sprite, &srcRect, &destRect);

}


size_t Object::getNextId() const { return currId++; }

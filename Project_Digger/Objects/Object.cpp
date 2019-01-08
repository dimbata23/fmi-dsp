#include "Object.hpp"


static size_t currId = 1;


Object::Object(int x, int y, int width, int height, int xOrigin, int yOrigin, SDL_Texture* texture, SDL_Renderer* renderer, const ObjectType& type) :
    id(getNextId()),
    type(type),
	x(x),
    y(y),
    sprite(texture),
    renderer(renderer)
{
    srcRect.w = width;
    srcRect.h = height;
    srcRect.x = 0;
    srcRect.y = 0;
    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;
    origin.x = xOrigin;
    origin.y = yOrigin;
}


Object::~Object() {

    //SDL_DestroyTexture(sprite);

}


void Object::update() {
    destRect.x = x;
    destRect.y = y;
}


void Object::draw() {

    SDL_RenderCopy(renderer, sprite, &srcRect, &destRect);

}


size_t Object::getNextId() const { return currId++; }

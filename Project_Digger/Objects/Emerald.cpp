#include "Emerald.hpp"

const int IMAGE_SIZE = 64;

Emerald::Emerald(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren) :
    Object(x, y, 0, 0, sprite, ren, EMERALD)
{}


void Emerald::update() {}


void Emerald::draw() {
    Object::draw();
}

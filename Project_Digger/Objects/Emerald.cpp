#include "Emerald.hpp"

const int IMAGE_SIZE = 64;

Emerald::Emerald(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren) :
    Object(x, y, IMAGE_SIZE, IMAGE_SIZE, 0, 0, sprite, ren, EMERALD)
{}


Emerald::~Emerald() {}


void Emerald::update() {}


void Emerald::draw() {
    Object::draw();
}

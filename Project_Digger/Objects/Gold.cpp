#include "Gold.hpp"


Gold::Gold(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren) :
    Object(x, y, 0, 0, sprite, ren)
{}


void Gold::update() {}


void Gold::draw() {
    Object::draw();
}

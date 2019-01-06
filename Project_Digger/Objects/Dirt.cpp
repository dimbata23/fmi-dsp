#include "Dirt.hpp"

const int IMAGE_SIZE = 64;

Dirt::Dirt(int x, int y, bool empty, SDL_Texture* sprite, SDL_Texture* wallSprite, SDL_Renderer* ren) :
    Object(x, y, IMAGE_SIZE, IMAGE_SIZE, 0, 0, sprite, ren, empty ? TUNNEL : DIRT),
	wallSprite(wallSprite),
    passable{false, },
    empty(empty)
{
	for (int i = 0; i < 4; ++i) {
		borderRect[i] = srcRect;
		borderRect[i].x += i * IMAGE_SIZE;
	}
}


Dirt::~Dirt() {}


void Dirt::handleEvents(const SDL_Event& event) {}


void Dirt::update() {

    Object::update();

}


void Dirt::draw() {

	SDL_SetTextureAlphaMod(sprite, 90);
	Object::draw();
	SDL_SetTextureAlphaMod(sprite, 255);

	if (!empty)
		Object::draw();

	for (int i = 0; i < 4; ++i)
		if (!passable[i])
			SDL_RenderCopy(renderer, wallSprite, &borderRect[i], &destRect);

}

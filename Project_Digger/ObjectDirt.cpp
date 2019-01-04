#include "ObjectDirt.hpp"

const int IMAGE_SIZE = 64;

ObjectDirt::ObjectDirt(int x, int y, bool empty, SDL_Texture* sprite, SDL_Texture* wallSprite, SDL_Renderer* ren) :
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


ObjectDirt::~ObjectDirt() {}


void ObjectDirt::handleEvents(const SDL_Event& event) {}


void ObjectDirt::update() {

    Object::update();

}


void ObjectDirt::draw() {

	SDL_SetTextureAlphaMod(sprite, 90);
	Object::draw();
	SDL_SetTextureAlphaMod(sprite, 255);

	if (!empty)
		Object::draw();

	for (int i = 0; i < 4; ++i)
		if (!passable[i])
			SDL_RenderCopy(renderer, wallSprite, &borderRect[i], &destRect);

}

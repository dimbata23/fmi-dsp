#include "Dirt.hpp"

const int IMAGE_SIZE = 64;

Dirt::Dirt(int x, int y, bool empty, SDL_Texture* sprite, SDL_Texture* wallSprite, SDL_Renderer* ren) :
    Object(x, y, IMAGE_SIZE, IMAGE_SIZE, 0, 0, sprite, ren, empty ? TUNNEL : DIRT),
	defaultDestRect(destRect),
	defaultSrcRect(srcRect),
	wallSprite(wallSprite),
    passable{false, },
    empty(empty)
{
	for (int i = 0; i < 4; ++i) {
		borderDestRect[i] = destRect;
		borderRect[i] = srcRect;
		borderRect[i].x += i * IMAGE_SIZE;
	}
}


Dirt::~Dirt() {}


void Dirt::update() {

	//Object::update();

	if (!empty) {
		borderDestRect[LEFT_SIDE].x = destRect.x + destRect.w;
		borderDestRect[TOP_SIDE].y = destRect.y + destRect.h;
		borderDestRect[RIGHT_SIDE].x = destRect.x - destRect.w;
		borderDestRect[BOTTOM_SIDE].x = destRect.y - destRect.h;
	}

}


void Dirt::draw() {

	SDL_SetTextureAlphaMod(sprite, 90);
	SDL_RenderCopy(renderer, sprite, &defaultSrcRect, &defaultDestRect);
	SDL_SetTextureAlphaMod(sprite, 255);

	if (!empty)
		Object::draw();

	for (int i = 0; i < 4; ++i)
		if (!passable[i])
			SDL_RenderCopy(renderer, wallSprite, &borderRect[i], &defaultDestRect);

	/*for (int i = 0; i < 4; ++i)
			SDL_RenderCopy(renderer, wallSprite, &borderRect[i], &borderDestRect[i]);*/

}

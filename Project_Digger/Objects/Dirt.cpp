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
		borderSrcRect[i] = srcRect;
		borderSrcRect[i].x += i * IMAGE_SIZE;
		defaultBorderSrcRect[i] = borderSrcRect[i];
	}
}


Dirt::~Dirt() {}


void Dirt::update() {

	if (!empty) {
		// setting up the jaggered border inside the block
		borderDestRect[LEFT_SIDE].x = destRect.x + destRect.w;
		borderDestRect[TOP_SIDE].y = destRect.y + destRect.h;
		borderDestRect[RIGHT_SIDE].x = destRect.x - GRID_SIZE;
		borderDestRect[BOTTOM_SIDE].y = destRect.y - GRID_SIZE;

		borderSrcRect[TOP_SIDE].w = borderSrcRect[BOTTOM_SIDE].w = srcRect.w;
		borderDestRect[TOP_SIDE].w = borderDestRect[BOTTOM_SIDE].w = destRect.w;
		borderSrcRect[TOP_SIDE].x = defaultBorderSrcRect[TOP_SIDE].x + srcRect.x;
		borderSrcRect[BOTTOM_SIDE].x = defaultBorderSrcRect[BOTTOM_SIDE].x + srcRect.x;
		borderDestRect[TOP_SIDE].x = borderDestRect[BOTTOM_SIDE].x = destRect.x;

		borderSrcRect[LEFT_SIDE].h = borderSrcRect[RIGHT_SIDE].h = srcRect.h;
		borderDestRect[LEFT_SIDE].h = borderDestRect[RIGHT_SIDE].h = destRect.h;
		borderSrcRect[LEFT_SIDE].y = defaultBorderSrcRect[LEFT_SIDE].y + srcRect.y;
		borderSrcRect[RIGHT_SIDE].y = defaultBorderSrcRect[RIGHT_SIDE].y + srcRect.y;
		borderDestRect[LEFT_SIDE].y = borderDestRect[RIGHT_SIDE].y = destRect.y;
	}

}


void Dirt::draw() {

	// Background
	SDL_SetTextureAlphaMod(sprite, 90);
	SDL_RenderCopy(renderer, sprite, &defaultSrcRect, &defaultDestRect);
	SDL_SetTextureAlphaMod(sprite, 255);

	if (!empty)
		Object::draw();

	// Drawing (passable) border
	for (int i = 0; i < 4; ++i)
		if (!passable[i])
			SDL_RenderCopy(renderer, wallSprite, &defaultBorderSrcRect[i], &defaultDestRect);

	// Drawing border inside the block
	for (int i = 0; i < 4; ++i)
		if (!empty)
			SDL_RenderCopy(renderer, wallSprite, &borderSrcRect[i], &borderDestRect[i]);

}

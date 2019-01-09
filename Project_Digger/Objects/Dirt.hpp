#pragma once
#include "Object.hpp"

// DEBUG
//#include <iostream>

enum Side {
	LEFT_SIDE,
	TOP_SIDE,
	RIGHT_SIDE,
	BOTTOM_SIDE
};


class Dirt : public Object {

public:

    Dirt(int x, int y, bool empty, SDL_Texture* sprite, SDL_Texture* wallSprite, SDL_Renderer* ren);
    virtual ~Dirt();

    virtual void update() override;
    virtual void draw() override;

    void setPassable(const Side& dir, bool b) { passable[dir] = b; }
    bool isPassable(const Side& dir) const { return passable[dir]; }
	bool isEmpty() const { return empty; }
	void setEmpty() { empty = true; }
	SDL_Rect& getSrcRect() { return srcRect; }
	SDL_Rect& getDestRect() { return destRect; }

	int emptyPercent() const { return empty ? 100 : (100 - (100*(destRect.w * destRect.h))/(GRID_SIZE * GRID_SIZE)); }

	// DEBUG
	//void print() const { std::cout << "\nPercent empty: " << emptyPercent() << std::endl; }

private:

	SDL_Rect borderSrcRect[4];
	SDL_Rect borderDestRect[4];
	SDL_Rect defaultBorderSrcRect[4];
	SDL_Rect defaultDestRect;
	SDL_Rect defaultSrcRect;
    SDL_Texture* wallSprite;
    // Which side the enemies can pass through
    bool passable[4];
    bool empty;

};

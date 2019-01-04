#pragma once
#include "Object.hpp"


enum Side {
	LEFT_SIDE,
	TOP_SIDE,
	RIGHT_SIDE,
	BOTTOM_SIDE
};


class ObjectDirt : public Object {

public:

    ObjectDirt(int x, int y, bool empty, SDL_Texture* sprite, SDL_Texture* wallSprite, SDL_Renderer* ren);
    virtual ~ObjectDirt();

    virtual void handleEvents(const SDL_Event& event);
    virtual void update();
    virtual void draw();

    void setPassable(const Side& dir, bool b) { passable[dir] = b; }

private:

    SDL_Rect borderRect[4];
    SDL_Texture* wallSprite;
    // Which side the enemies can pass through
    bool passable[4];
    bool empty;

};

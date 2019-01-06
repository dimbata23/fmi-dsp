#pragma once

#ifdef _MSC_VER
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

enum ObjectType {
    ANY = -1,
    DIGGER,
    DIRT,
	TUNNEL,
    GEM,
    BAG,
    ENEMY,
    SPAWNER
};

class Object {

public:

    Object(int x, int y, int width, int height, int xOrigin, int yOrigin, SDL_Texture* texture, SDL_Renderer* renderer, const ObjectType& type = ANY);
    virtual ~Object();

    virtual void update();
    virtual void draw();

    const ObjectType& getType() const { return type; }

protected:

    ObjectType type;
    int x;
    int y;
    //float image_xscale;
    //float image_yscale;

    SDL_Texture* sprite;
    SDL_Renderer* renderer;
    SDL_Point origin;
    SDL_Rect srcRect;
    SDL_Rect destRect;

};

#pragma once

#ifdef _MSC_VER
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

extern const int GRID_SIZE;
extern const int GRID_START;

enum ObjectType {
    ANY = -1,
    DIGGER,
    DIRT,
	TUNNEL,
    EMERALD,
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

    ObjectType getType() const { return type; }
    size_t getId() const { return id; }
    int getX() const { return x; }
    int getY() const { return y; }

private:

    size_t getNextId() const;

protected:

    size_t id;
    ObjectType type;
    int x;
    int y;

    SDL_Texture* sprite;
    SDL_Renderer* renderer;
    SDL_Point origin;
    SDL_Rect srcRect;
    SDL_Rect destRect;

};

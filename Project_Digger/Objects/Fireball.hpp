#pragma once

#include "Object.hpp"

class Fireball : public Object {

public:

    Fireball(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren, const Direction& dir);

    virtual void update() override;
    virtual void draw() override;

private:

    const Direction dir;

};

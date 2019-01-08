#pragma once

#include "Object.hpp"

class Emerald : public Object {

public:

    Emerald(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren);
    virtual ~Emerald();

    virtual void update() override;
    virtual void draw() override;

};

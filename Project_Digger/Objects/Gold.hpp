#pragma once

#include "Object.hpp"

class Gold : public Object {

public:
    
    Gold(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren);

    virtual void update() override;
    virtual void draw() override;

};

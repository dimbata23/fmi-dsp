#pragma once

#include "Object.hpp"


class Enemy : public Object {

public:

    Enemy(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);
    virtual ~Enemy();

    virtual void update();
    virtual void draw();

    void findPathBFS();

private:

    float realX;
    float realY;
    int nextPosX;
    int nextPosY;

};

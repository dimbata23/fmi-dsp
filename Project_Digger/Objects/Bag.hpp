#pragma once

#include "Object.hpp"

class Bag : public Object {

public:

    Bag(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);

    virtual void update() override;
    virtual void draw() override;

    void triggerFall() { excited = true; }
    bool isFalling() const { return falling; }

    void move(const Direction& dir) { moveDir = dir; }
    void releaseCoins();

    bool canMove(const Direction dir) const;

private:

    Direction moveDir;
    int time;
    int startFallY;
    float realX;
    bool excited;
    bool falling;

};

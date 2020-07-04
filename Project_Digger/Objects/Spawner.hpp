#pragma once

#include "Object.hpp"

class Spawner : public Object {

public:

    Spawner(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren);

    virtual void update() override;
    virtual void draw() override;

    void setActive() { active = true; }
    void setInactive() { active = false; }

private:

    int lastSpawnTime;
    bool active;

};

#pragma once

#include <string>
#include "Object.hpp"

class Digger : public Object {

public:

    Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);

    virtual void update() override;
    virtual void draw() override;
    void update(bool labirinthMode);

    void increaseScore(size_t points);
    const std::string& getScoreString() const { return scoreStr; }
    size_t getLives() const { return lives; }
    void kill();
    Direction getDirection() const { return dir; }
    void setPosition(int x, int y) { realX = x; this->y = y; }

private:

    Direction movement();
    void labirinthMovement();

private:

    SDL_Texture* canFireTexture;
    std::string scoreStr;
    Direction dir;
    size_t score;
    size_t lives;
    int startingX;
    int startingY;
    int lastFire;
    float realX;
    bool canFire;

};

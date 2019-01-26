#pragma once

#include "Object.hpp"

class Spawner : public Object {

public:

	Spawner(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren);

	virtual void update() override;
	virtual void draw() override;

private:

	size_t lastSpawnTime;

};

#include "Spawner.hpp"
#include "../GameEngine.hpp"

const int SPAWN_RATE = 3000; // in ms


Spawner::Spawner(int x, int y, SDL_Texture* sprite, SDL_Renderer* ren) :
	Object(x, y, 0, 0, sprite, ren, SPAWNER),
	lastSpawnTime(0)
{}


void Spawner::update() {

	size_t currTime = SDL_GetTicks();
	if (currTime - lastSpawnTime >= SPAWN_RATE)
		if (GameEngine::i()->spawnEnemy(x, y))
			lastSpawnTime = currTime;

}


void Spawner::draw() {}

#pragma once

#include <string>
#include "Object.hpp"

class Digger : public Object {

public:

	Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);

	virtual void update() override;
	virtual void draw() override;

	void increaseScore(size_t points);
	const std::string& getScoreString() const { return scoreStr; }
	size_t getLives() const { return lives; }

private:

	Direction movement();

private:

	std::string scoreStr;
	Direction dir;
	size_t score;
	size_t lives;
	bool canFire;

};

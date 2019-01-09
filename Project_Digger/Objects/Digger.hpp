#pragma once

#include <string>
#include "Object.hpp"

class Digger : public Object {

public:

	Digger(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer);
	virtual ~Digger() noexcept;

	virtual void update() override;
	virtual void draw() override;

	void increaseScore(size_t points);
	const std::string& getScoreString() const { return scoreStr; }

private:

	Direction movement();

private:

	std::string scoreStr;
	Direction dir;
	size_t score;

};

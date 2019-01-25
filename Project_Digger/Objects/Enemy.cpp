#include <queue>
#include <unordered_set>
#include <stack>
#include "Enemy.hpp"
#include "../GameEngine.hpp"


// Slightly faster than Digger
const float SPEED = GRID_SIZE / ((GRID_SIZE / 2) - 1.0f);


Enemy::Enemy(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) :
    Object(x, y, 0, 0, texture, renderer, ENEMY),
    realX(x),
    realY(y),
    nextPosX(x),
    nextPosY(y)
{}


void Enemy::update() {
    Object::update();

    if (x % GRID_SIZE == 0 && (y - GRID_START) % GRID_SIZE == 0)
        findPathBFS();

    if (nextPosX < x)
        realX -= SPEED;
    else if (nextPosX > x)
        realX += SPEED;
    else if (nextPosY < y)
        realY -= SPEED;
    else if (nextPosY > y)
        realY += SPEED;

    x = round(realX);
    y = round(realY);

	Gold* collidedGold = GameEngine::i()->getGoldAt((y - GRID_START + (GRID_SIZE / 2)) / GRID_SIZE, (x + (GRID_SIZE / 2)) / GRID_SIZE);
	if (collidedGold)
		GameEngine::i()->destroyObject(collidedGold);

}


void Enemy::draw() {
    Object::draw();

    // DEBUG
    //SDL_Rect test = {nextPosX, nextPosY, 64, 64};
    //SDL_RenderDrawRect(renderer, &test);

}


void Enemy::findPathBFS() {

    int player_center_x = GameEngine::i()->getPlayer()->getX() + GRID_SIZE / 2;
    int player_center_y = GameEngine::i()->getPlayer()->getY() + GRID_SIZE / 2;
    Dirt* goal = GameEngine::i()->getDirtAt((player_center_y - GRID_START) / GRID_SIZE,
                                            player_center_x / GRID_SIZE);

    std::unordered_set<Dirt*> visited;
    std::unordered_map<Dirt*, Dirt*> parentNodes;
    std::queue<Dirt*> wave;

    Dirt* curr = nullptr;

    Dirt* starting = GameEngine::i()->getDirtAt((this->y - GRID_START) / GRID_SIZE, this->x / GRID_SIZE);
    wave.push(starting);

    while (curr != goal && !wave.empty()) {

        curr = wave.front();
        visited.insert(curr);

        // Going through all the neighbouring places
        if (curr->isPassable(RIGHT_SIDE)) {
            Dirt* d = GameEngine::i()->getDirtAt((curr->getY() - GRID_START) / GRID_SIZE, curr->getX() / GRID_SIZE + 1);
            if ((d == goal && d->getDestRect().x > player_center_x) || (d->isEmpty() && visited.find(d) == visited.end())) {
                visited.insert(d);
                wave.push(d);
                parentNodes.insert(std::make_pair(d, curr));
            }
        }
        if (curr->isPassable(BOTTOM_SIDE)) {
            Dirt* d = GameEngine::i()->getDirtAt((curr->getY() - GRID_START) / GRID_SIZE + 1, curr->getX() / GRID_SIZE);
            if ((d == goal && d->getDestRect().y > player_center_y) || (d->isEmpty() && visited.find(d) == visited.end())) {
                visited.insert(d);
                wave.push(d);
                parentNodes.insert(std::make_pair(d, curr));
            }
        }
        if (curr->isPassable(TOP_SIDE)) {
            Dirt* d = GameEngine::i()->getDirtAt((curr->getY() - GRID_START) / GRID_SIZE - 1, curr->getX() / GRID_SIZE);
            if ((d == goal && d->getDestRect().y < player_center_y) || (d->isEmpty() && visited.find(d) == visited.end())) {
                visited.insert(d);
                wave.push(d);
                parentNodes.insert(std::make_pair(d, curr));
            }
        }
        if (curr->isPassable(LEFT_SIDE)) {
            Dirt* d = GameEngine::i()->getDirtAt((curr->getY() - GRID_START) / GRID_SIZE, curr->getX() / GRID_SIZE - 1);
            if ((d == goal && d->getDestRect().x < player_center_x) || (d->isEmpty() && visited.find(d) == visited.end())) {
                visited.insert(d);
                wave.push(d);
                parentNodes.insert(std::make_pair(d, curr));
            }
        }

        wave.pop();

    }

    auto it = parentNodes.find(curr);
    if (it != parentNodes.end()) {
        while (it->second != starting)
            it = parentNodes.find(it->second);

        nextPosX = it->first->getX();
        nextPosY = it->first->getY();
    }

}

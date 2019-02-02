#include <unordered_set>
#include <queue>
#include <cstdlib>
#include <vector>
#include "LabirinthManager.hpp"
#include "GameEngine.hpp"
#include "Utils.hpp"

const char* WALL_SPRITE = "Sprites/dirt2_64x64.png";
const char* WALL_BORDER_SPRITE = "Sprites/dirt_border.png";
const char* PLAYER_SPRITE = "Sprites/digger.png";
const char* FINISH_SPRITE = "Sprites/emerald.png";
const int LABIRINTH_START_X = 0;
const int LABIRINTH_START_Y = GRID_START;

const int SONG_INTRO_LENGTH = 10600;
const int SONG_LENGTH = 32000;
const int CREATION_DELAY = SONG_INTRO_LENGTH / 200;  // Calculated to delay the start of the maze about SONG_INTRO_LENGTH ms

LabirinthManager::LabirinthManager() :
    labirinth{nullptr, },
    graph{0, },
    finishX(-GRID_SIZE),
    finishY(-GRID_SIZE),
    startTime(SDL_GetTicks()),
	playerCanMove(false)
{}


LabirinthManager::~LabirinthManager() {
    clear();
}


void LabirinthManager::clear() {

    for (int i = 0; i < LABIRINTH_HEIGHT; ++i) {
        for (int j = 0; j < LABIRINTH_WIDTH; ++j) {
            delete labirinth[i][j];
            labirinth[i][j] = nullptr;
        }
    }

    GameEngine::i()->destroyObject(player);
    player = nullptr;

    finishX = -GRID_SIZE;
    finishY = -GRID_SIZE;

}


LabirinthMode LabirinthManager::update() {

    player->update(true);
    if (distance(player->getX(), player->getY(), finishX, finishY) <= GRID_SIZE / 2)
        return LAB_WON;
    if (SDL_GetTicks() - startTime > SONG_LENGTH)
        return LAB_LOST;

    return LAB_RUNNING;

}


void LabirinthManager::draw() {

    for (int i = 0; i < LABIRINTH_HEIGHT; ++i)
        for (int j = 0; j < LABIRINTH_WIDTH; ++j)
            labirinth[i][j]->draw();

    GameEngine::i()->drawTexture(finishX, finishY, FINISH_SPRITE);

    player->draw();

}


void LabirinthManager::drawGUI() {

	if (playerCanMove) {
		size_t timeLeft = (SONG_LENGTH - (SDL_GetTicks() - startTime));
		if (timeLeft > 5000)
			TextManager::i()->drawText(std::to_string(timeLeft / 1000).c_str(), GRID_SIZE*GRID_COLS / 2, -4, GameEngine::i()->getRenderer());
		else
			TextManager::i()->drawText(std::to_string(timeLeft / 1000.0).c_str(), GRID_SIZE*GRID_COLS / 2 - GRID_SIZE, -4, GameEngine::i()->getRenderer(), {200, 100, 100});
	}

}


void LabirinthManager::createLabirinth() {

    for (int i = 0; i < LABIRINTH_HEIGHT; ++i)
        for (int j = 0; j < LABIRINTH_WIDTH; ++j)
            labirinth[i][j] = dynamic_cast<Dirt*>(GameEngine::i()->createObject(DIRT, LABIRINTH_START_X + j * GRID_SIZE, LABIRINTH_START_Y + i * GRID_SIZE, WALL_SPRITE, WALL_BORDER_SPRITE));

    for (int i = 0; i < GRAPH_HEIGHT; ++i)
        for (int j = 0; j < GRAPH_WIDTH; ++j)
            graph[i][j] = i*GRAPH_WIDTH + j;

    player = dynamic_cast<Digger*>(GameEngine::i()->createObject(DIGGER, LABIRINTH_START_X, LABIRINTH_START_Y, PLAYER_SPRITE));

    randomizedKruskal();
	createFinish();

	playerCanMove = true;

}


void LabirinthManager::randomizedKruskal() {

    std::srand(SDL_GetTicks());

    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> edges;
    for (int i = 0; i < GRAPH_HEIGHT; ++i) {
        for (int j = 0; j < GRAPH_WIDTH; ++j) {
            if (j > 0)
                edges.push_back(std::make_pair(std::make_pair(i, j), std::make_pair(i, j - 1)));
            if (i > 0)
                edges.push_back(std::make_pair(std::make_pair(i, j), std::make_pair(i - 1, j)));
        }
    }

    AudioManager::i()->playMusic(AudioManager::i()->musicAudio("Sounds/maze.wav"));
    size_t startTime = SDL_GetTicks();
    size_t startDelay;
    size_t endDelay;

    while (!edges.empty()) {
        startDelay = SDL_GetTicks();
        auto it = edges.begin() + (std::rand() % edges.size());
        
        int set1 = graph[it->first.first][it->first.second];
        int set2 = graph[it->second.first][it->second.second];

        if (set1 != set2) {

            if (it->first.first > it->second.first) {
                labirinth[it->first.first][it->first.second]->setPassable(TOP_SIDE, true);
                labirinth[it->second.first][it->second.second]->setPassable(BOTTOM_SIDE, true);
            } else {
                labirinth[it->first.first][it->first.second]->setPassable(LEFT_SIDE, true);
                labirinth[it->second.first][it->second.second]->setPassable(RIGHT_SIDE, true);
            }

            if (labirinth[it->first.first][it->first.second]->isEmpty() && labirinth[it->second.first][it->second.second]->isEmpty()) {
                GameEngine::i()->draw();
                GameEngine::i()->drawGUI();
                endDelay = SDL_GetTicks();
                SDL_Delay(CREATION_DELAY - (endDelay - startDelay));
                startDelay = SDL_GetTicks();
            }

            if (!labirinth[it->first.first][it->first.second]->isEmpty()) {
                labirinth[it->first.first][it->first.second]->setEmpty();
                GameEngine::i()->draw();
                GameEngine::i()->drawGUI();
                endDelay = SDL_GetTicks();
                SDL_Delay(CREATION_DELAY - (endDelay - startDelay));
                startDelay = SDL_GetTicks();
            }

            if (!labirinth[it->second.first][it->second.second]->isEmpty()) {
                labirinth[it->second.first][it->second.second]->setEmpty();
                GameEngine::i()->draw();
                GameEngine::i()->drawGUI();
                endDelay = SDL_GetTicks();
                SDL_Delay(CREATION_DELAY - (endDelay - startDelay));
                startDelay = SDL_GetTicks();
            }

            for (int i = 0; i < GRAPH_HEIGHT; ++i)
                for (int j = 0; j < GRAPH_WIDTH; ++j)
                    if (graph[i][j] == set2)
                        graph[i][j] = set1;

        }

        edges.erase(it);
    }

}


void LabirinthManager::createFinish() {

    // Creating the finish emerald at the farthest point from the start
    // We've created a MST via the Kruskal's Alrgorithm
    // So a simple BFS can give us one of the farthest points in the tree

    std::unordered_set<Dirt*> visited;
    std::queue<Dirt*> wave;

    Dirt* curr = nullptr;

    Dirt* starting = labirinth[0][0];
    wave.push(starting);

    while (!wave.empty()) {

        curr = wave.front();
        visited.insert(curr);

        if (curr->isPassable(BOTTOM_SIDE)) {
            Dirt* d = labirinth[(curr->getY() - LABIRINTH_START_Y) / GRID_SIZE + 1][(curr->getX() - LABIRINTH_START_X) / GRID_SIZE];
            if (visited.find(d) == visited.end()) {
                visited.insert(d);
                wave.push(d);
            }
        }
        if (curr->isPassable(RIGHT_SIDE)) {
            Dirt* d = labirinth[(curr->getY() - LABIRINTH_START_Y) / GRID_SIZE][(curr->getX() - LABIRINTH_START_X) / GRID_SIZE + 1];
            if (visited.find(d) == visited.end()) {
                visited.insert(d);
                wave.push(d);
            }
        }
        if (curr->isPassable(LEFT_SIDE)) {
            Dirt* d = labirinth[(curr->getY() - LABIRINTH_START_Y) / GRID_SIZE][(curr->getX() - LABIRINTH_START_X) / GRID_SIZE - 1];
            if (visited.find(d) == visited.end()) {
                visited.insert(d);
                wave.push(d);
            }
        }
        if (curr->isPassable(TOP_SIDE)) {
            Dirt* d = labirinth[(curr->getY() - LABIRINTH_START_Y) / GRID_SIZE - 1][(curr->getX() - LABIRINTH_START_X) / GRID_SIZE];
            if (visited.find(d) == visited.end()) {
                visited.insert(d);
                wave.push(d);
            }
        }

        finishX = curr->getX();
        finishY = curr->getY();
        wave.pop();

    }

}


bool LabirinthManager::canMove(int x, int y, const Direction& d) const {

    switch (d) {
        case D_LEFT:
            return labirinth[(y - LABIRINTH_START_Y) / GRID_SIZE][(x - LABIRINTH_START_X) / GRID_SIZE]->isPassable(LEFT_SIDE);
        case D_RIGHT:
            return labirinth[(y - LABIRINTH_START_Y) / GRID_SIZE][(x - LABIRINTH_START_X) / GRID_SIZE]->isPassable(RIGHT_SIDE);
        case D_UP:
            return labirinth[(y - LABIRINTH_START_Y) / GRID_SIZE][(x - LABIRINTH_START_X) / GRID_SIZE]->isPassable(TOP_SIDE);
        case D_DOWN:
            return labirinth[(y - LABIRINTH_START_Y) / GRID_SIZE][(x - LABIRINTH_START_X) / GRID_SIZE]->isPassable(BOTTOM_SIDE);
   }

   return false;

}

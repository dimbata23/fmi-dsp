#pragma once
#ifdef _MSC_VER
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <array>
#include <list>
#include <string>
#include "TextureManager.hpp"
#include "InputHandler.hpp"
#include "TextManager.hpp"
#include "AudioManager.hpp"
#include "LabirinthManager.hpp"
#include "Objects/Object.hpp"
#include "Objects/Dirt.hpp"
#include "Objects/Digger.hpp"
#include "Objects/Emerald.hpp"
#include "Objects/Enemy.hpp"
#include "Objects/Bag.hpp"
#include "Objects/Gold.hpp"
#include "Objects/Spawner.hpp"
#include "Objects/Fireball.hpp"

using ObjectPoolType = std::list<Object*>;

const int GRID_COLS = 15;
const int GRID_ROWS = 10;

class GameEngine {

public:

	// Get the GameEngine instance
	static GameEngine* i();
    GameEngine(const GameEngine& other) = delete;
    GameEngine& operator=(const GameEngine& other) = delete;
    ~GameEngine();

    void handleEvents();
    void update();
    void draw();
    void drawGUI();
	static void release();
	void end() { running = false; }

    bool isRunning() const { return running; }
    Object* createObject(const ObjectType& type, int x, int y, const char* sprite, const char* sprite2 = nullptr);

	Dirt* getDirtAt(int row, int col) const { return field[row][col]; }
    Emerald* getEmeraldAt(int row, int col) { return emeralds[row][col]; }
    Gold* getGoldAt(int row, int col) { return gold[row][col]; }
    void destroyObject(Emerald* em);
    void destroyObject(Gold* el);
	void destroyObject(Object* obj);
    void destroyObject(size_t id);

    Object* getAtPosition(const ObjectType& type, int x, int y);
    ObjectPoolType::iterator getObjectById(size_t id);

    Digger* getPlayer() const { return player; }

	void destroyEnemies();
	bool spawnEnemy(int x, int y);

    void startLabirinth(size_t id);
    bool labirinthCanMove(int x, int y, const Direction& dir) { return lab->canMove(x, y, dir); }

    void drawTexture(int x, int y, const char* texture);

	SDL_Renderer* getRenderer() const { return renderer; }

private:

	GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen = false);
    void clean();
	void clearLevel();
    void generateNextLevel();
    void setupTunnels();

private:

    std::array<std::array<Dirt*, GRID_COLS>, GRID_ROWS> field;
    std::array<std::array<Emerald*, GRID_COLS>, GRID_ROWS> emeralds;
    std::array<std::array<Gold*, GRID_COLS>, GRID_ROWS> gold;
    ObjectPoolType objects;
    SDL_Window* window;
    SDL_Renderer* renderer;
	Digger* player;
	static GameEngine* instance;
	size_t enemiesToSpawn;
	size_t currNumOfEnemies;
	size_t level;
    LabirinthManager* lab;
    int labirinthMode;
    size_t currLabEnemyId;
	size_t numberOfEmeralds;
    bool running;

};

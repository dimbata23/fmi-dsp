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
#include "Objects/Object.hpp"
#include "Objects/Dirt.hpp"
#include "Objects/Digger.hpp"
#include "Objects/Emerald.hpp"
#include "Objects/Enemy.hpp"
#include "Objects/Bag.hpp"
#include "Objects/Gold.hpp"

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

    bool isRunning() const { return running; }
    Object* createObject(const ObjectType& type, int x, int y, const char* sprite, const char* sprite2 = nullptr);

	Dirt* getDirtAt(int row, int col) const { return field[row][col]; }
    Emerald* getEmeraldAt(int row, int col) { return emeralds[row][col]; }
    Gold* getGoldAt(int row, int col) { return gold[row][col]; }
    void destroyObject(Emerald* em);
    void destroyObject(Gold* el);
    void destroyObject(size_t id);

    Object* getAtPosition(const ObjectType& type, int x, int y);
    ObjectPoolType::iterator getObjectById(size_t id);

    Digger* getPlayer() const { return player; }

private:

	GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen = false);
    void clean();
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
    unsigned short level;
    bool running;

};

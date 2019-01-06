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
#include "Objects/Object.hpp"
#include "Objects/Dirt.hpp"
#include "Objects/Digger.hpp"
#include "TextureManager.hpp"
#include "InputHandler.hpp"

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
	void release();

    bool isRunning() const { return running; }
    Object* createObject(const ObjectType& type, int x, int y, int width, int height, const char* sprite, const char* sprite2 = nullptr);

	Dirt* getDirtAt(int row, int col) const { return field[row][col]; }

private:

	GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen = false);
    void clean();
    void generateNextLevel();
    void setupTunnels();

private:

    std::array<std::array<Dirt*, GRID_COLS>, GRID_ROWS> field;
    TextureManager texManager;
    ObjectPoolType objects;
    SDL_Window* window;
    SDL_Renderer* renderer;
	Digger* player;
	static GameEngine* instance;
    unsigned short level;
    bool running;

};

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
#include "TextureManager.hpp"

using ObjectPoolType = std::list<Object*>;

const int GRID_COLS = 15;
const int GRID_ROWS = 10;

class GameEngine {

public:

    GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen = false);
    GameEngine(const GameEngine& other) = delete;
    GameEngine& operator=(const GameEngine& other) = delete;
    ~GameEngine();

    void handleEvents();
    void update();
    void draw();

    bool isRunning() const { return running; }
    Object* createObject(const ObjectType& type, int x, int y, int width, int height, const char* sprite, const char* sprite2 = nullptr);

private:

    void clean();
    void generateNextLevel();
    void setupTunnels();

private:

    std::array<std::array<Dirt*, GRID_COLS>, GRID_ROWS> field;
    TextureManager texManager;
    ObjectPoolType objects;
    SDL_Window* window;
    SDL_Renderer* renderer;
    unsigned short level;
    bool running;

};

#pragma once
#ifdef _MSC_VER
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif
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
#include "Objects/Spawner.hpp"

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

	// Used for closing the window
	void handleEvents();

	// Updates all the objects in the game
    void update();

	// Draws all the objects in the game
    void draw();

	// Draw the GUI of the game
    void drawGUI();

	// Clears out all the memory
	static void release();

	// Ends the game
	void end() { running = false; }

	// Delays the next tick of the engine
	void wait(size_t miliseconds);

	// Returns true if the engine is running
    bool isRunning() const { return running; }

	// Creates an object of specific type with a specified texture and position
    Object* createObject(const ObjectType& type, int x, int y, const char* sprite, const char* sprite2 = nullptr);

	// Returns a pointer to the Dirt object at a specified row and col
	Dirt* getDirtAt(int row, int col) const { return field[row][col]; }

	// Returns a pointer to the Emerald object at a specified row and col
    Emerald* getEmeraldAt(int row, int col) { return emeralds[row][col]; }

	// Returns a pointer to the Gold object at a specified row and col
    Gold* getGoldAt(int row, int col) { return gold[row][col]; }

	// Destroys an Emerald object
    void destroyObject(Emerald* em);

	// Destroys a Gold object
    void destroyObject(Gold* el);

	// Destroys an object
	void destroyObject(Object* obj);

	// Destroys an object by its id
    void destroyObject(size_t id);

	// Returns the object at a specific position or nullptr if none are found
    Object* getAtPosition(const ObjectType& type, int x, int y);

	// Returns an iterator to the object with a specific id 
	// or an iterator to end() if no object with the specifed id exists
    ObjectPoolType::iterator getObjectById(size_t id);

	// Returns a pointer to the Digger object
    Digger* getPlayer() const { return player; }

	// Destroys all the enemies on the screen
	void destroyEnemies();

	// Spawns an enemy at a specified position
	bool spawnEnemy(int x, int y);

	// Starts the labirint mode with an id of the object that caused it
    void startLabirinth(size_t id);

	// Returns wheter or not the player can move a specific direction the labirinth
    bool labirinthCanMove(int x, int y, const Direction& dir) { return lab->canMove(x, y, dir); }

	// Draws a texture at a specified position
    void drawTexture(int x, int y, const char* texture);

	// Returns a pointer to the game renderer
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
	Spawner* spawner;
	static GameEngine* instance;
	size_t enemiesToSpawn;
	size_t currNumOfEnemies;
	size_t level;
    LabirinthManager* lab;
    int labirinthMode;
    size_t currLabEnemyId;
	size_t numberOfEmeralds;
	size_t waitTime;
    bool running;

};

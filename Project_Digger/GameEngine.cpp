#include <iostream>
#include <fstream>
#include "GameEngine.hpp"
#include "Objects/Digger.hpp"

const int GRID_SIZE = 64;
const int GRID_START = 40;

GameEngine::GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen) :
	field({nullptr, }),
    window(nullptr),
    renderer(nullptr),
	player(nullptr),
    level(0),
    running(false)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Couldn't initialize SDL! Error: " << SDL_GetError() << std::endl;
        return;
    }

    std::cout << "Initializing game engine..." << std::endl;

    window = SDL_CreateWindow(title, x, y, width, height, (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
    if (!window) {
        std::cerr << "Couldn't create window! Error: " << SDL_GetError() << std::endl;
        clean();
        return;
    }

    std::cout << "Window sucessfully created." << std::endl;

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Couldn't create game renderer! Error: " << SDL_GetError() << std::endl;
        clean();
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    std::cout << "Game renderer sucessfully created." << std::endl;

    running = true;

    generateNextLevel();

}


GameEngine::~GameEngine() {
    clean();
}


void GameEngine::handleEvents() {

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {

	case SDL_QUIT:
		running = false;
		break;

	default:
		break;

	}

}


void GameEngine::update() {

	InputHandler::update();

    for (Object* obj : objects)
        obj->update();
	player->update();

}


void GameEngine::draw() {

    SDL_RenderClear(renderer);
    for (Object* obj: objects)
        obj->draw();
	player->draw();
    SDL_RenderPresent(renderer);

}


Object* GameEngine::createObject(const ObjectType& type, int x, int y, int width, int height, const char* sprite, const char* sprite2) {

    Object* result = nullptr;
    SDL_Texture* tex = nullptr;
    SDL_Texture* tex2 = nullptr;
    if (sprite)
    	tex = texManager.sprite(sprite, renderer);
    if (sprite2)
    	tex2 = texManager.sprite(sprite2, renderer);

    switch(type) {

    case ANY:
        result = new Object(x, y, width, height, 0, 0, tex, renderer);
        break;

    case DIRT:
        result = new Dirt(x, y, false, tex, tex2, renderer);
        break;

    case TUNNEL:
    	result = new Dirt(x, y, true, tex, tex2, renderer);
    	break;

    case DIGGER:
        result = player = new Digger(x, y, tex, renderer);
        break;

    case GEM:
        break;

    case BAG:
        break;

    case ENEMY:
        break;

    case SPAWNER:
        break;

    }

	if (result != player)
		objects.push_front(result);

    return result;

}


void GameEngine::clean() {

    std::cout << std::endl << "Deleting objects..." << std::endl;
    size_t count = 0;
    for (Object* obj : objects) {
        delete obj;
		obj = nullptr;
        ++count;
    }
	delete player;
	player = nullptr;
    std::cout << "Deleted " << count + 1 << " objects." << std::endl;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

}


void GameEngine::generateNextLevel() {

	std::string levelName = "Levels/level" + std::to_string(++level);
	std::cout << "Loading level: " << levelName << std::endl;
    std::ifstream in(levelName);
    if (!in) {
        std::cout << "No next level!" << std::endl;
        return;
    }

    char ch;
    for ( int y = GRID_START; !in.eof(); y += GRID_SIZE ) {
        in.get(ch);
        for ( int x = 0; ch != '\n'; x += GRID_SIZE ) {
            switch (ch) {
            case 'd':
                field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, GRID_SIZE, GRID_SIZE, "Sprites/dirt2_64x64.png", "Sprites/dirt_border.png"));
                break;
            case '0':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, GRID_SIZE, GRID_SIZE, "Sprites/dirt2_64x64.png", "Sprites/dirt_border.png"));
				break;
            case 'g':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, GRID_SIZE, GRID_SIZE, "Sprites/dirt2_64x64.png", "Sprites/dirt_border.png"));
                // gem
                break;
            case 'b':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, GRID_SIZE, GRID_SIZE, "Sprites/dirt2_64x64.png", "Sprites/dirt_border.png"));
                // create bag
                break;
            case 'e':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, GRID_SIZE, GRID_SIZE, "Sprites/dirt2_64x64.png", "Sprites/dirt_border.png"));
                // create enemy spawner
            	break;
            case 'p':
            	createObject(DIGGER, x, y, GRID_SIZE, GRID_SIZE, "Sprites/digger.png");
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, GRID_SIZE, GRID_SIZE, "Sprites/dirt2_64x64.png", "Sprites/dirt_border.png"));
                // create player obj
            	break;
            default:
                break;
            }
            in.get(ch);
        }
    }
    in.close();

    setupTunnels();

}


void GameEngine::setupTunnels() {

	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (field[i][j] && i > 0 && field[i - 1][j] && field[i - 1][j]->getType() == TUNNEL)
				field[i][j]->setPassable(TOP_SIDE, true);
			if (field[i][j] && j > 0 && field[i][j - 1] && field[i][j - 1]->getType() == TUNNEL)
				field[i][j]->setPassable(LEFT_SIDE, true);
			if (field[i][j] && i < GRID_ROWS - 1 && field[i + 1][j] && field[i + 1][j]->getType() == TUNNEL)
				field[i][j]->setPassable(BOTTOM_SIDE, true);
			if (field[i][j] && j < GRID_COLS - 1 && field[i][j + 1] && field[i][j + 1]->getType() == TUNNEL)
				field[i][j]->setPassable(RIGHT_SIDE, true);
		}
	}

}

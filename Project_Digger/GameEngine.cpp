#include <iostream>
#include <fstream>
#include "GameEngine.hpp"

const int GRID_SIZE = 64;
const int GRID_START = 40;

const int WIDTH = 960;
const int HEIGHT = 680;

const char* WINDOW_TITLE = "Project Digger";
const char* DIRT_SPRITE = "Sprites/dirt2_64x64.png";
const char* DIRT_BORDER_SPRITE = "Sprites/dirt_border.png";
const char* DIGGER_SPRITE = "Sprites/digger.png";
const char* EMERALD_SPRITE = "Sprites/emerald.png";


const SDL_Rect GUI_BG_SRC_RECT = { 0, 0, GRID_SIZE, GRID_SIZE};
const SDL_Rect GUI_BG_DEST_RECT = { 0, GRID_START - GRID_SIZE, GRID_SIZE, GRID_SIZE};
const SDL_Rect GUI_BORDER_SRC_RECT = {GRID_SIZE * 3, 0, GRID_SIZE, GRID_SIZE};


GameEngine* GameEngine::instance = nullptr;


GameEngine* GameEngine::i() {
	return (instance ? instance : instance = new GameEngine(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT));
}


GameEngine::GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen) :
	field({nullptr, }),
	emeralds({nullptr, }),
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

    if (TTF_Init() != 0) {
        std::cout << "Couldn't initialize font system! Error: " << TTF_GetError() << std::endl;
        clean();
        return;
    }

    std::cout << "Font system sucessfully initialized." << std::endl;

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

	player->update();

   for (auto& objPair : objects)
        objPair.second->update();

	// DEBUG
	//int mx;
	//int my;
	//if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT))
	//	field[(my - GRID_START) / GRID_SIZE][mx / GRID_SIZE]->print();

}


void GameEngine::draw() {

    SDL_RenderClear(renderer);
    for (auto& objPair : objects) {
        objPair.second->draw();
    }

    for (auto& arr : emeralds) {
        for (auto& em : arr) {
            if (em) {
                em->draw();
            }
        }
    }

	player->draw();

}


void GameEngine::drawGUI() {

    SDL_Rect dest = GUI_BG_DEST_RECT;
    SDL_Texture* bgSpr = texManager.sprite(DIRT_SPRITE, renderer);
    for (int i = 0; i < GRID_COLS * GRID_SIZE; i += GRID_SIZE) {
        dest.x = i;
        SDL_SetTextureAlphaMod(bgSpr, 60);
        SDL_RenderCopy(renderer, bgSpr, &GUI_BG_SRC_RECT, &dest);
        SDL_RenderCopy(renderer, texManager.sprite(DIRT_BORDER_SPRITE, renderer), &GUI_BORDER_SRC_RECT, &dest);
    }

    TextManager::i()->setFont("Fonts/Score.ttf", 45);
	TextManager::i()->drawText(player->getScoreString().c_str(), 9, -4, renderer);

    SDL_RenderPresent(renderer);

}


void GameEngine::release() {
    TextManager::release();
	delete instance;
	instance = nullptr;
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

    case EMERALD:
        result = new Emerald(x, y, tex, renderer);
        break;

    case BAG:
        break;

    case ENEMY:
        break;

    case SPAWNER:
        break;

    }

	if (type != DIGGER && type != EMERALD)
		objects.insert(std::make_pair(result->getId(), result));

    return result;

}


void GameEngine::clean() {

    std::cout << std::endl << "Deleting objects..." << std::endl;
    size_t count = 0;
    for (auto& objPair : objects) {
        delete objPair.second;
		objPair.second = nullptr;
        ++count;
    }
	delete player;
	player = nullptr;
    ++count;
    for (auto& arr : emeralds) {
        for (auto& em : arr) {
            if (em) {
                delete em;
                em = nullptr;
                ++count;
            }
        }
    }
    std::cout << "Deleted " << count << " objects." << std::endl;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    TTF_Quit();
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
                field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, GRID_SIZE, GRID_SIZE, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                break;
            case '0':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, GRID_SIZE, GRID_SIZE, DIRT_SPRITE, DIRT_BORDER_SPRITE));
				break;
            case 'g':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, GRID_SIZE, GRID_SIZE, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                emeralds[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Emerald*>(createObject(EMERALD, x, y, GRID_SIZE, GRID_SIZE, EMERALD_SPRITE));
                break;
            case 'b':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, GRID_SIZE, GRID_SIZE, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                // create bag
                break;
            case 'e':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, GRID_SIZE, GRID_SIZE, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                // create enemy spawner
            	break;
            case 'p':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, GRID_SIZE, GRID_SIZE, DIRT_SPRITE, DIRT_BORDER_SPRITE));
            	createObject(DIGGER, x, y, GRID_SIZE, GRID_SIZE, DIGGER_SPRITE);
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
			if (field[i][j] && i > 0 && field[i - 1][j] && field[i - 1][j]->getType() == TUNNEL && field[i][j]->getType() == TUNNEL)
				field[i][j]->setPassable(TOP_SIDE, true);
			if (field[i][j] && j > 0 && field[i][j - 1] && field[i][j - 1]->getType() == TUNNEL && field[i][j]->getType() == TUNNEL)
				field[i][j]->setPassable(LEFT_SIDE, true);
			if (field[i][j] && i < GRID_ROWS - 1 && field[i + 1][j] && field[i + 1][j]->getType() == TUNNEL && field[i][j]->getType() == TUNNEL)
				field[i][j]->setPassable(BOTTOM_SIDE, true);
			if (field[i][j] && j < GRID_COLS - 1 && field[i][j + 1] && field[i][j + 1]->getType() == TUNNEL && field[i][j]->getType() == TUNNEL)
				field[i][j]->setPassable(RIGHT_SIDE, true);
		}
	}

}


void GameEngine::destroyEmerald(Emerald* em) {
    emeralds[(em->getY() - GRID_START) / GRID_SIZE][em->getX() / GRID_SIZE] = nullptr;
    delete em;
}

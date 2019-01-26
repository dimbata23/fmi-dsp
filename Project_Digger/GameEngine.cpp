#include <iostream>
#include <fstream>
#include <algorithm>
#include "GameEngine.hpp"

const int GRID_SIZE = 64;
const int GRID_START = 40;

const int WIDTH = 960;
const int HEIGHT = 680;

const char* WINDOW_TITLE        = "Project Digger";
const char* DIRT_SPRITE         = "Sprites/dirt2_64x64.png";
const char* DIRT_BORDER_SPRITE  = "Sprites/dirt_border.png";
const char* DIGGER_SPRITE       = "Sprites/digger.png";
const char* EMERALD_SPRITE      = "Sprites/emerald.png";

const char* SCORE_FONT  = "Fonts/Score.ttf";
const int   FONT_SIZE   = 45;

SDL_Texture* BG_TEXTURE = nullptr;
SDL_Texture* BG_BORDER_TEXTURE = nullptr;

const SDL_Rect GUI_BG_SRC_RECT = { 0, 0, GRID_SIZE, GRID_SIZE};
const SDL_Rect GUI_BG_DEST_RECT = { 0, GRID_START - GRID_SIZE, GRID_SIZE, GRID_SIZE};
const SDL_Rect GUI_BORDER_SRC_RECT = {GRID_SIZE * 3, 0, GRID_SIZE, GRID_SIZE};

const int LIVES_X_POS = GRID_SIZE * 1.5;
const int LIVES_Y_POS = -3;
const int LIVES_SIZE = GRID_START;


GameEngine* GameEngine::instance = nullptr;


GameEngine* GameEngine::i() {
	return instance ? instance : instance = new GameEngine(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT);
}


GameEngine::GameEngine(const char* title, int x, int y, int width, int height, bool fullscreen) :
	field({nullptr, }),
	emeralds({nullptr, }),
	gold({nullptr, }),
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

    BG_TEXTURE = TextureManager::i()->sprite(DIRT_SPRITE, renderer);
    BG_BORDER_TEXTURE = TextureManager::i()->sprite(DIRT_BORDER_SPRITE, renderer);

    generateNextLevel();

    TextManager::i()->setFont(SCORE_FONT, FONT_SIZE);

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

	for (auto& obj : objects) {
		if (obj)
			obj->update();
	}
    
    for (auto it = objects.begin(); it != objects.end();) {
        if (!*it)
            it = objects.erase(it);
        else
            ++it;
    }

    for (auto& arr : field) {
        for (auto& dirt : arr) {
            if (dirt) {
                dirt->update();
            }
        }
    }

}


void GameEngine::draw() {

    SDL_RenderClear(renderer);

    for (auto& arr : field) {
        for (auto& dirt : arr) {
            if (dirt) {
                dirt->draw();
            }
        }
    }

    for (auto& arr : emeralds) {
        for (auto& em : arr) {
            if (em) {
                em->draw();
            }
        }
    }

    for (auto& arr : gold) {
        for (auto& el : arr) {
            if (el) {
                el->draw();
            }
        }
    }

    for (auto& obj : objects) {
        obj->draw();
    }

	player->draw();

}


void GameEngine::drawGUI() {
    
    SDL_Rect dest = GUI_BG_DEST_RECT;
    for (int i = 0; i < GRID_COLS * GRID_SIZE; i += GRID_SIZE) {
        dest.x = i;
        SDL_SetTextureAlphaMod(BG_TEXTURE, 60);
        SDL_RenderCopy(renderer, BG_TEXTURE, &GUI_BG_SRC_RECT, &dest);
        SDL_RenderCopy(renderer, BG_BORDER_TEXTURE, &GUI_BORDER_SRC_RECT, &dest);
    }

	TextManager::i()->drawText(player->getScoreString().c_str(), 9, -4, renderer);

	SDL_Rect livesDest = { LIVES_X_POS, LIVES_Y_POS, LIVES_SIZE, LIVES_SIZE };
	for (int i = 0; i < player->getLives(); ++i) {
		livesDest.x = LIVES_X_POS + LIVES_SIZE * i;
		SDL_RenderCopy(renderer, player->getSprite(), &player->getSrcRect(), &livesDest);
	}

    SDL_RenderPresent(renderer);

}


void GameEngine::release() {

    TextManager::release();
    TextureManager::release();
	delete instance;
	instance = nullptr;

}


Object* GameEngine::createObject(const ObjectType& type, int x, int y, const char* sprite, const char* sprite2) {

    Object* result = nullptr;
    SDL_Texture* tex = nullptr;
    SDL_Texture* tex2 = nullptr;
    if (sprite)
    	tex = TextureManager::i()->sprite(sprite, renderer);
    if (sprite2)
    	tex2 = TextureManager::i()->sprite(sprite2, renderer);

    switch(type) {

    case ANY:
        result = new Object(x, y, 0, 0, tex, renderer);
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
        emeralds[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Emerald*>(result = new Emerald(x, y, tex, renderer));
        break;

    case GOLD:
        gold[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Gold*>(result = new Gold(x, y, tex, renderer));
        break;

    case BAG:
        result = new Bag(x, y, tex, renderer);
        break;

    case ENEMY:
        result = new Enemy(x, y, tex, renderer);
        break;

    case SPAWNER:
        break;

    }

	if (type == BAG || type == ENEMY)
		objects.push_back(result);

    return result;

}


void GameEngine::clean() {

    std::cout << std::endl << "Deleting objects..." << std::endl;
    size_t count = 0;
    for (auto& obj : objects) {
        delete obj;
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
    for (auto& arr : gold) {
        for (auto& el : arr) {
            if (el) {
                delete el;
                el = nullptr;
                ++count;
            }
        }
    }
    for (auto& arr : field) {
        for (auto& el : arr) {
            if (el) {
                delete el;
                el = nullptr;
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
	std::cout << "Loading level \"" << levelName << '\"' << std::endl;
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
                field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                break;
            case '0':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
				break;
            case 'g':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                emeralds[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Emerald*>(createObject(EMERALD, x, y, EMERALD_SPRITE));
                break;
            case 'b':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(DIRT, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                createObject(BAG, x, y, "Sprites/bag.png");
                break;
            case 'e':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                // create enemy spawner
                createObject(ENEMY, x, y, "Sprites/enemy.png");
            	break;
            case 'p':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
            	createObject(DIGGER, x, y, DIGGER_SPRITE);
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


void GameEngine::destroyObject(Emerald* em) {
    emeralds[(em->getY() - GRID_START) / GRID_SIZE][em->getX() / GRID_SIZE] = nullptr;
    delete em;
}


void GameEngine::destroyObject(Gold* el) {
    gold[(el->getY() - GRID_START) / GRID_SIZE][el->getX() / GRID_SIZE] = nullptr;
    delete el;
}


void GameEngine::destroyObject(Object* obj) {

	switch (obj->getType()) {
		case EMERALD:
			destroyObject(dynamic_cast<Emerald*>(obj));
			break;
		case GOLD:
			destroyObject(dynamic_cast<Gold*>(obj));
			break;
		default:
			destroyObject(obj->getId());
			break;
	}

}


void GameEngine::destroyObject(size_t id) {

    auto o = getObjectById(id);
    if (o == objects.end())
        return;

    if ((*o)->getType() == BAG) {
        delete dynamic_cast<Bag*>(*o);
        *o = nullptr;
    } else if ((*o)->getType() == ENEMY) {
		delete dynamic_cast<Enemy*>(*o);
		*o = nullptr;
    }

}


Object* GameEngine::getAtPosition(const ObjectType& type, int x, int y) {

	if (type == DIGGER && x >= player->getX() && x < player->getX() + GRID_SIZE
		&& y >= player->getY() && y < player->getY() + GRID_SIZE)
			return player;

    if (type == ENEMY || type == BAG)
        for (auto& i : objects)
            if (i && i->getType() == type && x >= i->getX() && x < i->getX() + GRID_SIZE
				&& y >= i->getY() && y < i->getY() + GRID_SIZE)
					return i;

    return nullptr;

}


ObjectPoolType::iterator GameEngine::getObjectById(size_t id) {

    auto result = objects.begin();
    while (result != objects.end()) {
        if ((*result)->getId() == id)
            break;
        ++result;
    }
    return result;

}


void GameEngine::destroyEnemies() {

	for (auto obj : objects)
		if (obj->getType() == ENEMY)
			destroyObject(obj);

}

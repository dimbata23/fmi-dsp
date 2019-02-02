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
const char* EMERALD_SPRITE		= "Sprites/emerald.png";
const char* BAG_SPRITE			= "Sprites/bag.png";
const char* ENEMY_SPRITE		= "Sprites/enemy.png";

const char* SCORE_FONT  = "Fonts/Score.ttf";
const int   FONT_SIZE   = 45;

const char* BACKGROUND_MUSIC = "Sounds/background.wav";
const char* VICTORY_SOUND = "Sounds/victory.wav";

SDL_Texture* BG_TEXTURE = nullptr;
SDL_Texture* BG_BORDER_TEXTURE = nullptr;

const SDL_Rect GUI_BG_SRC_RECT = { 0, 0, GRID_SIZE, GRID_SIZE};
const SDL_Rect GUI_BG_DEST_RECT = { 0, GRID_START - GRID_SIZE, GRID_SIZE, GRID_SIZE};
const SDL_Rect GUI_BORDER_SRC_RECT = {GRID_SIZE * 3, 0, GRID_SIZE, GRID_SIZE};

const int LIVES_X_POS = GRID_SIZE * 1.5;
const int LIVES_Y_POS = -3;
const int LIVES_SIZE = GRID_START;

const int MAX_ENEMIES_ONSCREEN = 4;


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
	enemiesToSpawn(0),
	currNumOfEnemies(0),
    level(0),
    labirinthMode(LAB_OFF),
    lab(nullptr),
    currLabEnemyId(0),
	numberOfEmeralds(0),
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

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512) != 0) {
		std::cout << "Couldn't initialize audio system! Error: " << Mix_GetError() << std::endl;
		clean();
		return;
	}

	AudioManager::i()->playMusic(AudioManager::i()->musicAudio(BACKGROUND_MUSIC));

	std::cout << "Audio system sucessfully initialized." << std::endl;

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

    if (labirinthMode == LAB_START) {

        lab = new LabirinthManager();
        lab->createLabirinth();
        labirinthMode = LAB_RUNNING;

    }

    InputHandler::update();

    if (lab) {

        LabirinthMode mode;
        mode = lab->update();
        if (mode != LAB_RUNNING) {
            delete lab;
            lab = nullptr;
            AudioManager::i()->playMusic(AudioManager::i()->musicAudio(BACKGROUND_MUSIC));
            draw();
            drawGUI();
        }
        switch (mode) {
            case LAB_WON:
                destroyObject(currLabEnemyId);
                draw();
                drawGUI();
                SDL_Delay(1000);
                break;
            case LAB_LOST:
                player->kill();
                break;
            default:
                break;
        }

    } else {

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

		if (numberOfEmeralds == 0) {
			draw();
			drawGUI();
			AudioManager::i()->pauseMusic();
			AudioManager::i()->playSoundEffect(AudioManager::i()->soundEffect(VICTORY_SOUND));
			SDL_Delay(2000);
			clearLevel();
			generateNextLevel();
			AudioManager::i()->playMusic(AudioManager::i()->musicAudio(BACKGROUND_MUSIC));
		}

    }

}


void GameEngine::draw() {


    SDL_RenderClear(renderer);

    if (!lab) {

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
            if (obj)
                obj->draw();
        }

        player->draw();

    } else {

        SDL_RenderClear(renderer);
        lab->draw();

    }

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

	if (lab)
		lab->drawGUI();

    SDL_RenderPresent(renderer);

}


void GameEngine::release() {

    TextManager::release();
    TextureManager::release();
	AudioManager::release();
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
        result = new Digger(x, y, tex, renderer);
        if (!player)
            player = dynamic_cast<Digger*>(result);
		else
			objects.push_back(result);
        break;

    case EMERALD:
		++numberOfEmeralds;
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
		result = new Spawner(x, y, tex, renderer);
        break;

    case FIREBALL:
        result = new Fireball(x, y, tex, renderer, player->getDirection());
        break;

    }

	if (type == BAG || type == ENEMY || type == SPAWNER || type == FIREBALL)
		objects.push_back(result);

    return result;

}


void GameEngine::clean() {

    delete lab;
    lab = nullptr;

    std::cout << std::endl << "Deleting objects..." << std::endl;
    
	clearLevel();
	delete player;
	player = nullptr;

	std::cout << "Destroying window..." << std::endl;
    SDL_DestroyWindow(window);
	std::cout << "Destroying game renderer..." << std::endl;
    SDL_DestroyRenderer(renderer);
    
	std::cout << "Closing audio system..." << std::endl;
	Mix_CloseAudio();
	std::cout << "Closing font system..." << std::endl;
    TTF_Quit();
	std::cout << "Closing game engine..." << std::endl;
    SDL_Quit();
	std::cout << "Done!" << std::endl;

}


void GameEngine::clearLevel()
{
	for (auto& obj : objects) {
		delete obj;
		obj = nullptr;
	}
	for (auto& arr : emeralds) {
		for (auto& em : arr) {
			if (em) {
				delete em;
				em = nullptr;
			}
		}
	}
	for (auto& arr : gold) {
		for (auto& el : arr) {
			if (el) {
				delete el;
				el = nullptr;
			}
		}
	}
	for (auto& arr : field) {
		for (auto& el : arr) {
			if (el) {
				delete el;
				el = nullptr;
			}
		}
	}
}


void GameEngine::generateNextLevel() {

	enemiesToSpawn = 6 + level;

	std::string levelName = "Levels/level" + std::to_string(++level);
	std::cout << "Loading level \"" << levelName << '\"' << std::endl;
    std::ifstream in(levelName);
    if (!in) {
        std::cout << "No next level!" << std::endl;
		running = false;
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
                createObject(BAG, x, y, BAG_SPRITE);
                break;
            case 'e':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
                createObject(SPAWNER, x, y, nullptr);
            	break;
            case 'p':
            	field[y / GRID_SIZE][x / GRID_SIZE] = dynamic_cast<Dirt*>(createObject(TUNNEL, x, y, DIRT_SPRITE, DIRT_BORDER_SPRITE));
				if (!player) {
            		createObject(DIGGER, x, y, DIGGER_SPRITE);
				} else {
					player->setPosition(x, y);
				}
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
	--numberOfEmeralds;
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
		--currNumOfEnemies;
    } else if ((*o)->getType() == SPAWNER) {
		delete dynamic_cast<Spawner*>(*o);
		*o = nullptr;
	} else if ((*o)->getType() == FIREBALL) {
		delete dynamic_cast<Fireball*>(*o);
		*o = nullptr;
    } else if ((*o)->getType() == DIGGER) {
		delete dynamic_cast<Digger*>(*o);
		*o = nullptr;
    }

}


Object* GameEngine::getAtPosition(const ObjectType& type, int x, int y) {

	if (type == DIGGER && x >= player->getX() && x < player->getX() + GRID_SIZE
		&& y >= player->getY() && y < player->getY() + GRID_SIZE)
			return player;

    if (type == ENEMY || type == BAG || type == SPAWNER)
        for (auto& i : objects)
            if (i && i->getType() == type && x >= i->getX() && x < i->getX() + GRID_SIZE
				&& y >= i->getY() && y < i->getY() + GRID_SIZE)
					return i;

    return nullptr;

}


ObjectPoolType::iterator GameEngine::getObjectById(size_t id) {

    auto result = objects.begin();
    while (result != objects.end()) {
        if (*result && (*result)->getId() == id)
            break;
        ++result;
    }
    return result;

}


void GameEngine::destroyEnemies() {

	for (auto obj : objects)
		if (obj && obj->getType() == ENEMY)
			destroyObject(obj);

	currNumOfEnemies = 0;
	enemiesToSpawn = 6 + level;

}


bool GameEngine::spawnEnemy(int x, int y) {

	if (enemiesToSpawn == 0) {
		// TODO: Create cherry
		destroyObject(GameEngine::getAtPosition(SPAWNER, x, y)->getId());
	}

	if (currNumOfEnemies < MAX_ENEMIES_ONSCREEN) {
		createObject(ENEMY, x, y, ENEMY_SPRITE);
		--enemiesToSpawn;
		++currNumOfEnemies;
		return true;
	}

	return false;

}


void GameEngine::startLabirinth(size_t id) { 
    currLabEnemyId = id; 
    labirinthMode = LAB_START;
}


void GameEngine::drawTexture(int x, int y, const char* texture) {

    SDL_Rect def = {0, 0, GRID_SIZE, GRID_SIZE};
    SDL_Rect defD = {x, y, GRID_SIZE, GRID_SIZE};
    SDL_RenderCopy(renderer, TextureManager::i()->sprite(texture, renderer), &def, &defD);

}

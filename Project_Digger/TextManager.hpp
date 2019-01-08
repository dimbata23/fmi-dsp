#pragma once
#include <unordered_map>
#ifdef _MSC_VER
#include <SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif

using FontsPoolType = std::unordered_map<std::string, TTF_Font*>;

class TextManager {

public:

    // Get the FontManager instance
	static TextManager* i();
    TextManager(const TextManager& other) = delete;
    TextManager& operator=(const TextManager& other) = delete;
    ~TextManager();

    void setFont(const std::string& text, int size);
    void drawText(const char* text, int x, int y, SDL_Renderer* ren, SDL_Color color = {255, 255, 255, 255});
    static void release();

private:

    FontsPoolType fonts;
    TextManager();
    static TextManager* instance;
    TTF_Font* font;

};

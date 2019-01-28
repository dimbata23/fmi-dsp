#include <iostream>
#include <string>
#include "TextManager.hpp"


TextManager* TextManager::instance = nullptr;


TextManager* TextManager::i() {
    return (instance ? instance : instance = new TextManager());
}


TextManager::TextManager() : font(nullptr) {}


TextManager::~TextManager() {

    std::cout << std::endl << "Unloading fonts..." << std::endl;
    for (auto& f : fonts) {
        TTF_CloseFont(f.second);
        std::cout << "Unloaded font \"" << f.first << '\"' << std::endl;
    }
    this->font = nullptr;

}


void TextManager::setFont(const std::string& text, int size) {

    std::string key = text + std::to_string(size);
	FontsPoolType::iterator it = fonts.find(key);
    if (it == fonts.end()) {
        this->font = TTF_OpenFont(text.c_str(), size);
		if (!this->font)
			std::cerr << "[ERROR] Unable to load \"" << text << "\"! Error: " << TTF_GetError() << std::endl;
        fonts.insert(std::make_pair(key, this->font));
    } else {
        this->font = it->second;
    }

}


void TextManager::drawText(const char* text, int x, int y, SDL_Renderer* ren, SDL_Color color) {

    SDL_Surface* surface = TTF_RenderText_Solid(this->font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(ren, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };
    SDL_RenderCopy(ren, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

}


void TextManager::release() {

    delete instance;
    instance = nullptr;
    
}

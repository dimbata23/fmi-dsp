#include <iostream>
#include "TextureManager.hpp"


TextureManager* TextureManager::instance = nullptr;


TextureManager* TextureManager::i() {
    return (instance ? instance : instance = new TextureManager());
}


TextureManager::TextureManager() {}


void TextureManager::release() {

    delete instance;
    instance = nullptr;

}


TextureManager::~TextureManager() {

    std::cout << std::endl << "Unloading textures..." << std::endl;
    for (const std::pair<const char*, SDL_Texture*>& p : textures) {
        std::cout << "Unloading texture \"" << p.first << "\"." << std::endl;
        SDL_DestroyTexture(p.second);
    }

}


SDL_Texture* TextureManager::sprite(const char* file, SDL_Renderer* ren) {

    SDL_Texture* result = nullptr;
    std::unordered_map<const char*, SDL_Texture*>::iterator it = textures.find(file);
    if (it == textures.end()) {
        result = TextureManager::loadTexture(file, ren);
        textures.insert(std::make_pair(file, result));
    } else {
        result = it->second;
    }
    return result;

}


SDL_Texture* TextureManager::loadTexture(const char* fileName, SDL_Renderer* ren) {

    SDL_Surface* temp = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, temp);
    if (!tex)
        std::cerr << "[ERROR] Unable to load " << fileName << '!' << std::endl;
    SDL_FreeSurface(temp);

    return tex;
    
}

#include "Texture.h"
#include "Ecran.h"

Texture* Texture::s_Instance = nullptr;
bool Texture::Load(std::string id, std::string filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());

    if (surface == nullptr) {
        SDL_Log("Eroare la incarcarea imagini: %s, %s", filename.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Ecran::GetInstance()->GetRenderer(), surface);
    if (texture == nullptr) {
        SDL_Log("Eroare la crearea texturii: %s", SDL_GetError());
        return false;
    }

    TextureMap[id] = texture;
    return true;
}

void Texture::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip) {
    SDL_Rect srcRect = {0, 0, width, height};
    SDL_Rect dstRect = {x, y, width, height};

    SDL_RenderCopyEx(Ecran::GetInstance()->GetRenderer(), TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void Texture::Drop(std::string id) {

}

void Texture::Clean() {

}

#include "Texture.h"

Texture::Texture(const char* imageFilePath){
    this->loadTexture(imageFilePath);
}


Texture::~Texture(){
    SDL_FreeSurface(this->image);
}

void Texture::loadTexture(const char* imageFilePath){
    this->image = STBIMG_Load(imageFilePath);
    if(image == NULL) {
      printf("ERROR: Couldn't load %s, reason: %s\n", imageFilePath, SDL_GetError());
      exit(1);
    }
}

SDL_Surface* Texture::getSurface(){
    return this->image;
}


#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>



class Texture{
    public:
        Texture(const char* imageFilePath);
        ~Texture();

        void loadTexture(const char* imageFilePath);
        SDL_Surface* getSurface();

    private:
        SDL_Surface *image;

};

#endif
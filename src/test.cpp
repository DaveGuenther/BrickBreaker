//Core includes
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

//SDL related
#include <SDL2/SDL.h>
#define SDL_STBIMAGE_IMPLEMENTATION
#include "3rd_party/bitmap_font.h"



int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN);  //Steamdeck is 1280x800
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    std::shared_ptr<stbimageTexture> entire_font_texture(new stbimageTexture(renderer, "../assets/stone_term.png"));
    SDL_Rect letter_A_Rect;
    letter_A_Rect.h=50;
    letter_A_Rect.w=25;
    letter_A_Rect.x=25;
    letter_A_Rect.y=0;
    std::shared_ptr<Glyph> Glyph_Exclamation(new Glyph(entire_font_texture, letter_A_Rect, 33,std::string("!")));


    
    std::ofstream file_obj;
    file_obj.open("glyph_object_for_testing.obj", std::ios::app);
    file_obj.write((char*)&Glyph_Exclamation, sizeof(Glyph_Exclamation));
    file_obj.close();
    

    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit(); 



	return 0;
}
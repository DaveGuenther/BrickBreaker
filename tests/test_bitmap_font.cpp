#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "../src/3rd_party/bitmap_font.h"
#include "../src/texture/Texture.h"


#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Test sample Sum function", "[default]"){
    int a = 5;
    int b = 10;
    REQUIRE(sum_nums(a, b)==15);
}

TEST_CASE("Test Glyph Class Instantiation", "[font]"){
    SDL_Init(SDL_INIT_EVERYTHING);
    std::unique_ptr<Texture> temp_texture(new Texture("assets/stone_term.png"));
    std::unique_ptr<Texture> png_image;
    png_image=std::move(temp_texture);
    SDL_Surface* this_font = png_image->getSurface();
    SDL_Rect letter_A_Rect;
    letter_A_Rect.h=50;
    letter_A_Rect.w=25;
    letter_A_Rect.x=25;
    letter_A_Rect.y=0;
    Glyph letter_A(this_font, letter_A_Rect, 33,std::string("!"));
    SDL_Quit();
}
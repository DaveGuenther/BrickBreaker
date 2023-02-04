#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "../src/3rd_party/bitmap_font.h"
#include "../src/texture/Texture.h"


#define CATCH_CONFIG_MAIN
#include "catch.hpp"



TEST_CASE("Load Bitmap Font CSV","[font]"){
    SDL_Init(SDL_INIT_EVERYTHING);
    REQUIRE_THROWS(CSV_Object("tests/stone_term_bad1.csv")); // Invalid ASCII Code
    REQUIRE_THROWS(CSV_Object("tests/stone_term_bad2.csv")); // Invalid Width
    REQUIRE_NOTHROW(CSV_Object("assets/stone_term.csv"));

    CSV_Object my_CSV("assets/stone_term.csv");
    REQUIRE(my_CSV.getNumRows()==96);
    REQUIRE(my_CSV.getRowByID(0).ASCII_code==32);
    REQUIRE(my_CSV.getRowByID(0).cumulativeXPos==0);
    REQUIRE(my_CSV.getRowByID(0).ASCII_chars=="SP");        
    REQUIRE(my_CSV.getRowByID(1).ASCII_code==33);
    REQUIRE(my_CSV.getRowByID(1).cumulativeXPos==5);
    REQUIRE(my_CSV.getRowByID(1).ASCII_chars=="!");
    REQUIRE(my_CSV.getRowByID(95).ASCII_code==127);
    REQUIRE(my_CSV.getRowByID(95).cumulativeXPos==475);
    REQUIRE(my_CSV.getRowByID(95).ASCII_chars=="default");

    REQUIRE(my_CSV.getRowByASCII_Code(32).ASCII_code==32);
    REQUIRE(my_CSV.getRowByASCII_Code(32).cumulativeXPos==0);
    REQUIRE(my_CSV.getRowByASCII_Code(32).ASCII_chars=="SP");        
    REQUIRE(my_CSV.getRowByASCII_Code(33).ASCII_code==33);
    REQUIRE(my_CSV.getRowByASCII_Code(33).cumulativeXPos==5);
    REQUIRE(my_CSV.getRowByASCII_Code(33).ASCII_chars=="!");
    REQUIRE(my_CSV.getRowByASCII_Code(95).ASCII_code==95);
    REQUIRE(my_CSV.getRowByASCII_Code(95).cumulativeXPos==315);
    REQUIRE(my_CSV.getRowByASCII_Code(95).ASCII_chars=="_");

    REQUIRE_THROWS(my_CSV.getRowByID(-1).ASCII_chars); // Out of Bounds
    REQUIRE_THROWS(my_CSV.getRowByASCII_Code(-1).ASCII_chars); // Out of Bounds
    REQUIRE_THROWS(my_CSV.getRowByID(96).ASCII_chars); // Out of Bounds
    REQUIRE_THROWS(my_CSV.getRowByASCII_Code(128).ASCII_chars); // Out of Bounds
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
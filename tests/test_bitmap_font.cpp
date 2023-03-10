#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#define SDL_STBIMAGE_IMPLEMENTATION
#include "../src/3rd_party/bitmap_font.h"



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
    REQUIRE(my_CSV.getRowByID(1).cumulativeXPos==25);
    REQUIRE(my_CSV.getRowByID(1).ASCII_chars=="!");
    REQUIRE(my_CSV.getRowByID(95).ASCII_code==127);
    REQUIRE(my_CSV.getRowByID(95).cumulativeXPos==2375);
    REQUIRE(my_CSV.getRowByID(95).ASCII_chars=="default");

    REQUIRE(my_CSV.getRowByASCII_Code(32).ASCII_code==32);
    REQUIRE(my_CSV.getRowByASCII_Code(32).cumulativeXPos==0);
    REQUIRE(my_CSV.getRowByASCII_Code(32).ASCII_chars=="SP");        
    REQUIRE(my_CSV.getRowByASCII_Code(33).ASCII_code==33);
    REQUIRE(my_CSV.getRowByASCII_Code(33).cumulativeXPos==25);
    REQUIRE(my_CSV.getRowByASCII_Code(33).ASCII_chars=="!");
    REQUIRE(my_CSV.getRowByASCII_Code(95).ASCII_code==95);
    REQUIRE(my_CSV.getRowByASCII_Code(95).cumulativeXPos==1575);
    REQUIRE(my_CSV.getRowByASCII_Code(95).ASCII_chars=="_");

    REQUIRE_THROWS(my_CSV.getRowByID(-1).ASCII_chars); // Out of Bounds
    REQUIRE_THROWS(my_CSV.getRowByASCII_Code(-1).ASCII_chars); // Out of Bounds
    REQUIRE_THROWS(my_CSV.getRowByID(96).ASCII_chars); // Out of Bounds
    REQUIRE_THROWS(my_CSV.getRowByASCII_Code(128).ASCII_chars); // Out of Bounds
}



TEST_CASE("Test Glyph Class Instantiation", "[font]"){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN);  //Steamdeck is 1280x800
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    std::shared_ptr<stbimageTexture> entire_font_texture(new stbimageTexture(renderer, "assets/stone_term.png"));
    SDL_Rect letter_A_Rect;
    letter_A_Rect.h=50;
    letter_A_Rect.w=25;
    letter_A_Rect.x=25;
    letter_A_Rect.y=0;
    std::shared_ptr<Glyph> Glyph_Exclamation(new Glyph(entire_font_texture, letter_A_Rect, 33,std::string("!")));
    REQUIRE(Glyph_Exclamation->getGlyphHeight()==50);
    REQUIRE(Glyph_Exclamation->getGlyphWidth()==25);
    REQUIRE(Glyph_Exclamation->getGlyphASCII_Code()==33);
    REQUIRE(Glyph_Exclamation->getGlyphASCII_Char()==std::string("!"));
    
    // Test the Glyph Scalar class which resized the rendering rectangle based on a provided pt value (12pt, 10pt, etc)
    GlyphRectScalar glyph_scalar;
    REQUIRE(glyph_scalar.getAdjGlyphHeight(12)==35);
    REQUIRE(glyph_scalar.getAdjGlyphWidth(Glyph_Exclamation,glyph_scalar.getAdjGlyphHeight(12))==17);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();    
}
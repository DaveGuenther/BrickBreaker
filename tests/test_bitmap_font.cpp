#include "../src/3rd_party/bitmap_font.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Test sample Sum function", "[default]"){
    int a = 5;
    int b = 10;
    REQUIRE(sum_nums(a, b)==15);
}

TEST_CASE("Test Glyph Class Instantiation", "[font]"){
    
}
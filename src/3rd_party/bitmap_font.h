/*
* bitmap_font 
*
* A small header only library that uses allows you to load long bitmap fonts (PNG file with companion CSV file that 
* details font information).  This library depends on several other header only libraries so that SDL2_image binaries are
* not required to run.  
*
* Created by: Dave Guenther 2022
*
* Dependencies:
*     libSDL2         http://www.libsdl.org
*     stb_image.h     https://github.com/nothings/stb
*     SDL_stbimage.h  https://github.com/DanielGibson/Snippets/
*
* If you are already using stb_image.h in your source code, please ensure that this line is added somewhere (and only once) 
* in your project source code files.
*
* #define SDL_STBIMAGE_IMPLEMENTATION
*
* If you are not using stb_image.h anywhere else in your source code and this header will be the only file it, then 
* uncomment the #define line above.
*
*
*
*/


#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "SDL_stbimage.h" // 

int sum_nums(int x, int y){
    return x+y;
}


class Glyph{
    public:
        Glyph(SDL_Surface this_glyph, int height, int width, int ascii_code, std::string ascii_char){
            loadGlyph(this_glyph, height, width, ascii_code, ascii_char);
        }
        void loadGlyph(SDL_Surface this_glyph, int height, int width, int ascii_code, std::string ascii_char){
            this->glyph_surface = this_glyph;
            this->height = height;
            this->width = width;
            this->ascii_code = ascii_code;
            this->ascii_char = ascii_char;
        }

        const SDL_Surface& getSurface(){
            return this->glyph_surface;
        }

        const std::string& getASCII_Char(){
            return this->ascii_char;
        }

        const int& getASCII_Code(){
            return this->ascii_code;
        }

        const int& getWidth(){
            return this->width;
        }

        const int& getHeight(){
            return this->height;
        }

    private:
        std::string ascii_char;
        int ascii_code;
        int width;
        int height;
        SDL_Surface glyph_surface;
};

class CSV_Object{
    public:
        CSV_Object(std::string csv_font_map_fname){
            this->CSV_Load(csv_font_map_fname);
        }

        void CSV_Load(std::string csv_font_map_fname){
            std::vector<std::string> row;
            std::string line, word;
            std::fstream csv_file (csv_font_map_fname, std::ios::in);       

            std::cout << "Object Loaded" << std::endl;

            if(csv_file.is_open())
            {
                while(getline(csv_file, line))
                {
                    row.clear();

                    std::stringstream str(line);

                    while(getline(str, word, ','))
                        row.push_back(word);
                    this->CSV_data.push_back(row);
                }
            }
            else
                throw std::runtime_error(std::string("Could not open file: " + csv_font_map_fname));
        }

        const std::vector<std::vector<std::string>>& getData(){
            return this->CSV_data;
        }

    private:
        std::vector<std::vector<std::string>> CSV_data;
};

class BitmapFont{
    public:
        BitmapFont(std::string bitmap_fname, std::string csv_font_map_fname):   bitmap_fname(bitmap_fname), 
                                                                                csv_font_map_fname(csv_font_map_fname),
                                                                                font_CSV(CSV_Object(csv_font_map_fname)){

            

            for(int i=0;i<font_CSV.getData().size();i++)
            {
                for(int j=0;j<font_CSV.getData().size();j++)
                {
                    std::cout<<font_CSV.getData()[i][j]<<" ";
                }
                std::cout<<"\n";
            }
            std::cout<<"CSV Loaded"<<std::endl;
        }

    private:
        std::string bitmap_fname;
        std::string csv_font_map_fname;
        CSV_Object font_CSV;

};


#endif
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
#include <map>
#include <memory>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "SDL_stbimage.h" // 

int sum_nums(int x, int y){
    return x+y;
}


class Glyph{
    public:
        Glyph(SDL_Surface* font_image_surface, SDL_Rect glyph_rect, int ascii_code, std::string ascii_char){
            loadGlyph(font_image_surface, glyph_rect, ascii_code, ascii_char);
        }
        ~Glyph(){
            SDL_FreeSurface(this->glyph_surface);
        }
        
        void loadGlyph(SDL_Surface* font_image_surface, SDL_Rect glyph_rect, int ascii_code, std::string ascii_char){
            this->glyph_rect = glyph_rect;
            this->glyph_surface = SDL_CreateRGBSurface(0,glyph_rect.w,glyph_rect.h,32,0,0,0,0);
            SDL_Rect dest_rect;
            dest_rect.x=0;
            dest_rect.y=0;
            dest_rect.w=glyph_rect.w;
            dest_rect.h=glyph_rect.h;
            SDL_BlitSurface(font_image_surface,&glyph_rect,this->glyph_surface,&dest_rect);
            this->height = height;
            this->width = width;
            this->ascii_code = ascii_code;
            this->ascii_char = ascii_char;
        }


        const SDL_Surface* getGlyphSurface(){
            return this->glyph_surface;
        }

        const SDL_Rect& getGlyphRect(){
            return this->glyph_rect;
        }

        const std::string& getGlyphASCII_Char(){
            return this->ascii_char;
        }

        const int& getGlyphASCII_Code(){
            return this->ascii_code;
        }

        const int& getGlyphWidth(){
            return this->width;
        }

        const int& getGlyphHeight(){
            return this->height;
        }

    private:
        std::string ascii_char;
        int ascii_code;
        int width;
        int height;
        SDL_Rect glyph_rect;
        SDL_Surface *glyph_surface;
};

class CSV_Object{
    public:
        CSV_Object(std::string csv_font_map_fname){
            this->CSV_Load(csv_font_map_fname);
            int i=0;
            for (std::string key:this->CSV_data[0])
            {
                this->column_names[key]=i;
                //this->column_names.insert({key,i});
                i++;
            }
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
                    this->rows++;
                    std::stringstream str(line);
                    this->columns=0;
                    while(getline(str, word, ','))
                        row.push_back(word);
                        this->columns++;
                    this->CSV_data.push_back(row);
                }
            }
            else
                throw std::runtime_error(std::string("Could not open file: " + csv_font_map_fname));
        }

        const std::vector<std::vector<std::string>>& getData(){
            return this->CSV_data;
        }

        int getNumRows(){
            return this->CSV_data.size()-1;
        }

        int getNumColumns(){
            return this->CSV_data[0].size();
        }

        auto at(std::string column_name, int row_num){
            int col_num = this->column_names.at(column_name);
            if (column_name=="ASCII_chr") {
                return std::string(this->CSV_data[row_num+1][col_num]);  // Add 1 to row_num because row 0 is the column name
            }
            else{
                return this->CSV_data[row_num+1][col_num]; // Add 1 to row_num because row 0 is the column name
            }
        }

    private:
        std::vector<std::vector<std::string>> CSV_data;
        std::map<std::string, int> column_names;
        int rows=0;
        int columns=0;
};

class BitmapFont{
    public:
        BitmapFont(std::string bitmap_fname, std::string csv_font_map_fname):   bitmap_fname(bitmap_fname), 
                                                                                csv_font_map_fname(csv_font_map_fname),
                                                                                font_CSV(CSV_Object(csv_font_map_fname)){

            

            for(int i=0;i<font_CSV.getNumRows();i++)
            {
                for(int j=0;j<font_CSV.getNumColumns();j++) // Find a way to access number of columns, don't hardcode to 3 forever
                {
                    std::cout<<font_CSV.getData()[i][j]<<" ";
                }
                std::cout<<"\n";
            }
            std::cout<<"CSV Loaded"<<std::endl;
            std::cout << font_CSV.at("Decimal",3) << std::endl;
        }

    private:
        std::string bitmap_fname;
        std::string csv_font_map_fname;
        CSV_Object font_CSV;

};


#endif
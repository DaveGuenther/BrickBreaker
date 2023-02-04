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

typedef struct Row{
    int ASCII_code;
    std::string ASCII_chars;
    int cumulativeXPos;        
    int width;
}Row;

class CSV_Object{
    public:
        CSV_Object(std::string csv_font_map_fname){
            this->CSV_Load(csv_font_map_fname);

        }

        void CSV_Load(std::string csv_font_map_fname){

            std::string line, str_ascii_code, str_ascii_char, str_glyph_width;
            std::fstream csv_file (csv_font_map_fname, std::ios::in);       
            this->rows=0;
            this->ascii_codes.clear();
            std::cout << "Object Loaded" << std::endl;
            int ascii_code =0;
            int cumulative_xPos=0;
            int width=0;
            if(csv_file.is_open())
            {
                // Get first line of column names and discard
                if (getline(csv_file, line)){
                
                }
                
                // Get remaining lines
                while(getline(csv_file, line))
                {
                    //Each row should be formatted as follows:
                    //  {int ascii_code, std::string ascii_char, int glyph_width_in_pixels}
                    this->rows++;
                    std::stringstream ss(line);

                    if(getline(ss, str_ascii_code, ',')){
                        // first token is the ascii code of the character
                        ascii_code=std::stoi(str_ascii_code);
                        this->ascii_codes.push_back(ascii_code);
                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+ ": at row "+std::to_string(rows-1)+ ". Expected int for ASCII_code column."));
                    }

                    if(getline(ss,str_ascii_char,',')){
                        // second token is the ascii character (or description of character)
                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+": at row "+std::to_string(rows-1)+ ". Expected string for ASCII_char column."));
                    }

                    if(getline(ss,str_glyph_width,',')){
                        // This last token is a width in pixels of the glyph
                        width = std::stoi(str_glyph_width);
                        cumulative_xPos+=width;
                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+ ": at row "+std::to_string(rows-1)+ ". Expected int for Width column."));
                    }
                    this->row_data.insert({ascii_code,{ascii_code,str_ascii_char,cumulative_xPos,width}});
                }
            }
            else
                throw std::runtime_error(std::string("Could not open file: " + csv_font_map_fname));
        }

        int getNumRows(){
            return this->rows;
        }
        
        /**
         * @brief Get the Row By I D object.  This provides a Zero indexed accessor to CSV row data.
         * 
         * @param id 
         * @return Row 
         */
        Row getRowByID(int id){
            return this->row_data[this->ascii_codes[id]];
        }

        /**
         * @brief Get the RowByASCII Code object.  This provides an ASCII_Code indexed accessor to CSV row data.
         * 
         * @param ascii_code 
         * @return Row 
         */
        Row getRowByASCII_Code(int ascii_code){
            return this->row_data[ascii_code];
        }

    private:

        std::vector<int> ascii_codes;
        std::map<int, Row> row_data;
        int rows=0;
};

class BitmapFont{
    public:
        BitmapFont(std::string bitmap_fname, std::string csv_font_map_fname):   bitmap_fname(bitmap_fname), 
                                                                                csv_font_map_fname(csv_font_map_fname),
                                                                                font_CSV(CSV_Object(csv_font_map_fname)){

            

            for(int i=0;i<font_CSV.getNumRows();i++)
            {
                int ascii_code = font_CSV.getRowByID(i).ASCII_code;
                std::cout << ascii_code << ", " << font_CSV.getRowByASCII_Code(ascii_code).ASCII_chars << ", " << font_CSV.getRowByASCII_Code(ascii_code).width << std::endl;
            }
            std::cout<<"CSV Loaded"<<std::endl;
        }

    private:
        std::string bitmap_fname;
        std::string csv_font_map_fname;
        CSV_Object font_CSV;

};


#endif
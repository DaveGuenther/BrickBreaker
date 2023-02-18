/*
* bitmap_font 
*
* A small header only library that uses allows you to load long bitmap fonts (PNG file with companion CSV file that 
* details font information).  This library depends on several other header only libraries so that SDL2_image binaries are
* not required to run.  
*
* Created by: Dave Guenther 2023
*
* Dependencies:
*     libSDL2         http://www.libsdl.org
*     stb_image.h     https://github.com/nothings/stb
*     SDL_stbimage.h  https://github.com/DanielGibson/Snippets/
*
* If you are already using stb_image.h in your source code, please ensure that this line is added somewhere (and only once) 
* in your project source code files.  Your root .cpp (main.cpp) is a good option
*
* #define SDL_STBIMAGE_IMPLEMENTATION
*
* If you are not using stb_image.h anywhere else in your source code and this header will be the only file it, then 
* uncomment the #define line above.
*
* Using this header requires that you establish a renderer context and pass that into the BitmapFont class.  It won't work with rendering
* approaches using SDL_UpdateWindowSurface(window);
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

class stbimageTexture{
    public:
        stbimageTexture(SDL_Renderer* renderer, const char* imageFilePath){
            this->renderer = renderer;
            this->loadTexture(imageFilePath);
        }
        ~stbimageTexture(){
            SDL_FreeSurface(this->image);
            SDL_DestroyTexture(this->textureImage);
        }

        void loadTexture(const char* imageFilePath){
            this->image = STBIMG_Load(imageFilePath);
            if(image == NULL) {
            printf("ERROR: Couldn't load %s, reason: %s\n", imageFilePath, SDL_GetError());
            exit(1);
            }
            this->textureImage = SDL_CreateTextureFromSurface(this->renderer, this->image);
        }

        SDL_Surface* getSurface(){
            return this->image;
        }

        SDL_Texture* getTexture(){
            return this->textureImage;
        }

        SDL_Renderer* getRenderer(){
            return this->renderer;
        }

    private:
        SDL_Renderer *renderer;
        SDL_Surface *image;
        SDL_Texture *textureImage;

};

class Glyph{
    public:
        Glyph(){}
        Glyph(std::shared_ptr<stbimageTexture> font_image_texture, SDL_Rect glyph_rect, int ascii_code, std::string ascii_char){
            loadGlyph(font_image_texture, glyph_rect, ascii_code, ascii_char);
        }
        ~Glyph(){
            //SDL_DestroyTexture(this->glyph_texture);

        }
        
        void loadGlyph(std::shared_ptr<stbimageTexture> font_image_texture, SDL_Rect glyph_rect, int ascii_code, std::string ascii_char){
            this->glyph_rect = glyph_rect;
            this->entire_font_texture = font_image_texture->getTexture();
            this->height = glyph_rect.h;
            this->width = glyph_rect.w;
            this->ascii_code = ascii_code;
            this->ascii_char = ascii_char;
                     
        }

        SDL_Texture* getFontTexture(){
            return this->entire_font_texture;
        }

        SDL_Rect* getGlyphOffsetRect(){
            return &(this->glyph_rect);
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
        SDL_Texture* entire_font_texture;
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

                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+ ": at row "+std::to_string(rows-1)+ ". Expected int for Width column."));
                    }
                    this->row_data.insert({ascii_code,{ascii_code,str_ascii_char,cumulative_xPos,width}});
                    cumulative_xPos+=width;                
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
            if ((id>=0)&&(id<96)){            
                return this->row_data[this->ascii_codes[id]];
            }else{
                throw std::out_of_range(std::string("Row index out of range.  id must be [0-95]"));
            }                
        }

        /**
         * @brief Get the RowByASCII Code object.  This provides an ASCII_Code indexed accessor to CSV row data.
         * 
         * @param ascii_code 
         * @return Row 
         */
        Row getRowByASCII_Code(int ascii_code){
            if ((ascii_code>=32)&&(ascii_code<128)){
                return this->row_data[ascii_code];
            }else{
                throw std::out_of_range(std::string("ASCII Code out of range.  id must be [32-127]"));
            }
        }

    private:

        std::vector<int> ascii_codes;
        std::map<int, Row> row_data;
        int rows=0;
};


class BitmapFont{
    public:
        BitmapFont(SDL_Renderer* renderer, std::string bitmap_fname, std::string csv_font_map_fname, int height_px):   
                                                                                renderer(renderer),
                                                                                bitmap_fname(bitmap_fname), 
                                                                                csv_font_map_fname(csv_font_map_fname),
                                                                                font_CSV(CSV_Object(csv_font_map_fname)),
                                                                                glyph_height_in_pixels(height_px){


            loadGlyphs();
            std::cout<<"CSV Loaded"<<std::endl;
        }

        void loadGlyphs(){

            this->font_image = std::shared_ptr<stbimageTexture>(new stbimageTexture(this->renderer, this->bitmap_fname.c_str()));
            const char* my_c_str = this->bitmap_fname.c_str();
            for(int i=0;i<this->font_CSV.getNumRows();i++)
            {
                int ascii_code = font_CSV.getRowByID(i).ASCII_code;
                
                
                SDL_Rect glyphRect;
                glyphRect.x=font_CSV.getRowByASCII_Code(ascii_code).cumulativeXPos;
                glyphRect.y=0;
                glyphRect.h=this->glyph_height_in_pixels;
                glyphRect.w=font_CSV.getRowByASCII_Code(ascii_code).width;

                std::shared_ptr<Glyph> temp_Glyph(new Glyph(font_image, glyphRect, ascii_code,font_CSV.getRowByASCII_Code(ascii_code).ASCII_chars));                   
                std::cout << ascii_code << ", " << font_CSV.getRowByASCII_Code(ascii_code).ASCII_chars << ", " << font_CSV.getRowByASCII_Code(ascii_code).width << std::endl;
                alphabetGlyphs.insert({ascii_code,temp_Glyph});

            }            
        }

        void placeCharAtXY(int x, int y, int ascii_code){
            std::shared_ptr<Glyph> tempGlyph=this->alphabetGlyphs[ascii_code];
            SDL_Rect* glyphRect = tempGlyph->getGlyphOffsetRect();
            SDL_Rect temp_rect;
            temp_rect.x=x;
            temp_rect.y=y;
            temp_rect.h=tempGlyph->getGlyphHeight();
            temp_rect.w=tempGlyph->getGlyphWidth();
            SDL_RenderCopy(renderer, tempGlyph->getFontTexture(),glyphRect,&temp_rect);

        }

    private:
        std::map<int,std::shared_ptr<Glyph>> alphabetGlyphs;
        int glyph_height_in_pixels;
        SDL_Renderer* renderer;
        std::string bitmap_fname;
        std::string csv_font_map_fname;
        std::shared_ptr<stbimageTexture> font_image;
        CSV_Object font_CSV;

};


#endif
//
// If you are already using stb_image.h in your source code, please ensure that this line is added somewhere (and only once) in your project source code files.
//
// #define SDL_STBIMAGE_IMPLEMENTATION
//
// If you are not using stb_image.h anywhere else in your source code and this header will be the only file it, then uncomment the line above.
//
//
//

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

#include "SDL_stbimage.h"

int sum_nums(int x, int y){
    return x+y;
}

/*
class Glyph{
    public:
        Glyph()
};*/

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
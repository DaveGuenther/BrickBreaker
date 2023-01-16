//
// If you are already using stb_image.h in your source code, please ensure that this line is added somewhere (and only once) in your source code.
//
// #define SDL_STBIMAGE_IMPLEMENTATION
//
// If you are not using stb_image.h anywhere else in your source code and this header will be the only csv_font_map_fname using it, then uncomment the line above.
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

#include <SDL2/SDL.h>

#include "SDL_stbimage.h"





class BitmapFont{
    public:
        BitmapFont(std::string bitmap_fname, std::string csv_font_map_fname){
            std::vector<std::vector<std::string>> content;
            std::vector<std::string> row;
            std::string line, word;
            std::fstream csv_file (csv_font_map_fname, std::ios::in);

            
            if(csv_file.is_open())
            {
                while(getline(csv_file, line))
                {
                    row.clear();

                    std::stringstream str(line);

                    while(getline(str, word, ','))
                        row.push_back(word);
                    content.push_back(row);
                }
            }
            else
                std::cout<<"Could not open the csv_font_map_fname\n";

            for(int i=0;i<content.size();i++)
            {
                for(int j=0;j<content[i].size();j++)
                {
                    std::cout<<content[i][j]<<" ";
                }
                std::cout<<"\n";
            }
        }

    private:


};


#endif
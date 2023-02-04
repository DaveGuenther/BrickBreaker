//Core includes
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

//SDL related
#include <SDL2/SDL.h>




int main(int argv, char** args)
{
    std::string line="ASCII_code,ASCII_char,Width";
    std::string word;
    std::stringstream ss(line);
    while(getline(ss, word, ',')){
        if (word!="ASCII_code"){
            std::cout << "String Match!" << std::endl;
        }else{
            std::cout << "Strings do NOT match" << std::endl;
        }
    }



	return 0;
}
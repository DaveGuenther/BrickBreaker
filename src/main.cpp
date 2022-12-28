#include "preprocessor.cpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <SDL2/SDL.h>
//#include "core/Game_Loop.h"

#if defined(PREDEF_PLATFORM_UNIX)
	#include "core/Game_Loop.h"
#endif

#if defined(PREDEF_PLATFORM_WINDOWS)
	#include "core/Game_Loop.h"
#endif

int main(int argv, char** args)
{
	std::ifstream infile("../../profile.sh");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::cout << line.c_str() << std::endl;
	}
	std::cout<<"Hello SDL World!" << std::endl;
	Game_Loop my_Game_Loop;
	while(my_Game_Loop.isRunning()){
		my_Game_Loop.update();
	}
	
	my_Game_Loop.shutDown();


	std::cout << "Goodbye!" << std::endl;

	return 0;
}
//Core includes
#include <iostream>

//SDL related
#include <SDL2/SDL.h>


//Game related
#include "preprocessor.cpp"  // helps with cross-platform related identification
#include "core/Game_Loop.h"

#if defined(PREDEF_PLATFORM_UNIX)
	//Linux Specific preprocessing here, such as include paths if needed
#endif

#if defined(PREDEF_PLATFORM_WINDOWS)
	//Windows specific preprocessing here, such as include paths if needed
#endif

int main(int argv, char** args)
{

	std::cout<<"Hello SDL World!" << std::endl;
	Game_Loop my_Game_Loop;
	while(my_Game_Loop.isRunning()){
		//std::cout << "Starting Main Loop" << std::endl;
		my_Game_Loop.update();
	}
	
	my_Game_Loop.shutDown();


	std::cout << "Goodbye!" << std::endl;

	return 0;
}
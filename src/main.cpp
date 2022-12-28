#include "preprocessor.cpp"

#include <iostream>


#include <SDL2/SDL.h>
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
		my_Game_Loop.update();
	}
	
	my_Game_Loop.shutDown();


	std::cout << "Goodbye!" << std::endl;

	return 0;
}
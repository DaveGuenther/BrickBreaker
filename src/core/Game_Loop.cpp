#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include "../3rd_party/SDL_stbimage.h" // Used for loading .png files as surface
#include "Game_Loop.h"

Game_Loop::Game_Loop(){
	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_FULLSCREEN);
	//this->renderer = SDL_CreateRenderer(window, -1, 0);
    this->screen = SDL_GetWindowSurface(this->window);
    
	this->running = true;
	if(SDL_NumJoysticks() <1)
    {
        std::cout<<"No Joysticks Found"<<std::endl;
    }
    else
    {
        gGameController = SDL_JoystickOpen(0);
        if (gGameController == NULL)
        {
            std::cout << "Warning: Unable to open game controller!  SDL Error: " << SDL_GetError() << std::endl;
            
        }
    }
}

SDL_Surface* yourFunction(const char* imageFilePath)
{
SDL_Surface* surf = STBIMG_Load(imageFilePath);
if(surf == NULL) {
    printf("ERROR: Couldn't load %s, reason: %s\n", imageFilePath, SDL_GetError());
    exit(1);
}

// ... do something with surf ...

SDL_FreeSurface(surf);
}

bool Game_Loop::isRunning(){
    return this->running;
}

void Game_Loop::update(){
    //std::cout << "Polling for events" << std::endl;
    while (SDL_PollEvent(&this->event))
            {
                //std::cout << "Event Detected" << std::endl;
                switch (this->event.type)
                {
                case SDL_QUIT:
                    this->running = false;
                    break;

                case SDL_JOYBUTTONDOWN:
                    std::cout<<"Joystick Event from ID: " << this->event.jbutton.which << " and the button pressed is: " << this->event.jbutton.button << std::endl;
                    break;

                case SDL_KEYDOWN:
                    if (this->event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        this->running = false;
                    }
                }
            }

            //std::cout << "Clearing Renderer" << std::endl;
            //SDL_RenderClear(this->renderer);
            //std::cout << "setting color red" << std::endl;
            //SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
            //std::cout << "flip frame buffer" << std::endl;
            //SDL_RenderPresent(this->renderer);
}

void Game_Loop::shutDown(){


	//SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
Game_Loop::~Game_Loop(){

}
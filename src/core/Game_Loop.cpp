#include <SDL2/SDL_surface.h>
#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>


#include "Game_Loop.h"
#include "../texture/Texture.h"

Game_Loop::Game_Loop(){
	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN);
	//this->renderer = SDL_CreateRenderer(window, -1, 0);
    this->screen = SDL_GetWindowSurface(this->window);
    std::unique_ptr<Texture> temp_texture(new Texture("assets/sdl_logo.png"));
    this->png_image=std::move(temp_texture);
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
            SDL_BlitSurface(this->png_image->getSurface(),NULL,screen,NULL);
            //std::cout << "Clearing Renderer" << std::endl;
            //SDL_RenderClear(this->renderer);
            //std::cout << "setting color red" << std::endl;
            //SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
            //std::cout << "flip frame buffer" << std::endl;
            //SDL_RenderPresent(this->renderer);
            SDL_UpdateWindowSurface(window);
}

void Game_Loop::shutDown(){


	//SDL_DestroyRenderer(this->renderer);

	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
Game_Loop::~Game_Loop(){

}
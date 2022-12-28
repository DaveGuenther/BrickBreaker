#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include <memory>

#include "../core/Game_Loop.h"

Game_Loop::Game_Loop(){
	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	this->renderer = SDL_CreateRenderer(window, -1, 0);


	this->running = true;
	
}

bool Game_Loop::isRunning(){
    return this->running;
}

void Game_Loop::update(){
    while (SDL_PollEvent(&this->event))
            {
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

            SDL_RenderClear(this->renderer);
            SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);

            SDL_RenderPresent(this->renderer);
}

void Game_Loop::shutDown(){


	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
Game_Loop::~Game_Loop(){

}
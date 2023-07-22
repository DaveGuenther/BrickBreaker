
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>


#include "Game_Loop.h"
//#include "../texture/Texture.h"


Game_Loop::Game_Loop(){
	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN);  //Steamdeck is 1280x800
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    this->png_image=std::shared_ptr<stbimageTexture>(new stbimageTexture(this->renderer, "assets/sdl_logo.png"));

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

    myFont = std::shared_ptr<BitmapFont>(new BitmapFont(this->renderer, "assets/stone_term.png", "assets/stone_term.csv",50));



   
}

Game_Loop::~Game_Loop(){
   
}

bool Game_Loop::isRunning(){
    return this->running;
}

void Game_Loop::update(){
    SDL_RenderClear(renderer);
    while (SDL_PollEvent(&this->event)){
        //std::cout << "Event Detected" << std::endl;
        std::cout<<this->event.type<<std::endl;
        switch (this->event.type)
        {
        case SDL_QUIT:
            this->running = false;
            break;

        case SDL_JOYBUTTONDOWN:
            std::cout<<"Joystick Event from ID: " << this->event.jbutton.which << " and the jbutton pressed is: " << this->event.jbutton.button << " or button: " <<this->event.button.which<< std::endl;
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            std::cout<<"Controller Event" << std::endl;
            break; 

        case SDL_JOYAXISMOTION:
            std::cout<<"Joy Axis Motion Event" << this->event.jaxis.which << " and the jbutton pressed is: " << this->event.jaxis.axis << " or button: " <<this->event.motion.which<< " or x:" << this->event.motion.x<<" y:"<< this->event.motion.y<< std::endl;
            break; 

        case SDL_JOYBALLMOTION:
            std::cout<<"Joy ball Motion Event" << std::endl;
            break; 
        
        case SDL_JOYHATMOTION:
            std::cout<<"Joy hat Motion Event" << this->event.jhat.which << " and the jbutton pressed is: " << this->event.jhat.hat << " or button: " <<this->event.motion.which<< std::endl;
            break; 



        
        case SDL_KEYDOWN:
            if (this->event.key.keysym.sym == SDLK_ESCAPE)
            {
                this->running = false;
            }
        }
    }
    SDL_RenderCopy(renderer,this->png_image->getTexture(),NULL,NULL);
    this->myFont->placeStringAtXY("Brick Breaker!",50, 300, 60);
    this->myFont->placeWordWrappedStringAtXY("The quick brown fox jumps over the lazy dog!",100,100,12,300,"left");
    this->myFont->placeWordWrappedStringAtXY("The quick brown fox jumps over the lazy dog!",400,200,10,300,"right");
    this->myFont->placeWordWrappedStringAtXY("The quick brown fox jumps over the lazy dog!",50,450,14,500,"center");
    SDL_RenderPresent(renderer);


}

void Game_Loop::shutDown(){

    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
	SDL_Quit(); 

    std::cout << "Shutting Down from GameLoop" << std::endl;
}

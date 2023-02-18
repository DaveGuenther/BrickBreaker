#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <memory>
#include <SDL2/SDL.h>


#include "../3rd_party/bitmap_font.h"

class Game_Loop{
    public:
        Game_Loop(void);
        ~Game_Loop();

        bool isRunning();

        void update();

        void shutDown();
    
    private:
        // SDL Objects
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Event event;
        std::shared_ptr<stbimageTexture> png_image;
        std::shared_ptr<BitmapFont> myFont;
        bool running; //If set to false, this will end the 3D engine during the next engine_update()
        const int JOYSTICK_DEAD_ZONE = 8000;
        SDL_Joystick* gGameController = NULL;
};
#endif
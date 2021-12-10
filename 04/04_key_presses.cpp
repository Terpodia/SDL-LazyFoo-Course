#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
string mediaNames[5] = {"press.bmp", "up.bmp", "down.bmp", "right.bmp", "left.bmp"};

enum KeyPressSurfaces{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* window;
SDL_Surface* screenSurface;
SDL_Surface* keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; 

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);    
    
    screenSurface = SDL_GetWindowSurface(window);
}

void loadMedia(int id){
    keyPressSurfaces[id] = SDL_LoadBMP(mediaNames[id].c_str());
}

void exit(){
    for(int i=0; i<KEY_PRESS_SURFACE_TOTAL; i++){
        SDL_FreeSurface(keyPressSurfaces[i]);
        keyPressSurfaces[i] = nullptr;
    }    
    SDL_FreeSurface(screenSurface);
    screenSurface = nullptr;
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]){
    init();
    for(int i=0; i<KEY_PRESS_SURFACE_TOTAL; i++)
        loadMedia(i);        
    
    SDL_Surface* current = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    SDL_BlitSurface(current, nullptr, screenSurface, nullptr);
    SDL_UpdateWindowSurface(window);

    SDL_Event e;

    bool quit = false;

    while(!quit){
        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_QUIT)
                quit = true;
            
            else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                    current = keyPressSurfaces[KEY_PRESS_SURFACE_UP];
                    break;

                    case SDLK_DOWN:
                    current = keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                    break;

                    case SDLK_RIGHT:
                    current = keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                    break;

                    case SDLK_LEFT:
                    current = keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                    break;

                    default:
                    current = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                    break;
                }
            }
            SDL_BlitSurface(current, nullptr, screenSurface, nullptr);
            SDL_UpdateWindowSurface(window);
        }
    }

    SDL_Quit();

    return 0;
}
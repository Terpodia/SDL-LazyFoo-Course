#include <SDL2/SDL.h>
#include <cstdio>

const int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 630; 

SDL_Window* window = nullptr;
SDL_Surface* screenSurface = nullptr; 
SDL_Surface* helloWorld = nullptr;

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_error: %s\n", SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);        

    if(window == nullptr){
        printf("Window could not be created! SDL_error: %s\n", SDL_GetError());
        return false;
    }
    
    screenSurface = SDL_GetWindowSurface(window);

    return true;
}

bool loadMedia(){
    helloWorld = SDL_LoadBMP("hello-world.bmp");
    if(helloWorld == nullptr){
        printf("Unable to load image :( SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void close(){
    SDL_FreeSurface(helloWorld);
    helloWorld = nullptr;

    SDL_FreeSurface(screenSurface);
    screenSurface = nullptr; 

    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

int main(int argc, char* args[]){
    if(!init()) printf("Failed to initialize!\n");
    else if(!loadMedia()) printf("Failed to load media!\n");
    else{
        SDL_BlitSurface(helloWorld, nullptr, screenSurface, nullptr);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(3000);
    }

    close();

    return 0;
}
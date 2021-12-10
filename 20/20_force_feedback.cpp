#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* tex = nullptr;

SDL_Joystick* controller = nullptr;
SDL_Haptic* controllerHaptic = nullptr;

void init(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    IMG_Init(IMG_INIT_PNG);
    if(SDL_NumJoysticks() < 1)
        printf("WARNING: No controller connected\n");

    else{
        controller = SDL_JoystickOpen(0); 
        controllerHaptic = SDL_HapticOpenFromJoystick(controller);
        if(SDL_HapticRumbleInit(controllerHaptic) < 0)
            printf("WARNING: Unnable to initialize rumble :(\n");
    }
}

void loadMedia(){
    SDL_Surface* surface = IMG_Load("splash.png");
    SDL_FreeSurface(surface); surface = nullptr;
    tex = SDL_CreateTextureFromSurface(renderer, surface);
}

void exit(){
    SDL_DestroyTexture(tex); tex = nullptr;
    SDL_HapticClose(controllerHaptic); controllerHaptic = nullptr;
    SDL_JoystickClose(controller); controller = nullptr;

    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); window = nullptr;

    SDL_Quit(); IMG_Quit();
}

int main(int argc, char* args[]){
    init(); loadMedia();

    bool quit = false;
    SDL_Event e;

    while(!quit){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tex, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        
        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_QUIT) quit = true;

            else if(e.type == SDL_JOYBUTTONDOWN){
                if(controller != nullptr && controllerHaptic != nullptr)
                    SDL_HapticRumblePlay(controllerHaptic, 0.70, 500);
            }
        }
    }

    exit();
    return 0;
}

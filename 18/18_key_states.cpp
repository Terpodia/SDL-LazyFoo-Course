#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

const int WIDTH=640, HEIGHT = 480;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* textures[5];

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void loadMedia(string path, int index){
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    textures[index] = SDL_CreateTextureFromSurface(renderer, surface); 
    SDL_FreeSurface(surface); surface = nullptr;
}

void exit(){
    for(int i=0; i<5; i++){
        SDL_DestroyTexture(textures[i]); textures[i] = nullptr;
    }
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); window = nullptr;
    SDL_Quit();
}

int main(int argc, char* args[]){
    init();
    loadMedia("press.bmp", 0); loadMedia("up.bmp", 1); loadMedia("down.bmp", 2); loadMedia("right.bmp", 3); loadMedia("left.bmp", 4);

    SDL_Texture* currentTexture = textures[0];
    SDL_Event event;
    bool quit = false;

    while(!quit){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, currentTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT) quit = true;
            else{
                const Uint8* currentStateKey = SDL_GetKeyboardState(nullptr);
                if(*(currentStateKey + SDL_SCANCODE_UP)) currentTexture = textures[1];
                else if(*(currentStateKey + SDL_SCANCODE_DOWN)) currentTexture = textures[2];
                else if(*(currentStateKey + SDL_SCANCODE_RIGHT)) currentTexture = textures[3];
                else if(*(currentStateKey + SDL_SCANCODE_LEFT)) currentTexture = textures[4];
            }
        }
    }

    exit();
    return 0;
}
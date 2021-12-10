#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
using namespace std;

const int WIDTH = 640, HEIGHT = 480;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

Mix_Music* music;
Mix_Chunk* scratch;
Mix_Chunk* high;
Mix_Chunk* low;
Mix_Chunk* medium;

void init(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
}

void loadMedia(){
    SDL_Surface* surface = IMG_Load("prompt.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;

    music = Mix_LoadMUS("beat.wav");
    scratch = Mix_LoadWAV("scratch.wav");
    high = Mix_LoadWAV("high.wav");
    low = Mix_LoadWAV("low.wav");
    medium = Mix_LoadWAV("medium.wav");
}

void exit(){
    Mix_FreeMusic(music);
    Mix_FreeChunk(scratch);
    Mix_FreeChunk(high);
    Mix_FreeChunk(low);
    Mix_FreeChunk(medium);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    music = nullptr;
    scratch = high = low = medium = nullptr;
    renderer = nullptr; window = nullptr;

    SDL_Quit(); IMG_Quit(); Mix_Quit();
}

int main(){
    init(); loadMedia();
    SDL_Event e;
    bool quit = false;

    while(!quit){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_KEYDOWN){
                const Uint8* key = SDL_GetKeyboardState(nullptr);
                if(*(key+SDL_SCANCODE_1)) Mix_PlayChannel(-1, scratch, 0);
                else if(*(key+SDL_SCANCODE_2)) Mix_PlayChannel(-1, medium, 0);
                else if(*(key+SDL_SCANCODE_3)) Mix_PlayChannel(-1, low, 0);
                else if(*(key+SDL_SCANCODE_4)) Mix_PlayChannel(-1, high, 0);
                else if(*(key+SDL_SCANCODE_9)){
                    if(Mix_PlayingMusic() == 0) Mix_PlayMusic(music, -1);
                    else{
                        if(Mix_PausedMusic() == 1) Mix_ResumeMusic();
                        else Mix_PauseMusic();
                    }        
                } 
                else if(*(key+SDL_SCANCODE_0)) Mix_HaltMusic();
            }
        }
    }

    exit();
    return 0;
}
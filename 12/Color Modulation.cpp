#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void free();
        void loadFromFile(string path);
        void render(int x, int y, SDL_Rect* src = nullptr);
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window;
SDL_Renderer* renderer;
LTexture texture;

void init();
void loadMedia();
void exit();

int main(int argc, char* args[]){
    init();
    loadMedia();
    
    bool quit = false;
    SDL_Event event;

    Uint8 r, g, b;
    r = g = b = 255;

    while(!quit){
        SDL_RenderClear(renderer);
        texture.setColor(r, g, b);
        texture.render(0, 0);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT)
                quit = true;

            else if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_q:
                        r += 32; break;

                    case SDLK_w:
                        g += 32; break;

                    case SDLK_e:
                        b += 32; break;

                    case SDLK_a:
                        r -= 32; break;

                    case SDLK_s:
                        g -= 32; break;

                    case SDLK_d:
                        b -= 32; break;
                }
            }
        }
    }
    
    exit();
    return 0;
}

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    IMG_Init(IMG_INIT_PNG);
} 

void loadMedia(){
    texture.loadFromFile("colors.png");
}

void exit(){
    texture.free();
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
    IMG_Quit();
}

LTexture::LTexture(){
    mWidth = mHeight = 0;
    mTexture = nullptr;
}

void LTexture::loadFromFile(string path){
    SDL_Surface* surface = IMG_Load(path.c_str());     
    mWidth = surface->w; mHeight = surface->h;
    mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
}

void LTexture::render(int x, int y, SDL_Rect* src){
    SDL_Rect rect = {x, y, mWidth, mHeight};
    if(src != nullptr){
        rect.w = src->w; rect.h = src->h;
    }
    SDL_RenderCopy(renderer, mTexture, src, &rect);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::free(){
    mWidth = mHeight = 0;
    if(mTexture != nullptr){
        SDL_DestroyTexture(mTexture); 
    }
}

LTexture::~LTexture(){
    free();
}


#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>
using namespace std;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void loadFromFile(string path);
        void render(int x, int y, SDL_Rect* clip = nullptr, float angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE); 
        void free();
        int getWidth();
        int getHeight();
    private:
        int w;
        int h;
        SDL_Texture* texture;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window;
SDL_Renderer* renderer;
LTexture arrow;

void init();
void loadMedia();
void exit();

int main(int argc, char* args[]){
    init(); loadMedia();
    
    SDL_Event event;
    float degrees = 0.0;
    bool quit = false;
    SDL_RendererFlip flip = SDL_FLIP_NONE; 

    while(!quit){
        SDL_RenderClear(renderer);
        arrow.render((WIDTH - arrow.getWidth())/2, (HEIGHT - arrow.getHeight())/2, nullptr, degrees, nullptr, flip); 
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT) quit = true; 
            else if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_a:
                        degrees -= 60; break;

                    case SDLK_d:
                        degrees += 60; break;

                    case SDLK_q:
                        flip = SDL_FLIP_HORIZONTAL; break;

                    case SDLK_w:
                        flip = SDL_FLIP_NONE; break;

                    case SDLK_e:
                        flip = SDL_FLIP_VERTICAL; break;
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    IMG_Init(IMG_INIT_PNG);
}

void loadMedia(){
    arrow.loadFromFile("arrow.png");
}

void exit(){
    arrow.free();
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); window = nullptr;
    SDL_Quit();
    IMG_Quit();
}

LTexture::LTexture(){
    w = h = 0;
    texture = nullptr;
}

LTexture::~LTexture(){
    free();
}

void LTexture::free(){
    if(texture != nullptr){
        w = h = 0;
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void LTexture::loadFromFile(string path){
    SDL_Surface* surface = IMG_Load(path.c_str());
    w = surface->w; h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;
}

void LTexture::render(int x, int y, SDL_Rect* clip, float angle, SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect rect = {x, y, w, h};
    if(clip != nullptr){
        rect.w = clip->w; rect.h = clip->h;
    }
    SDL_RenderCopyEx(renderer, texture, clip, &rect, angle, center, flip);
}

int LTexture::getWidth(){
    return w;
}

int LTexture::getHeight(){
    return h;
}

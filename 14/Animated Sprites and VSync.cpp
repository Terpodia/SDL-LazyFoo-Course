#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>
using namespace std;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void loadFromFile(string path);
        void render(int x, int y, SDL_Rect* clip = nullptr);
        void free();

    private:
        int w;
        int h;
        SDL_Texture* t;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect animationClips[4];
LTexture foo;

void init();
void loadMedia();
void exit();

int main(int argc, char* args[]){
    init(); loadMedia();
    
    int frame = 0;
    SDL_Event e;
    bool quit = false;

    while(!quit){
        frame %= 16;
        SDL_RenderClear(renderer);
        foo.render(WIDTH/2, HEIGHT/2, animationClips+(frame/4));
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&e) > 0)
            if(e.type == SDL_QUIT) quit = true;

        frame++;
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
    foo.loadFromFile("foo.png");
    animationClips[0] = {0, 0, 64, 205};
    animationClips[1] = {64, 0, 64, 205};
    animationClips[2] = {128, 0, 64, 205};
    animationClips[3] = {192, 0, 64, 205};
}

void exit(){
    foo.free();
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); window = nullptr;
    SDL_Quit(); IMG_Quit();
}

LTexture::LTexture(){
    w = h = 0;
    t = nullptr;
}

void LTexture::loadFromFile(string path){
    SDL_Surface* surface = IMG_Load(path.c_str()); 
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));

    w = surface->w; h = surface->h;
    t = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    surface = nullptr;
}

void LTexture::render(int x, int y, SDL_Rect* clip){
    SDL_Rect rect = {x, y, w, h};
    if(clip != nullptr){
        rect.w = clip->w; rect.h = clip->h;
    }
    SDL_RenderCopy(renderer, t, clip, &rect);
}

void LTexture::free(){
    w = h = 0;
    if(t != nullptr){
        SDL_DestroyTexture(t); t = nullptr;
    }
}

LTexture::~LTexture(){
    free();
}


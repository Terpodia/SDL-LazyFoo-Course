#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <iostream>

const int WIDTH = 640, HEIGHT = 480;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void loadFromFile(std::string path);
        void free();
        void render(int x, int y);

    private:
        int mWidth;
        int mHeight;
        SDL_Texture* mTexture;
};

SDL_Window* window;
SDL_Renderer* renderer;
LTexture fooTexture, backgroundTexture;

void init();
void loadMedia();
void exit();

int main(int argc, char* args[]){
    init();
    loadMedia();

    SDL_Event event;
    bool quit = false;

    while(!quit){
        SDL_RenderClear(renderer);
        backgroundTexture.render(0, 0);
        fooTexture.render(240, 190);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event))
            if(event.type == SDL_QUIT)
                quit = true; 
    }

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
    fooTexture.loadFromFile("foo.png"); 
    backgroundTexture.loadFromFile("background.png");
}

void exit(){
    fooTexture.free();
    backgroundTexture.free();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    IMG_Quit();
}

LTexture::LTexture(){
    mWidth = mHeight = 0;
    mTexture = nullptr;
}

void LTexture::loadFromFile(std::string path){
    SDL_Surface* surface = IMG_Load(path.c_str()); 
    mWidth = surface->w; mHeight = surface->h;
    
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

    mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
}

void LTexture::render(int x, int y){
    SDL_Rect rect = {x, y, mWidth, mHeight};
    SDL_RenderCopy(renderer, mTexture, nullptr, &rect);
}

void LTexture::free(){
    if(mTexture != nullptr){
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = mHeight = 0;
    }
}

LTexture::~LTexture(){
    free();
}

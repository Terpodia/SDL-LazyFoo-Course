#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void loadFromFile(string path);
        void render(int x, int y, SDL_Rect* clip = nullptr);
        void setBlendMode(SDL_BlendMode blending);
        void setAlpha(Uint8 alpha); 
        void free();

    private:
        int mWidth;
        int mHeight;
        SDL_Texture* mTexture;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window *window;
SDL_Renderer* renderer;
LTexture backgroundTexture, frontTexture;

void init();
void loadMedia();
void exit();

int main(int argc, char* agrs[]){
    init(); loadMedia();

    SDL_Event e; bool quit = false;
    Uint8 alpha = 255;

    while(!quit){
        SDL_RenderClear(renderer);
        backgroundTexture.render(0, 0);
        frontTexture.setAlpha(alpha);
        frontTexture.render(0, 0);

        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_w) alpha = min(alpha + 32, 255);
                else if(e.key.keysym.sym == SDLK_s) alpha = max(alpha - 32, 0);
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
    backgroundTexture.loadFromFile("fadein.png");
    frontTexture.loadFromFile("fadeout.png");
    frontTexture.setBlendMode(SDL_BLENDMODE_BLEND);
}

void exit(){
    backgroundTexture.free();
    frontTexture.free();

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

void LTexture::render(int x, int y, SDL_Rect* clip){
    SDL_Rect rect = {x, y, mWidth, mHeight};
    if(clip != nullptr){
        rect.w = clip->w; rect.h = clip->h;
    }
    SDL_RenderCopy(renderer, mTexture, clip, &rect);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::free(){
    mWidth = mHeight = 0;
    if(mTexture != nullptr) SDL_DestroyTexture(mTexture); 
}

LTexture::~LTexture(){
    free();
}

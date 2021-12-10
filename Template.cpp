#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class LTexture
{
    public:
        LTexture();
        ~LTexture();
        void loadFromFile(string path);
        void loadFromRenderedText(string text, SDL_Color color);
        void render(int x, int y, SDL_Rect* clip = nullptr, float angles = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flipType = SDL_FLIP_NONE);
        void free();
        void setColor(Uint8 r, Uint8 g, Uint8 b);
        void setBlendMode(SDL_BlendMode blendMode);
        void setAlpha(Uint8 a);
        int getWidth();
        int getHeight();

    private:
        int w;
        int h;
        SDL_Texture* texture;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

void init();
void loadMedia();
void exit();

int main(int argc, char* args[])
{
    init(); loadMedia();
    
    SDL_Event event;
    bool quit = false;

    while(!quit){
        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT) quit = true;
        }
    }

    exit();
    return 0;
}

void init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
}

void loadMedia()
{

}

void exit()
{
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); renderer = nullptr;
    if(font != nullptr){ TTF_CloseFont(font); font = nullptr; }

    SDL_Quit(); IMG_Quit(); TTF_Quit();
}

LTexture::LTexture()
{
    w = h = 0;
    texture = nullptr;
}

LTexture::~LTexture(){ free(); }

void LTexture::free()
{
    if(texture != nullptr){
        SDL_DestroyTexture(texture);
        texture = nullptr, w = 0; h = 0;
    }
}

int LTexture::getWidth(){ return w; }

int LTexture::getHeight(){ return h; }

void LTexture::loadFromFile(string path)
{
    SDL_Surface* surface = IMG_Load(path.c_str()); 
    w = surface->w; h = surface->h; 
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;
}

void LTexture::render(int x, int y, SDL_Rect* clip, float angles, SDL_Point* center, SDL_RendererFlip flipType)
{
    SDL_Rect dst = {x, y, w, h};
    if(clip != nullptr){
        dst.w = clip->w; dst.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, texture, clip, &dst, angles, center, flipType);
}

void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetTextureColorMod(texture, r, g, b);
}

void LTexture::setBlendMode(SDL_BlendMode blendMode)
{
    SDL_SetTextureBlendMode(texture, blendMode);
}

void LTexture::setAlpha(Uint8 a)
{
    SDL_SetTextureAlphaMod(texture, a);
}

void LTexture::loadFromRenderedText(string text, SDL_Color color)
{
    free();
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    w = surface->w; h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;
}

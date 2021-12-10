#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>
using namespace std;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void loadFromRenderedText(string text, SDL_Color color);
        void render(int x, int y);
        void free();
        int getWidth();
        int getHeight();

    private:
        int w;
        int h;
        SDL_Texture* texture;
};

void init();
void loadMedia();
void exit();

const int WIDTH = 640, HEIGHT = 480;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
LTexture textTexture;

int main(int argc, char* args[]){
    init(); loadMedia();
        
    SDL_Event event;
    bool quit = false;
    
    while(!quit){
        SDL_RenderClear(renderer);
        textTexture.render((WIDTH-textTexture.getWidth())/2, (HEIGHT-textTexture.getHeight())/2);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0)
            if(event.type == SDL_QUIT) quit = true;
    }

    exit();
    return 0;
}

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); 
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    TTF_Init();
}

void loadMedia(){
    font = TTF_OpenFont("lazy.ttf", 36);
    SDL_Color color = {0, 0, 0};
    textTexture.loadFromRenderedText("AMOR PROPIO ERIKA", color);
}

void exit(){
    textTexture.free();
    TTF_CloseFont(font); font = nullptr;
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); window = nullptr;

    SDL_Quit(); TTF_Quit();
}

LTexture::LTexture(){
    w = h = 0;
    texture = nullptr;
}

LTexture::~LTexture(){
    free();
}

void LTexture::loadFromRenderedText(string text, SDL_Color color){
    free();
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    w = surface->w; h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;
}

void LTexture::render(int x, int y){
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void LTexture::free(){
    if(texture != nullptr){
        SDL_DestroyTexture(texture);
        texture = nullptr; w = 0; h = 0;
    }
} 

int LTexture::getWidth(){ return w; }
int LTexture::getHeight(){ return h; }

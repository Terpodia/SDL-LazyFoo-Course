#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class LTexture{
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

const int WIDTH = 600, HEIGHT = 400;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

const int BUTTON_WIDTH = 300, BUTTON_HEIGHT = 200;

enum LButtonSprite{
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_MOUSE_TOTAL
};

SDL_Rect buttonClips[4];
LTexture buttonTexture;

class LButton{
    public:
        LButton();
        ~LButton();
        void handleEvent(SDL_Event* event);
        void render();
        void setPosition(int x, int y);

    private:
        SDL_Point mPosition;
        LButtonSprite currentState;
};

LButton buttons[4];

void init();
void loadMedia();
void exit();

int main(int argc, char* args[]){
    init(); loadMedia();
    
    SDL_Event event;
    bool quit = false;

    while(!quit){
        SDL_RenderClear(renderer);
        for(int i=0; i<4; i++) buttons[i].render();
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT) quit = true;
            else{
                for(int i=0; i < 4; i++)
                    buttons[i].handleEvent(&event);
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
    TTF_Init();
}

void loadMedia(){
    buttonTexture.loadFromFile("button.png");
    buttonClips[BUTTON_SPRITE_MOUSE_OUT] = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT}; 
    buttonClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = {0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
    buttonClips[BUTTON_SPRITE_MOUSE_DOWN] = {0, BUTTON_HEIGHT*2, BUTTON_WIDTH, BUTTON_HEIGHT};
    buttonClips[BUTTON_SPRITE_MOUSE_UP] = {0, BUTTON_HEIGHT*3, BUTTON_WIDTH, BUTTON_HEIGHT}; 

    buttons[0].setPosition(0, 0);
    buttons[1].setPosition(300, 0);
    buttons[2].setPosition(0, 200);
    buttons[3].setPosition(300, 200);
}

void exit(){
    buttonTexture.free();
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); renderer = nullptr;
    if(font != nullptr){ TTF_CloseFont(font); font = nullptr; }

    SDL_Quit(); IMG_Quit(); TTF_Quit();
}

LButton::LButton(){
    mPosition.x = mPosition.y = 0;
    currentState = BUTTON_SPRITE_MOUSE_OUT;
}

LButton::~LButton(){}

void LButton::setPosition(int x, int y){
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent(SDL_Event *event){
    if(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP){
        int x, y; SDL_GetMouseState(&x, &y);
        bool inside = (x >= mPosition.x && x <= mPosition.x + BUTTON_WIDTH && y >= mPosition.y && y <= mPosition.y + BUTTON_HEIGHT);

        if(!inside) currentState = BUTTON_SPRITE_MOUSE_OUT;
        else{
            switch(event->type){
                case SDL_MOUSEMOTION:
                    currentState = BUTTON_SPRITE_MOUSE_OVER_MOTION; break;

                case SDL_MOUSEBUTTONDOWN:
                    currentState = BUTTON_SPRITE_MOUSE_DOWN; break;

                case SDL_MOUSEBUTTONUP:
                    currentState = BUTTON_SPRITE_MOUSE_UP; break;
            }
        }
    }
}

void LButton::render(){
    buttonTexture.render(mPosition.x, mPosition.y, &buttonClips[currentState]);
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
        SDL_DestroyTexture(texture);
        texture = nullptr, w = 0; h = 0;
    }
}

int LTexture::getWidth(){
    return w;
}

int LTexture::getHeight(){
    return h;
}

void LTexture::loadFromFile(string path){
    SDL_Surface* surface = IMG_Load(path.c_str()); 
    w = surface->w; h = surface->h; 
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;
}

void LTexture::render(int x, int y, SDL_Rect* clip, float angles, SDL_Point* center, SDL_RendererFlip flipType){
    SDL_Rect dst = {x, y, w, h};
    if(clip != nullptr){
        dst.w = clip->w; dst.h = clip->h;
    }
    SDL_RenderCopyEx(renderer, texture, clip, &dst, angles, center, flipType);
}

void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b){
    SDL_SetTextureColorMod(texture, r, g, b);
}

void LTexture::setBlendMode(SDL_BlendMode blendMode){
    SDL_SetTextureBlendMode(texture, blendMode);
}

void LTexture::setAlpha(Uint8 a){
    SDL_SetTextureAlphaMod(texture, a);
}

void LTexture::loadFromRenderedText(string text, SDL_Color color){
    free();
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    w = surface->w; h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); surface = nullptr;
}


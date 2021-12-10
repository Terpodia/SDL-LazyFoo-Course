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

class LTimer
{
    public:
        LTimer();
        ~LTimer();
        void start();
        void pause();
        void stop();
        void unpause();
        bool isPaused();
        bool isStarted();
        Uint32 getTicks();         
        
    private:
        bool paused;
        bool started;
        Uint32 startTicks;
        Uint32 pausedTicks;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

LTexture  timeTextTexture, startTextTexture, pauseTextTexture;

void init();
void loadMedia();
void exit();

int main(int argc, char* args[])
{
    init(); loadMedia();
    
    SDL_Event event;
    bool quit = false;
    LTimer timer;
    
    while(!quit){
        SDL_RenderClear(renderer);
        timeTextTexture.loadFromRenderedText("Seconds since start time: " + to_string(timer.getTicks() / 1000.f), {0,0,0,255});
        startTextTexture.render((WIDTH - startTextTexture.getWidth())/2, 0);
        pauseTextTexture.render((WIDTH - pauseTextTexture.getWidth())/2, startTextTexture.getHeight()); 
        timeTextTexture.render((WIDTH - timeTextTexture.getWidth())/2, (HEIGHT - timeTextTexture.getHeight())/2);

        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT) quit = true;
            
            else if(event.type == SDL_KEYDOWN){
                const Uint8* key = SDL_GetKeyboardState(nullptr); 
                if(*(key + SDL_SCANCODE_S)){
                    if(!timer.isStarted()) timer.start();
                    else timer.stop();
                }
                else if(*(key + SDL_SCANCODE_P)){
                    if(!timer.isPaused()) timer.pause();
                    else timer.unpause();
                }
            }
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
    font = TTF_OpenFont("lazy.ttf", 28);
    startTextTexture.loadFromRenderedText("Press S to Start or Stop the Timer", {0,0,0,255});
    pauseTextTexture.loadFromRenderedText("Press P to Pause or Unpause the Timer", {0,0,0,255});
}

void exit()
{
    SDL_DestroyRenderer(renderer); renderer = nullptr;
    SDL_DestroyWindow(window); renderer = nullptr;
    startTextTexture.free(); pauseTextTexture.free(); timeTextTexture.free();

    if(font != nullptr){ TTF_CloseFont(font); font = nullptr; }

    SDL_Quit(); IMG_Quit(); TTF_Quit();
}

LTimer::LTimer()
{
    started = paused = false;
    startTicks = pausedTicks = 0;
}

LTimer::~LTimer(){}

void LTimer::start()
{
    if(!started){
        started = true;
        startTicks = SDL_GetTicks();
    }
}

void LTimer::pause()
{
    if(started && !paused){
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void LTimer::unpause()
{
    if(paused && started){
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
        paused = false;
    }
}

void LTimer::stop()
{
    started = paused = false;
    startTicks = pausedTicks = 0;
}

Uint32 LTimer::getTicks()
{
    if(!started) return 0;
    if(paused) return pausedTicks;
    return SDL_GetTicks() - startTicks; 
}

bool LTimer::isStarted() { return started; }

bool LTimer::isPaused() { return paused; }

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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class LTexture{
    public:
        LTexture();
        ~LTexture();
        void loadFromPath(std::string path);
        void render(int x, int y, SDL_Rect* clip);
        void free();

    private:
        int mWidth;
        int mHeight;
        SDL_Texture* texture;
};

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window;
SDL_Renderer* renderer;
LTexture spriteSheet; 
SDL_Rect clips[4];

void init();
void loadMedia();
void exit();

int main(int argv, char* args[]){
    init();
    loadMedia();
    
    SDL_Event event;
    bool quit = false;

    while(!quit){
        SDL_RenderClear(renderer);

        spriteSheet.render(0, 0, clips);
        spriteSheet.render(WIDTH - clips[1].w, 0, clips+1);
        spriteSheet.render(0, HEIGHT - clips[2].h, clips+2);
        spriteSheet.render(WIDTH - clips[3].w, HEIGHT - clips[3].h, clips+3);

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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    IMG_Init(IMG_INIT_PNG);
}

void loadMedia(){
    spriteSheet.loadFromPath("dots.png");
    clips[0] = {0, 0, 100, 100};
    clips[1] = {100, 0, 100, 100};
    clips[2] = {0, 100, 100, 100};
    clips[3] = {100, 100, 100, 100};
}

void exit(){
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
    IMG_Quit();
}

LTexture::LTexture(){
    mWidth = mHeight = 0;
    texture = nullptr;
}

LTexture::~LTexture(){
    free();
}

void LTexture::free(){
    mWidth = mHeight = 0;
    if(texture != nullptr){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void LTexture::loadFromPath(std::string path){
    SDL_Surface* surface = IMG_Load(path.c_str());
    mWidth = surface->w; mHeight = surface->h;

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
}

void LTexture::render(int x, int y, SDL_Rect *clip){
    SDL_Rect rect = {x, y, mWidth, mHeight};
    if(clip != nullptr){
        rect.w = clip->w; rect.h = clip->h;
    }
    SDL_RenderCopy(renderer, texture, clip, &rect);
}


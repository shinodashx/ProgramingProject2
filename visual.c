//
// Created by shinoda_shx on 2022/5/9.
//
#include <stdio.h>
#include "SDL2/SDL.h"


#define WINDOW_WIDTH 2400
#define WINDOW_HEIGHT 1400


void visual_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
}

void update_visual(void) {

}

void visual_main(){
    visual_init();
    SDL_Window *window;
    SDL_Renderer *renderer;
    long double offset_x = -200;
    long double offset_y = -200;
    long double pointsz = 2;
    window = SDL_CreateWindow("Visual", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
//
// Created by shinoda_shx on 2022/5/9.
//
#include <stdio.h>
#include "SDL2/SDL.h"
#include "utils.h"

#define WINDOW_WIDTH 2400
#define WINDOW_HEIGHT 1400

void visual_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
}

void calc(long double lat, long double lon, long double *x, long double *y) {
    long double base_lat = 1000, base_lon = 1000;
    *x = (lon - base_lon) * 1000000 / 10 + 10;
    *y = (lat - base_lat) * 1000000 / 10 + 10;
}

void draw(SDL_Window *window,link *edge_list, nodeLink *node_list, SDL_Renderer *renderer, struct Edge *edge, long long *head,nodeArray *node_array, long long endPoint, long long *prev) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    nodeLink *node_ptr = node_list;
    node_ptr = node_ptr->next;
    while (node_ptr != NULL) {
        long double *x = malloc(sizeof(long double)), *y = malloc(sizeof(long double));
        calc(node_ptr->lat, node_ptr->lon, x, y);
        SDL_Rect rect = {.x = (int)*x, .y = (int)*y, .w = 10, .h = 10};
        SDL_RenderFillRect(renderer, &rect);
        long long u = node_ptr->ID;
        for(long long i = head[u]; i != -1; i = edge[i].next) {
            long double *x1 = malloc(sizeof(long double)), *y1 = malloc(sizeof(long double));
            calc(node_ptr->lat, node_ptr->lon, x1, y1);
            SDL_RenderDrawLine(renderer, (int)*x, (int)*y, (int)*x1, (int)*y1);
        }
        node_ptr = node_ptr->next;
    }
    for(;endPoint != -1;endPoint = prev[endPoint]) {
        long double *x = malloc(sizeof(long double)), *y = malloc(sizeof(long double));
        calc(node_array[endPoint].lat, node_array[endPoint].lon, x, y);
        long double *x1 = malloc(sizeof(long double)), *y1 = malloc(sizeof(long double));
        calc(node_array[prev[endPoint]].lat, node_array[prev[endPoint]].lon, x1, y1);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, (int)*x, (int)*y, (int)*x1, (int)*y1);
    }
}

void visual_main(link *edgelink, nodeLink *nodelink, struct Edge *edge, long long *head, long long *path, nodeArray *node_array,long long endPoint) {
    visual_init();
    SDL_Window *window;
    SDL_Renderer *renderer;
    long double offset_x = -200;
    long double offset_y = -200;
    long double pointsz = 2;
    window = SDL_CreateWindow("Visual", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
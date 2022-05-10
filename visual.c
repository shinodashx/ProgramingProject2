//
// Created by shinoda_shx on 2022/5/9.
//
#include <stdio.h>
#include "SDL2/SDL.h"
#include "utils.h"

#define WINDOW_WIDTH 2400
#define WINDOW_HEIGHT 1400

long double baselat, baselon;

void visual_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    baselat = 1e18, baselon = 1e18;
}

void calc(long double lat, long double lon, long double *x, long double *y) {
    *x = (lon - baselon) * 1000000 / 10 + 10;
    *y = (lat - baselat) * 1000000 / 10 + 10;
}

void draw(SDL_Window *window,link *edge_list, nodeLink *node_list, SDL_Renderer *renderer, struct Edge *edge, long long *head,nodeArray *node_array, long long endPoint, long long *prev, long long totNode) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    nodeLink *node_ptr = node_list;
    for (long long i = 1; i<=totNode; ++ i){
        long double *x = malloc(sizeof(long double)), *y = malloc(sizeof(long double));
        calc(node_array[i].lat, node_array[i].lon, x, y);
        SDL_Rect rect = {.x = (int)*x, .y = (int)*y, .w = 6, .h = 6};
        SDL_RenderFillRect(renderer, &rect);
        long long u = i;
        for(long long j = head[u]; j != -1; j = edge[j].next) {
            long long v = edge[j].v;
            long double *x1 = malloc(sizeof(long double)), *y1 = malloc(sizeof(long double));
            calc(node_array[v].lat, node_array[v].lon, x1, y1);
            SDL_RenderDrawLine(renderer, (int)*x, (int)*y, (int)*x1, (int)*y1);
        }
        node_ptr = node_ptr->next;
        free(x);
        free(y);
    }
    long long last = endPoint;
    for(;endPoint != -1;endPoint = prev[endPoint]) {
        long double *nowx = malloc(sizeof(long double)), *nowy = malloc(sizeof(long double));
        calc(node_array[endPoint].lat, node_array[endPoint].lon, nowx, nowy);
        long double *prex = malloc(sizeof(long double)), *prey = malloc(sizeof(long double));
        calc(node_array[last].lat, node_array[last].lon, prex, prey);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, (int)*nowx, (int)*nowy, (int)*prex, (int)*prey);
        last = endPoint;
    }
}

void visual_main(link *edgelink, nodeLink *nodelink, struct Edge *edge, long long *head, long long *prev, nodeArray *node_array,long long endPoint, long long totNode) {
    visual_init();
    for(int i = 1; i<=totNode;++i){
        if(node_array[i].lat < baselat) baselat = node_array[i].lat;
        if(node_array[i].lon<baselon) baselon = node_array[i].lon;
    }
    SDL_Window *window;
    SDL_Renderer *renderer;
    long double offset_x = -200;
    long double offset_y = -200;
    long double pointsz = 2;
    window = SDL_CreateWindow("Visual", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    while(1) {
        draw(window, edgelink, nodelink, renderer, edge, head, node_array, endPoint, prev,totNode);
        SDL_RenderPresent(renderer);
    }

}
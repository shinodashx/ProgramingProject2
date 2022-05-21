//
// Created by shx
//
#include <stdio.h>
#include "SDL2/SDL.h"
#include "utils.h"
#include "routeFind.h"

#define WINDOW_WIDTH 2400
#define WINDOW_HEIGHT 1200

long double baselat, baselon;
long double offset_x = -20;
long double offset_y = -20;
long double pointsz = 4;
long double scale = 10;

//Initialize SDL
void visual_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    baselat = 1e18, baselon = 1e18;
}

//Calculate the position of the current point on the screen
void calc(long double lat, long double lon, long double *x, long double *y) {
    *x = (lon - baselon) * 1000000 / scale + offset_x;
    *y = (lat - baselat) * 1000000 / scale + offset_y;
}

//Calculate the position of the current point x coordinate on the screen
long double calcx(long double lat, long double lon, long double x, long double y) {
    x = (lon - baselon) * 1000000 / scale + offset_x;
    return x;
}

//Calculate the position of the current point y coordinate on the screen
long double calcy(long double lat, long double lon, long double x, long double y) {
    y = (lat - baselat) * 1000000 / scale + offset_y;
    return y;
}

//Draw the map and the shortest path.
void draw(SDL_Window *window, link *edge_list, nodeLink *node_list, SDL_Renderer *renderer, struct Edge *edge,
          long long *head, nodeArray *node_array, long long endPoint, long long totNode) {


    long long *prev = getPrev();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    nodeLink *node_ptr = node_list;
    long double x, y;
    for (long long i = 1; i <= totNode; ++i) {
        x = calcx(node_array[i].lat, node_array[i].lon, x, y);
        y = calcy(node_array[i].lat, node_array[i].lon, x, y);
        SDL_Rect rect = {.x = (int) x, .y = (int) y, .w = pointsz, .h = pointsz};
        SDL_RenderFillRect(renderer, &rect);
        int a = x, b = y;

        long long u = i;
        for (long long j = head[u]; j != -1; j = edge[j].next) {
            long long v = edge[j].v;
            long double *x1 = malloc(sizeof(long double)), *y1 = malloc(sizeof(long double));
            calc(node_array[v].lat, node_array[v].lon, x1, y1);
            SDL_RenderDrawLine(renderer, (int) x + pointsz / 2, (int) y + pointsz / 2, (int) *x1 + pointsz / 2,
                               (int) *y1 + pointsz / 2);
        }
        node_ptr = node_ptr->next;
        //free(x);
        //free(y);
    }
    long long last = endPoint;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (; endPoint != -1; endPoint = prev[endPoint]) {
        long double *nowx = malloc(sizeof(long double)), *nowy = malloc(sizeof(long double));
        calc(node_array[endPoint].lat, node_array[endPoint].lon, nowx, nowy);
        long double *prex = malloc(sizeof(long double)), *prey = malloc(sizeof(long double));
        calc(node_array[last].lat, node_array[last].lon, prex, prey);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {.x = (int) *nowx, .y = (int) *nowy, .w = pointsz, .h = pointsz};
        SDL_RenderFillRect(renderer, &rect);
        SDL_Rect rect1 = {.x = (int) *prex, .y = (int) *prey, .w = pointsz, .h = pointsz};
        SDL_RenderFillRect(renderer, &rect1);
        SDL_RenderDrawLine(renderer, (int) *nowx + pointsz / 2, (int) *nowy + pointsz / 2, (int) *prex + pointsz / 2,
                           (int) *prey + pointsz / 2);
        last = endPoint;
    }
}

//Update and red mark the point selected by the user.
void highlight(SDL_Window *window, link *edge_list, nodeLink *node_list, SDL_Renderer *renderer, struct Edge *edge,
               long long *head, nodeArray *node_array, long long endPoint, long long *prev, long long totNode,
               long long nownode) {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    nodeLink *node_ptr = node_list;
    long double x, y;
    for (long long i = 1; i <= totNode; ++i) {
        if (node_array[i].ID == nownode) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            x = calcx(node_array[i].lat, node_array[i].lon, x, y);
            y = calcy(node_array[i].lat, node_array[i].lon, x, y);
            SDL_Rect rect = {.x = (int) x, .y = (int) y, .w = pointsz, .h = pointsz};
            SDL_RenderFillRect(renderer, &rect);
            int a = x, b = y;
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            x = calcx(node_array[i].lat, node_array[i].lon, x, y);
            y = calcy(node_array[i].lat, node_array[i].lon, x, y);
            SDL_Rect rect = {.x = (int) x, .y = (int) y, .w = pointsz, .h = pointsz};
            SDL_RenderFillRect(renderer, &rect);
            int a = x, b = y;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        long long u = i;
        for (long long j = head[u]; j != -1; j = edge[j].next) {
            long long v = edge[j].v;
            long double *x1 = malloc(sizeof(long double)), *y1 = malloc(sizeof(long double));
            calc(node_array[v].lat, node_array[v].lon, x1, y1);
            SDL_RenderDrawLine(renderer, (int) x + pointsz / 2, (int) y + pointsz / 2, (int) *x1 + pointsz / 2,
                               (int) *y1 + pointsz / 2);
        }
    }
}

long long *newprev;

void visual_main(link *edgelink, nodeLink *nodelink, struct Edge *edge, long long *head, long long *prev,
                 nodeArray *node_array, long long endPoint, long long totNode) {
    visual_init();
    for (int i = 1; i <= totNode; ++i) {
        if (node_array[i].lat < baselat) baselat = node_array[i].lat;
        if (node_array[i].lon < baselon) baselon = node_array[i].lon;
    }
    int quitflag = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;

    window = SDL_CreateWindow("Visual", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    draw(window, edgelink, nodelink, renderer, edge, head, node_array, endPoint, totNode);
    SDL_RenderPresent(renderer);

    long long start = -1, end = -1;

    while (!quitflag) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitflag = 1;
            }
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scale -= 0.5;
                    pointsz /= 0.9;
                    if (pointsz <= 0) pointsz = 0.1;
                } else if (event.wheel.y < 0) {
                    scale += 0.5;
                    pointsz *= 0.9;
                    if (pointsz <= 0) pointsz = 0.1;
                }
                long long t = endPoint;
                draw(window, edgelink, nodelink, renderer, edge, head, node_array, t, totNode);
                SDL_RenderPresent(renderer);
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state == SDL_PRESSED) {
                offset_x += event.motion.xrel / 4;
                offset_y += event.motion.yrel / 4;
                long long t = endPoint;
                draw(window, edgelink, nodelink, renderer, edge, head, node_array, t, totNode);
                SDL_RenderPresent(renderer);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // get the mouse position
                if (end != -1) {
                    start = -1;
                    end = -1;
                }
                int a, b;
                SDL_GetMouseState(&a, &b);
                long double x = (long double) a, y = (long double) b;
                long double xx, yy;
                for (long long i = 1; i <= totNode; ++i) {
                    xx = calcx(node_array[i].lat, node_array[i].lon, xx, yy);
                    yy = calcy(node_array[i].lat, node_array[i].lon, xx, yy);
                    if (fabsl((x - xx)) <= pointsz && fabsl(y - yy) <= pointsz) {
                        if (start != -1) end = i;
                        else start = i;
                        highlight(window, edgelink, nodelink, renderer, edge, head, node_array, endPoint, prev, totNode,
                                  node_array[i].ID);
                        SDL_RenderPresent(renderer);
                        break;
                    }
                }
            }

            if (end != -1) {
                long long s = start, t = end;
                endPoint = t;
                start = -1, end = -1;
                update(edgelink, nodelink, node_array[s].ID, node_array[t].ID);
                newprev = getPrev();
                draw(window, edgelink, nodelink, renderer, edge, head, node_array, t, totNode);
                SDL_RenderPresent(renderer);
            }
        }
    }
}
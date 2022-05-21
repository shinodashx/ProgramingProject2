//
// Created by shinoda_shx on 2022/5/9.
//


#include <stdio.h>
#include "SDL2/SDL.h"
#include "utils.h"
#ifndef CWK2_VISUAL_H
#define CWK2_VISUAL_H

void visual_init(void);
void clac(long double lat, long double lon, long double *x, long double *y);
void draw(SDL_Window *window,link *edge_list, nodeLink *node_list, SDL_Renderer *renderer);
void visual_main(link *edgelink, nodeLink *nodelink, struct Edge *edge, long long *head, long long *prev, nodeArray *node_array,long long endPoint,long long totNode);


#endif //CWK2_VISUAL_H
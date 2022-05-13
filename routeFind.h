//
// Created by shinoda_shx on 2022/5/9.
//

#ifndef CWK2_ROUTEFIND_H
#define CWK2_ROUTEFIND_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"


void addEdge(long long u, long long v, long double w);

void readLink(link *linklist, nodeLink *nodeLinkList, char *filename);

void init(link *linklist, nodeLink *nodeLinkList);

void nodedeDuplication();

void initDcsNode();

void buildGraph(link *linklist);

void getLen(long long node1, long long node2);

void build(long long n);

void modify(long long x, long double nv);

void del(long long x);

long double dijkstra(long long s, long long t);

long double SPFA(long long s, long long t);

void printPath(long long end);

void find();

#endif //CWK2_ROUTEFIND_H

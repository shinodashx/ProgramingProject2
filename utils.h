//
// Created by shinoda_shx on 2022/4/30.
//

#ifndef CWK2_UTILS_H
#define CWK2_UTILS_H

long long cmp(long long a, long long b, long double x, long double y);

long long binarySearchPos(long long node, long long totNode, long long *nodeId);

struct Edge {
    long long u, v;
    long double w;
    long long next;
};

typedef struct _link {
    long double linkId;
    long long node1;
    long long node2;
    long double way;
    long double len;
    long double arch;
    long double veg;
    long double land;
    long double poi;
    // long long linkcnt;
    struct _link *next;
} link;

typedef struct _NodeLink {
    long long ID;
    long double lat;
    long double lon;
    struct _NodeLink *next;
} nodeLink;


typedef struct _Node {
    long long ID;
    long double lat;
    long double lon;
} nodeArray;

void mergeSort(long long *a, long long *b, long long l, long long r);

#endif //CWK2_UTILS_H

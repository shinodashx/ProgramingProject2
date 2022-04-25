#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int maxn = 5e4 + 100;

int totNode, totLink;


//int head[maxn * 2], to[maxn * 2], nxt[maxn * 2], n, m;

long long nodeid[maxn * 2], dcsNode[maxn * 2], dcsNodeId[maxn * 2];

struct _link {
    long double linkId;
    long double node1;
    long double node2;
    long double way;
    long double len;
    long double arch;
    long double veg;
    long double land;
    long double poi;
} link[maxn * 2];


long long linkid[maxn * 2];
long double disOfLink[maxn * 2];
long double vegOfLink[maxn * 2];
long double archOfLink[maxn * 2];
long double landOfLink[maxn * 2];

int cntEdge;

typedef struct _Edge{
    long long  u, v;
    long double w;
    int head;
    struct _Edge *next;

} Edge;
void addEdge(long long u, long long v, long double w){
    Edge *e = (Edge *)malloc(sizeof(Edge));
    e->u = u;
    e->v = v;
    e->w = w;
    e->next = head[u];
    head[u] = e;
    cntEdge++;
}


void readLink(char *filename) {
    long long linkCnt = 0;
    long long nodeCnt = 0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("open file error!\n");
        exit(1);
    }
    while (!feof(fp)) {
        char linkId[100], node1[100], node2[100], way[100], len[100], veg[100], arch[100], land[100], poi[100];
        char buffer[100];
        fscanf(fp, "%s", buffer);
        if (strstr(buffer, "Link") == NULL)
            continue;
        linkCnt++;
        sscanf(buffer,
               "<link id=%s node=%s node=%s way=%s length=%s veg=%s arch=%s "
               "land=%s POI=%s;/link>",
               &linkId, &node1, &node2, &way, &len, &veg, &arch, &land, &poi);

        link[linkCnt].linkId = atof(linkId);
        link[linkCnt].node1 = atof(node1);
        link[linkCnt].node2 = atof(node2);
        link[linkCnt].way = atof(way);
        link[linkCnt].len = atof(len);
        link[linkCnt].veg = atof(veg);
        link[linkCnt].arch = atof(arch);
        link[linkCnt].land = atof(land);
        link[linkCnt].poi = atof(poi);

        nodeid[++nodeCnt] = atoi(node1);
        nodeid[++nodeCnt] = atoi(node2);

    }
    totNode = nodeCnt;
    totLink = linkCnt;
    for (int i = 1; i <= totNode; i++) {
        dcsNode[i] = nodeid[i];
    }
}

void swap(long long *a, long long *b) {
    long long tmp = *a;
    *a = *b;
    *b = tmp;
}

void sortNode(int l, int r) {
    int mid = dcsNode[(l + r) / 2];
    int i = l, j = r;
    do {
        while (dcsNode[i] < mid)
            i++;
        while (dcsNode[j] > mid)
            j--;
        if (i <= j) {
            swap(dcsNode[i], dcsNode[j]);
            i++;
            j--;
        }
    } while (i <= j);
    if (l < j) sortNode(l, j);
    if (i < r) sortNode(i, r);
}


void buildGraph() {
    cntEdge = 0;
    for (int i = 1; i <= totLink; i++) {
        addEdge(link[i].node1, link[i].node2, link[i].len);
        addEdge(link[i].node2, link[i].node1, link[i].len);
    }
}

int main() {
    printf("Hello, World!\n");
    return 0;
}

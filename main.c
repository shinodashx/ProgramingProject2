#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int maxn = 5e4 + 100;

int totNode, totLink;


//int head[maxn * 2], to[maxn * 2], nxt[maxn * 2], n, m;

//long long nodeid[maxn * 2], dcsNode[maxn * 2], dcsNodeId[maxn * 2];

long long *nodeId, *dcsNodeId, *dcsNode;
typedef struct _link{
    long double linkId;
    long double node1;
    long double node2;
    long double way;
    long double len;
    long double arch;
    long double veg;
    long double land;
    long double poi;
    long long linkcnt;
    struct _link *next;
} link;


long long linkid[maxn * 2];
long double disOfLink[maxn * 2];
long double vegOfLink[maxn * 2];
long double archOfLink[maxn * 2];
long double landOfLink[maxn * 2];

int cntEdge;

void readLink(link *linklist,char *filename) {

    long long linkCnt = 0;
    long long nodeCnt = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("open file error!\n");
        exit(1);
    }

    link *now = linklist;

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
        link *p = (link *)malloc(sizeof(link));
        p->linkId = atof(linkId);
        p->node1 = atof(node1);
        p->node2 = atof(node2);
        p->way = atof(way);
        p->len = atof(len);
        p->veg = atof(veg);
        p->arch = atof(arch);
        p->land = atof(land);
        p->poi = atof(poi);
        p->linkcnt = now->linkcnt + 1;
        p->next = NULL;
        now->next = p;
        now = p;

        nodeId[++nodeCnt] = atof(node1);
        nodeId[++nodeCnt] = atof(node2);
    }
    totLink = linkCnt;
    totNode = nodeCnt;
    for(int i = 1; i <= nodeCnt; i++)
    {
        dcsNodeId[i] = nodeId[i];
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

void uniqueNode(int l, int r) {
    int i = l, j = l + 1;
    while (j <= r) {
        if (dcsNode[j] != dcsNode[i]) {
            i++;
            dcsNode[i] = dcsNode[j];
        }
        j++;
    }
    totNode = i - l + 1;
}
void buildGraph() {
    cntEdge = 0;

}

int main() {
    link *linklist = (link *)malloc(sizeof(link));
    linklist->next = NULL;
    linklist->linkcnt = 0;
    printf("Hello, World!\n");
    return 0;
}

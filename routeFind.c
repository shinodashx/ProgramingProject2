#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "visual.h"
#include "UI.h"

long long totNode, totLink;

long long *nodeId, *dcsNodeId, *dcsNode;
long long *tmpNodeId;
long long *head;

long double *heap;
long double *dist;
long long *prev;
long long *path;
long long M;
long long *mp;
long long *vis;

struct Edge *edge;

void findmain(link *pLink, nodeLink *pLink1);

int totEdge;
nodeArray *nodeA;

//Used to add edges to an array-optimized adjacency list
void addEdge(long long u, long long v, long double w) {
    edge[++totEdge].u = u;
    edge[totEdge].v = v;
    edge[totEdge].w = w;
    edge[totEdge].next = head[u];
    head[u] = totEdge;
}

//It is used to read the edge and point information in the file and store it in two linked lists respectively.
int readLink(link *linklist, nodeLink *nodeLinkList, char *filename) {

    long long linkCnt = 0;
    long long nodeCnt = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("open file error!\n");
        return -1;
    }

    link *now = linklist;
    nodeLink *nowNode = nodeLinkList;

    while (!feof(fp)) {
        char linkId[100] = {0}, node1[100] = {0}, node2[100] = {0}, way[100] = {0}, len[20], veg[100] = {
                0}, arch[100] = {0}, land[100] = {0}, poi[100] = {0};
        char buffer[100] = {0};
        fgets(buffer, 100, fp);
        if (strstr(buffer, "<link") != NULL) {
            linkCnt++;
            sscanf(buffer,
                   "<link id=%s node=%s node=%s way=%s length=%s veg=%s arch=%s "
                   "land=%s POI=%s;/link>",
                   &linkId, &node1, &node2, &way, &len, &veg, &arch, &land, &poi);
            //printf("%s %s %s \n",node1,node2,len);
            link *p = (link *) malloc(sizeof(link));
            p->linkId = atof(linkId);
            p->node1 = atoll(node1);
            p->node2 = atoll(node2);
            p->way = atof(way);
            // p->len = atof(len);
            p->len = strtold(len, NULL);
            p->veg = atof(veg);
            p->arch = atof(arch);
            p->land = atof(land);
            p->poi = atof(poi);
            // p->linkcnt = now->linkcnt + 1;
            p->next = NULL;
            now->next = p;
            now = p;
        } else if (strstr(buffer, "<node") != NULL) {
            nodeCnt++;
            char Id[100] = {0};
            char lat[100] = {0};
            char lon[100] = {0};
            sscanf(buffer, "<node id=%s lat=%s lon=%s;/node>", &Id, &lat, &lon);
            nodeLink *p = (nodeLink *) malloc(sizeof(nodeLink));
            p->ID = atoll(Id);
            p->lat = atof(lat);
            p->lon = atof(lon);
            p->next = NULL;
            nowNode->next = p;
            nowNode = p;
        }
        // printf("%lld %lld %Lf\n", p->node1, p->node2, p->len);
    }
    totLink = linkCnt;
    totNode = linkCnt + nodeCnt;
    return 0;
}

//Initialize the required arrays and store the point information.
void init(link *linklist, nodeLink *nodeLinkList) {
    int cnt = 0;
    tmpNodeId = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    nodeId = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    dcsNodeId = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    dcsNode = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    head = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));

    edge = (struct Edge *) malloc(sizeof(struct Edge) * 2 * (totNode + 100));
    memset(head, -1, sizeof(long long) * 2 * (totNode + 100));

    memset(tmpNodeId, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(nodeId, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(dcsNodeId, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(dcsNode, -1, sizeof(long long) * 2 * (totNode + 100));
    //memset(edge, 0, sizeof(struct Edge) * 2 * (totNode + 100));
    heap = (long double *) malloc(sizeof(long double) * 5 * (totNode + 100));
    memset(heap, 0, sizeof(long double) * 5 * (totNode + 100));
    dist = (long double *) malloc(sizeof(long double) * 5 * (totNode + 100));
    memset(dist, 0x7f, sizeof(long double) * 5 * (totNode + 100));
    prev = (long long *) malloc(sizeof(long long) * 5 * (totNode + 100));
    memset(prev, -1, sizeof(long long) * 5 * (totNode + 100));
    path = (long long *) malloc(sizeof(long long) * 5 * (totNode + 100));
    memset(path, -1, sizeof(long long) * 5 * (totNode + 100));
    mp = (long long *) malloc(sizeof(long long) * 6 * (totNode + 100));
    M = 1;
    vis = (long long *) malloc(sizeof(long long) * 5 * (totNode + 100));
    memset(vis, 0, sizeof(int) * 5 * (totNode + 100));

    nodeA = (nodeArray *) malloc(sizeof(nodeArray) * 2 * (totNode + 100));

    link *now = linklist;
    now = now->next;
    while (now != NULL) {
        nodeId[++cnt] = now->node1;
        nodeId[++cnt] = now->node2;
        now = now->next;
    }
    nodeLink *nowNode = nodeLinkList;
    nowNode = nowNode->next;
    while (nowNode != NULL) {
        nodeId[++cnt] = nowNode->ID;
        nowNode = nowNode->next;
    }
    totNode = cnt;
}

//Deduplicates points that were repeated during the previous read.
void nodedeDuplication() {
    long long i, j;
    for (i = 1; i <= totNode; i++) {
        for (j = i + 1; j <= totNode; j++) {
            if (nodeId[i] == nodeId[j]) {
                nodeId[j] = -1;
            }
        }
    }
    for (i = 1; i <= totNode; i++) {
        if (nodeId[i] == -1) {
            for (j = i; j <= totNode; j++) {
                nodeId[j] = nodeId[j + 1];
            }
            totNode--;
            i--;
        }
    }
}

//Initialize the labels of the discretized points.
void initDcsNode() {
    for (int i = 1; i <= totNode; i++) {
        dcsNode[i] = i;
    }
}

//Build graphs using array-optimized adjacency lists.
void buildGraph(link *linklist) {
    totEdge = 0;
    link *now = linklist;
    now = now->next;
    while (now != NULL) {
        long long node1 = now->node1;
        long long node2 = now->node2;
        long long pos1 = binarySearchPos(node1, totNode, nodeId);
        long long pos2 = binarySearchPos(node2, totNode, nodeId);
        if (pos1 == -1 || pos2 == -1) {
            printf("error!\n");
            exit(1);
        }
        long double weight = now->len;
        addEdge(pos1, pos2, weight);
        addEdge(pos2, pos1, weight);
        now = now->next;
    }
}

//Returns the distance between two edges
void getLen(long long node1, long long node2) {
    long long pos1 = binarySearchPos(node1, totNode, nodeId);
    long long pos2 = binarySearchPos(node2, totNode, nodeId);
    for (int i = head[pos1]; i != -1; i = edge[i].next) {
        if (edge[i].v == pos2) {
            printf("%Lf\n", edge[i].w);
            return;
        }
    }
    printf("-1\n");
}

//Used to build segment trees
void build(long long n) {
    while (M < n + 2) M <<= 1;
    mp[0] = n + 1;
}

//Single-point modification to the value of the segment tree
void modify(long long x, long double nv) {
    for (long long i = x + M; (long double) dist[mp[i]] > (long double) nv; i >>= 1)
        mp[i] = x;
    dist[x] = nv;
}

//delete a point on the segment tree
void del(long long x) {
    for (mp[x += M] = 0, x >>= 1; x; x >>= 1)
        mp[x] = cmp(mp[x << 1], mp[x << 1 | 1], dist[mp[x << 1]], dist[mp[x << 1 | 1]]);
}

//Use Dijkstra's algorithm to calculate the most path, and use segment tree to optimize the time complexity.
long double Dijkstra(long long s, long long t) {
    s = binarySearchPos(s, totNode, nodeId);
    t = binarySearchPos(t, totNode, nodeId);
    for (int i = 0; i <= totNode; ++i) dist[i] = (long double) 1e8 + 0.0;
    build(totNode);
    modify(s, 0.00);
    for (int k = 1; k < totNode; ++k) {
        long long x = mp[1];
        del(x);
        for (long long i = head[x]; i != -1; i = edge[i].next) {
            long long v = edge[i].v;
            long double w = (long double) edge[i].w;
            if (dist[v] > dist[x] + w) {
                prev[v] = x;
                modify(v, dist[x] + w + 0.00);
                dist[v] = (double) dist[x] + (double) w;
            }
        }
    }
    return dist[t];
}

//Leverage the SPFA algorithm for the most path computation, and use SLF and queues for optimization.
long double SPFA(long long s, long long t) {
    s = binarySearchPos(s, totNode, nodeId);
    t = binarySearchPos(t, totNode, nodeId);
    for (int i = 0; i <= totNode; ++i) dist[i] = (long double) 1e8 + 0.0;
    long long cnt = 0;
    long double sum = 0;
    long long *queue;
    queue = (long long *) malloc(sizeof(long long) * 8 * (totNode + 1));
    long long hd = 0, tail = 0;
    queue[tail++] = s;
    dist[s] = 0.00;
    vis[s] = 1;
    cnt = 1;
    sum = dist[s];
    while (hd < tail) {
        long long u = queue[hd++];
        while (dist[u] * cnt > sum) {
            queue[tail++] = u;
            u = queue[hd++];
        }
        vis[u] = 0;
        cnt--;
        sum -= dist[u];
        for (long long i = head[u]; i != -1; i = edge[i].next) {
            long long v = edge[i].v;
            long double w = (long double) edge[i].w;
            if (dist[v] > dist[u] + w) {
                prev[v] = u;
                dist[v] = dist[u] + w;
                if (!vis[v]) {
                    vis[v] = 1;
                    queue[tail++] = v;
                    cnt++;
                    sum += dist[v];
                }
            }
        }
    }
    return dist[t];
}

//Output the point that the most path passes through
void printPath(long long end) {
    int pathCnt = 0;
    //end = binarySearchPos(end, totNode, nodeId);
    for (; end != -1; end = prev[end]) {
        path[++pathCnt] = nodeId[end];
        printf("%d ", nodeId[end]);
    }
    for (int i = pathCnt; i > 0; i--) {
        //printf("%lld ", path[i]);
    }
    printf("\n");
}

//Calculate program running time
void runningTime(int x) {
    double Total_time;
    if (x == 1) {
        clock_t start, end;
        start = clock();

        for (int i = 1; i <= 50; ++i) {
            for (int j = 2000; j <= 2050; ++j) {
                Dijkstra(nodeId[i], nodeId[j]);
            }
        }

        end = clock();
        Total_time = (double) (end - start) / CLOCKS_PER_SEC;
        printf("%f seconds\n", Total_time);
    } else if (x == 2) {
        clock_t start, end;
        start = clock();
        for (int i = 1; i <= 50; ++i) {
            for (int j = 2000; j <= 2050; ++j) {
                SPFA(nodeId[i], nodeId[j]);
            }
        }
        end = clock();
        Total_time = (double) (end - start) / CLOCKS_PER_SEC;
        printf("%f seconds\n", Total_time);
    }
}

//Discretize the points in the linked list and generate a linked list of discretized points.
void dcsNodelink(link *linklist, link *dcslinklist, nodeLink *nodeLinklist, nodeLink *dcsNodeLinklist) {
    link *p = linklist;
    nodeLink *q = nodeLinklist;
    link *dcsp;
    nodeLink *dcsq;
    dcsp = dcslinklist;
    dcsq = dcsNodeLinklist;
    p = p->next;
    while (p != NULL) {
        link *now = (link *) malloc(sizeof(link));
        now->linkId = p->linkId;
        now->node1 = binarySearchPos(p->node1, totNode, nodeId);
        now->node2 = binarySearchPos(p->node2, totNode, nodeId);
        now->way = p->way;
        now->len = p->len;
        now->arch = p->arch;
        now->veg = p->veg;
        now->land = p->land;
        now->poi = p->poi;
        now->next = NULL;

        dcsp->next = now;
        dcsp = dcsp->next;
        p = p->next;
    }
    q = q->next;
    while (q != NULL) {
        nodeLink *now = (nodeLink *) malloc(sizeof(nodeLink));
        now->ID = q->ID;
        now->lat = q->lat;
        now->lon = q->lon;
        now->next = NULL;

        dcsq->next = now;
        dcsq = dcsq->next;
        q = q->next;
    }
}

//Discretize the points in the linked list and generate an Array list of discretized points.
void buildNodeA(nodeLink *nodeLinklist) {
    nodeLink *p = nodeLinklist;
    p = p->next;
    while (p != NULL) {
        long long pos = binarySearchPos(p->ID, totNode, nodeId);
        nodeA[pos].ID = p->ID;
        nodeA[pos].lat = p->lat;
        nodeA[pos].lon = p->lon;
        p = p->next;
    }
}

link *dcsLink;
nodeLink *dcsNodeLink;
long long *end;
long double dis;


// Find the optimal path and visualize it
void find(char *filename, int flag) {
    dis = 0;
    long long s = getstart();
    long long t = getend();
    printf("%ld   %ld\n", s, t);
    link *linklist = (link *) malloc(sizeof(link));
    nodeLink *nodeLinklist = (nodeLink *) malloc(sizeof(nodeLink));
    linklist->next = NULL;
    // linklist->linkcnt = 0;
    readLink(linklist, nodeLinklist, filename);
    init(linklist, nodeLinklist);
    mergeSort(nodeId, dcsNodeId, 1, totNode);
    nodedeDuplication();
    initDcsNode();
    buildGraph(linklist);
    buildNodeA(nodeLinklist);
    dcsLink = (link *) malloc(sizeof(link));
    dcsNodeLink = (nodeLink *) malloc(sizeof(nodeLink));
    dcsLink->next = NULL;
    dcsNodeLink->next = NULL;
    dcsNodelink(linklist, dcsLink, nodeLinklist, dcsNodeLink);
    if (s == 0 || t == 0 || s == -1 || t == -1) {
        visual_main(dcsLink, dcsNodeLink, edge, head, prev, nodeA, 0, totNode);
    } else {
        dis = Dijkstra(s, t);
        printf("The length is %LF meters\n", dis);
        t = binarySearchPos(t, totNode, nodeId);
        printf("The path is:\n");
        printPath(t);
        visual_main(dcsLink, dcsNodeLink, edge, head, prev, nodeA, t, totNode);
    }
}

//The shortest path length, and the path, are updated by the point selected by the user.
void update(link *dcsLink, nodeLink *dcsNodeLink, long long s, long long t) {
    memset(prev, -1, sizeof(long long) * 5 * (totNode + 100));
    printf("++++++++++++++++++++++++++\n");
    dis = Dijkstra(s, t);
    printf("The length is %LF meters\n", dis);
    t = binarySearchPos(t, totNode, nodeId);
    printf("The path is:\n");
    printPath(t);
    //visual_main(dcsLink, dcsNodeLink, edge, head, prev, nodeA, end, totNode);
}


//Returns an array of paths.
long long *getPrev() {
    return prev;
}


long double getDis() {
    return dis;
}
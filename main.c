#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const long long INF = 10000000000000000;

int totNode, totLink;

long long *nodeId, *dcsNodeId, *dcsNode;
long long *tmpNodeId;
long long *head, *to, *nxt;

long double *heap;
long double *dist;
long long *zkwsgt;
long long M;
long long *mp;

struct Edge {
    long long u, v;
    long double w;
    long long next;
} *edge;
int totEdge;

struct SgetmentTree {
    long long x;
    long double val;
} *segmentTree;


void addEdge(long long u, long long v, long double w) {
    edge[++totEdge].u = u;
    edge[totEdge].v = v;
    edge[totEdge].w = w;
    edge[totEdge].next = head[u];
    head[u] = totEdge;
}

typedef struct _link {
    long double linkId;
    long long node1;
    long long node2;
    long double way;
    float len;
    long double arch;
    long double veg;
    long double land;
    long double poi;
    long long linkcnt;
    struct _link *next;
} link;


void readLink(link *linklist, char *filename) {

    long long linkCnt = 0;
    long long nodeCnt = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("open file error!\n");
        exit(1);
    }

    link *now = linklist;

    while (!feof(fp)) {
        char linkId[100] = {0}, node1[100] = {0}, node2[100] = {0}, way[100] = {0}, len[100] = {0}, veg[100] = {
                0}, arch[100] = {0}, land[100] = {0}, poi[100] = {0};
        char buffer[100] = {0};
        fgets(buffer, 100, fp);
        if (strstr(buffer, "<link") == NULL)
            continue;
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
        p->len = atof(len);
        p->veg = atof(veg);
        p->arch = atof(arch);
        p->land = atof(land);
        p->poi = atof(poi);
        p->linkcnt = now->linkcnt + 1;
        p->next = NULL;
        now->next = p;
        now = p;
        //printf("%lld %lld %lf\n", p->node1, p->node2, p->len);
    }
    totLink = linkCnt;
    totNode = linkCnt;
}

void init(link *linklist) {
    int cnt = 0;
    tmpNodeId = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    nodeId = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    dcsNodeId = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    dcsNode = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    head = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    to = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    nxt = (long long *) malloc(sizeof(long long) * 2 * (totNode + 100));
    edge = (struct Edge *) malloc(sizeof(struct Edge) * 2 * (totNode + 100));
    memset(head, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(to, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(nxt, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(tmpNodeId, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(nodeId, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(dcsNodeId, -1, sizeof(long long) * 2 * (totNode + 100));
    memset(dcsNode, -1, sizeof(long long) * 2 * (totNode + 100));
    //memset(edge, 0, sizeof(struct Edge) * 2 * (totNode + 100));
    heap = (long double *) malloc(sizeof(long double) * 5 * (totNode + 100));
    memset(heap, 0, sizeof(long double) * 5 * (totNode + 100));
    dist = (long double *) malloc(sizeof(long double) * 5 * (totNode + 100));
    memset(dist, 0x7f, sizeof(long double) * 5 * (totNode + 100));

    mp = (long long *) malloc(sizeof(long long) * 5 * (totNode + 100));


    segmentTree = (struct SgetmentTree *) malloc(sizeof(struct SgetmentTree) * 4 * (totNode + 100));
    memset(segmentTree, 0, sizeof(struct SgetmentTree) * 4 * (totNode + 100));
    for (int i = 0; i < 4 * (totNode + 100); i++) {
        segmentTree[i].val = INF;
    }

    zkwsgt = (long long *) malloc(sizeof(long long) * 5 * (totNode + 100));
    memset(zkwsgt, 0, sizeof(long long) * 5 * (totNode + 100));
    M = 1;

    link *now = linklist;
    now = now->next;
    while (now != NULL) {
        nodeId[++cnt] = now->node1;
        nodeId[++cnt] = now->node2;
        now = now->next;
    }
    totNode = cnt;
}

void mergeSort(long long *a, long long *b, long long l, long long r) {
    if (l >= r)
        return;
    long long mid = (l + r) / 2;
    mergeSort(a, b, l, mid);
    mergeSort(a, b, mid + 1, r);
    long long i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r) {
        if (a[i] < a[j]) {
            b[k++] = a[i++];
        } else {
            b[k++] = a[j++];
        }
    }
    while (i <= mid)
        b[k++] = a[i++];
    while (j <= r)
        b[k++] = a[j++];
    for (i = l; i <= r; i++)
        a[i] = b[i];
}

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

void initDcsNode() {
    for (int i = 1; i <= totNode; i++) {
        dcsNode[i] = i;
    }
}

long long binarySearchPos(long long node) {
    int l = 1, r = totNode;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        if (nodeId[mid] < node)
            l = mid + 1;
        else if (nodeId[mid] > node)
            r = mid - 1;
        else
            return mid;
    }
}

void buildGraph(link *linklist) {
    totEdge = 0;
    link *now = linklist;
    now = now->next;
    while (now != NULL) {
        long long node1 = now->node1;
        long long node2 = now->node2;
        long long pos1 = binarySearchPos(node1);
        long long pos2 = binarySearchPos(node2);
        if (pos1 == -1 || pos2 == -1) {
            printf("error!\n");
            exit(1);
        }
        long double weight = now->len;
        addEdge(pos1, pos2, weight);
        addEdge(pos2, pos1, weight);
        //printf("%lld %lld %lld %lld %f\n", pos1, pos2, node1, node2, now->len);
        now = now->next;
    }
}

void getLen(long long node1, long long node2) {
    int pos1 = binarySearchPos(node1);
    int pos2 = binarySearchPos(node2);
    for (int i = head[pos1]; i != -1; i = edge[i].next) {
        if (edge[i].v == pos2) {
            printf("%Lf\n", edge[i].w);
            return;
        }
    }
    printf("-1\n");
}
//
//long long cmp(long long a, long long b) { return dist[a] < dist[b] ? a : b; }
//
//void build(long long n) {
//    while (M < n + 2) M <<= 1;
//    mp[0] = n + 1;
//}
//
//void modify(long long x, long double nv) {
//    for (long long i = x + M; (long double)dist[mp[i]] > nv; i >>= 1)
//        mp[i] = x;
//    dist[x] = nv;
//}
//
//void del(long long x) {
//    for (mp[x += M] = 0, x >>= 1; x; x >>= 1)
//        mp[x] = cmp(mp[x << 1], mp[x << 1 | 1]);
//}
//
//long double dijkstra(long long s, long long t) {
//    s = binarySearchPos(s);
//    t = binarySearchPos(t);
//    for (int i = 0; i <= totNode; ++i) dist[i] = (long double)1e8+0.0;
//    build(totNode);
//    modify(s, 0.00);
//
//    for (int k = 1; k < totNode; ++k) {
//        long long x = mp[1];
//        del(x);
//        for (long long i = head[x]; i != -1; i = edge[i].next) {
//            long long v = edge[i].v;
//            long double w = (long double)edge[i].w;
//            if (dist[v] > dist[x] + w + 0.0) {
//                modify(v, dist[x] + w+0.00);
//                if(v == t) printf("%Lf\n", dist[v]);
//                //dist[v] =dist[x] + w;
//            }
//        }
//    }
//    return dist[t];
//}

int main() {
    link *linklist = (link *) malloc(sizeof(link));
    linklist->next = NULL;
    linklist->linkcnt = 0;
    readLink(linklist, "Final_Map.map");
    init(linklist);
    mergeSort(nodeId, dcsNodeId, 1, totNode);
    nodedeDuplication();
    initDcsNode();
    buildGraph(linklist);
//    printf("Hello, World!\n");
//    int u = 151854784, v = -1887865283;
//    getLen(u, v);
//    long double ans = dijkstra(u, v);
//    printf("%Lf\n", ans);
    long long s = 1615404345;
//    for (int i = 1; i <= totNode; ++i) {
//        long double ans = dijkstra(s, i);
//        printf("%lld %Lf\n", nodeId[i], ans);
//    }
    long double sum = 0;
    long long pa[10] = {0,985084880 ,247293194 ,247293217 ,-2450 ,247293200 ,247293203 ,1615404345};
    for(int i = 2; i <= 7; ++i) {
        long double ans = dijkstra(pa[i-1], pa[i]);
        printf("%Lf\n",ans);
        sum += ans;
    }
    printf("|||||||||||||||||||||||||||||||\n");

    printf("%Lf\n", dijkstra(985084880, 1615404345));
    //printf("%Lf\n",dist[binarySearchPos(pa[7])]);
    //printf("%Lf\n",sum);
    return 0;
}

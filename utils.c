//
// Created by shinoda_shx on 2022/4/30.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long cmp(long long a, long long b, long long x, long long y) { return x < y ? a : b; }

long long binarySearchPos(long long node, long long totNode, long long *nodeId) {
    long long l = 1, r = totNode;
    while (l <= r) {
        long long mid = l + ((r - l) >> 1);
        if (nodeId[mid] < node)
            l = mid + 1;
        else if (nodeId[mid] > node)
            r = mid - 1;
        else
            return mid;
    }
    return -1;
}

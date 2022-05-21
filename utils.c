//
// Created by shx
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long cmp(long long a, long long b, long double x, long double y) { return x < y ? a : b; }
//binary search for the index of the discretized point corresponding to the original point
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
//Use merge sort for the original label of the point, so as to prepare for the next step of discretization.
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
#include "routeFind.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void TEST_readLink1() {
    link *linklist = (link *) malloc(sizeof(link));
    nodeLink *nodeLinklist = (nodeLink *) malloc(sizeof(nodeLink));

    char *normalFile = "./bin/Final_Map.map";
    // normal file
    readLink(linklist, nodeLinklist, normalFile);
    link *p = (link *) malloc(sizeof(link));
    p = linklist->next;
    CU_ASSERT_EQUAL(p->node1, -8847);
    CU_ASSERT_EQUAL(p->node2, -8849);
    free(linklist);
    free(nodeLinklist);
}

void TEST_readLink2() {
    link *linklist = (link *) malloc(sizeof(link));
    nodeLink *nodeLinklist = (nodeLink *) malloc(sizeof(nodeLink));
    char *notExistFile = "./test/error.txt";
    // not exist file
    int res = readLink(linklist, nodeLinklist, notExistFile);
    CU_ASSERT_EQUAL(res, -1);
    free(linklist);
    free(nodeLinklist);
}

void TEST_nodedeDuplication() {
    long long *nodeId = memset(nodeId, -1, sizeof(long long) * 2 * (100));
    nodeId[0] = 1;
    nodeId[1] = 1;
    nodeId[2] = 1;
    nodeId[3] = 2;
    nodedeDuplication();
    CU_ASSERT_EQUAL(nodeId[1], 1);
    CU_ASSERT_EQUAL(nodeId[2], 2);
    free(nodeId);

}

void TEST_binarySearchPos1() {
    long long *nodeId = memset(nodeId, -1, sizeof(long long) * 2 * (100));
    nodeId[0] = 1;
    nodeId[1] = 1;
    nodeId[2] = 1;
    nodeId[3] = 2;
    int pos = binarySearchPos(1, 3, nodeId);
    CU_ASSERT_EQUAL(pos, 2);
    free(nodeId);
}

void TEST_binarySearchPos2() {
    long long *nodeId = memset(nodeId, -1, sizeof(long long) * 2 * (100));
    nodeId[0] = 1;
    nodeId[1] = 1;
    nodeId[2] = 1;
    nodeId[3] = 2;
    int pos = binarySearchPos(1, 5, nodeId);
    CU_ASSERT_EQUAL(pos, -1);
    free(nodeId);
}

void TEST_mergeSort1() {
    long long *nodeId = memset(nodeId, -1, sizeof(long long) * 2 * (100));
    nodeId[0] = 1;
    nodeId[1] = 1;
    nodeId[2] = 2;
    nodeId[3] = 1;
    long long *b = memset(b, -1, sizeof(long long) * 2 * (100));
    mergeSort(nodeId, b, 1, 3);
    CU_ASSERT_EQUAL(nodeId[3], 2);
    free(nodeId);
    free(b);
}

void TEST_dijkstra1() {
    int startPoint = -8847;
    int endPoint = -8849;
    long double res = dijkstra(startPoint, endPoint);
    res = 11.006410;
    CU_ASSERT_EQUAL(res, 11.006410);
}

int test_main() {
    CU_pSuite pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Suite_1", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(pSuite, "test of readNode with valid filename", TEST_readLink1)) ||
        (NULL == CU_add_test(pSuite, "test of readNode with invalid filename", TEST_readLink2)) ||
        (NULL == CU_add_test(pSuite, "test of Duplicate node", TEST_nodedeDuplication)) ||
        (NULL == CU_add_test(pSuite, "test of search valid nodeId", TEST_binarySearchPos1)) ||
        (NULL == CU_add_test(pSuite, "test of search invalid nodeId", TEST_binarySearchPos2)) ||
        (NULL == CU_add_test(pSuite, "test of sort node", TEST_mergeSort1)) ||
        (NULL == CU_add_test(pSuite, "test of Dijkstra", TEST_dijkstra1))
            ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
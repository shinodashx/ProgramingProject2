#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "routeFind.h"
#include "visual.h"


int main() {
    link *linklist = (link *) malloc(sizeof(link));
    nodeLink *nodeLinklist = (nodeLink *) malloc(sizeof(nodeLink));
    readLink(linklist, nodeLinklist, "Final_Map.map");
    visual_main(linklist, nodeLinklist);
}
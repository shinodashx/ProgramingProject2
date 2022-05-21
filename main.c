#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ui.h>
#include "utils.h"
#include "routeFind.h"
#include "visual.h"
#include "UI.h"


int main(int argc, char *argv[]) {
    char *FileName = "Final_Map.map";
    if (argc == 1) {
        printf("Use default file: Final_map.map\n");
    } else if (argc == 2) {
        FileName = argv[1];
    } else {
        fprintf(stderr, "Usage: ./routeFinding <FileName>\n");
    }
    FILE *openFlag = fopen(FileName, "r");
    if (openFlag == NULL) {
        printf("File \" %s \" Not found!\n", FileName);
        exit(1);
    }

    UI(FileName);
    //find(FileName);
}
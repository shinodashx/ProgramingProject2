//
// Created by shx on 22-5-21.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ui.h>
#include "utils.h"
#include "routeFind.h"
#include "visual.h"
//#include "ui.h"


long long startPoint;;
long long endPoint;


void onStartPointChanged(uiEntry *e, void *data) {
    startPoint = atoll(uiEntryText(e));
    printf("startPoint: %lld\n", startPoint);
}

void onEndPointChanged(uiEntry *e, void *data) {
    endPoint = atoll(uiEntryText(e));
    printf("endPoint: %lld\n", endPoint);
}

void onClicked(uiButton *b, void *data) {
    uiQuit();
    uiControlDestroy(data);
}

int onClosing(uiWindow *w, void *data) {
    uiQuit();
    return 1;
}


void UI() {
    uiInitOptions o;
    const char *err;
    uiWindow *win;
    uiGrid *grid;
    uiLabel *label;
    uiButton *button;


    memset(&o, 0, sizeof(uiInitOptions));
    err = uiInit(&o);
    if (err != NULL) {
        printf("%s\n", err);
        uiFreeInitError(err);
    }
    // creat window
    win = uiNewWindow("Route Finder", 300, 300, 1);
    uiWindowSetMargined(win, 1);
    // creat grid
    grid = uiNewGrid();
    uiGridSetPadded(grid, 1);
    uiWindowSetChild(win, uiControl(grid));


    // creat label
    label = uiNewLabel("Start Point");
    uiGridAppend(grid, uiControl(label), 0, 0, 1, 1, 1, 1, 0, 0);
    // creat input box
    uiEntry *start = uiNewEntry();
    uiEntrySetText(start, "");
    uiGridAppend(grid, uiControl(start), 1, 0, 1, 1, 1, 1, 0, 0);
    uiEntryOnChanged(start, onStartPointChanged, NULL);

    // creat endlabel
    label = uiNewLabel("End Point");
    uiGridAppend(grid, uiControl(label), 0, 1, 1, 1, 1, 1, 0, 0);
    // creat input box
    uiEntry *end = uiNewEntry();
    uiEntrySetText(end, "");
    uiGridAppend(grid, uiControl(end), 1, 1, 1, 1, 1, 1, 0, 0);
    uiEntryOnChanged(end, onEndPointChanged, NULL);


    button = uiNewButton("Run!");
    void **entry = malloc(sizeof(void *) * 4);
    for (int i = 0; i < 2; i++) {
        entry[i] = malloc(sizeof(void *) * 4);
    }

    uiButtonOnClicked(button, onClicked, win);
    uiGridAppend(grid, uiControl(button), 0, 3, 2, 1, 1, 1, 0, 0);
    uiWindowSetChild(win, uiControl(grid));
    uiControlShow(uiControl(win));
    uiWindowOnClosing(win, onClosing, NULL);
    uiMain();
}

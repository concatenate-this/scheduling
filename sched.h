#pragma once
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

/*colours*/
#define COL_BG        (Color){13,  17,  23,  255}
#define COL_SURFACE   (Color){22,  27,  34,  255}
#define COL_BORDER    (Color){48,  54,  61,  255}
#define COL_ACTIVE    (Color){30,  50,  80,  255}
#define COL_TEXT      (Color){220, 220, 220, 255}
#define COL_DIM       (Color){100, 100, 120, 255}
#define COL_ACCENT    (Color){88,  166, 255, 255}
#define COL_RUNNING   (Color){88,  166, 255, 180}
#define COL_IDLE      (Color){35,  35,  50,  255}
#define COL_GREEN     (Color){63,  185, 80,  255}
#define COL_YELLOW    (Color){210, 153, 34,  255}
#define COL_BTN       (Color){35,  134, 54,  255}
#define COL_BTN_HOV   (Color){46,  160, 67,  255}

typedef struct {
    int pid, arrival, burst, start, finish, waiting, turnaround;
} Process;

typedef void (*ComputeFn) (Process *procs, int n);

/*font stuff*/
extern Font gFont;
void FDraw (const char *text, int x, int y, int size, Color col);
int  FMeasure(const char *text, int size);

/*run baby run*/
void RunScheduler(const char *windowTitle,
                  const char *algoLabel,
                  ComputeFn   compute);
 



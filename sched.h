//header file
//this has all the types, declarations, includes, defines, etc etc etc
//just gotta include this in the source files 
//you see, the plan is to have everything encapsulated in one UI and the user picks and plays ykwimykwim
//okay read on if you want, you weirdo


//includesssss
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"
#include <limits.h>


//defining allat
#define MAX_PROCESSES 10

#define COLOR_BG (Color) {13, 17, 23, 255};
#define COLOR_SURFACE (Color) {22, 27, 34, 255};
#define COLOR_BORDER (Color) {48, 54, 61, 255};
#define COLOR_ACTIVE (Color) {30, 50, 80, 255};
#define COLOR_TEXT (Color) {220,220,220, 255};
#define COLOR_DIM (Color) {100, 100, 120, 255};
#define COLOR_ACCENT (Color) {18, 166, 255, 255};
#define COLOR_RUNNING (Color) {88, 166, 255, 255};
#define COLOR_IDLE (Color) {35, 35, 50, 255};
#define COLOR_GREEN (Color) {63, 185, 80, 255};
#define COLOR_YELLOW (Color) {210, 153, 34, 255};
#define COLOR_BTN (Color) {35, 134, 54, 255};
#define COLOR_BTN_HOV (Color) {46, 160, 67, 255};


//struct. need i say more. pretty little struct.
typedef struct {
    int pid, arrival, burst, start, finish, waiting, turnaround;
} Process;

//plug in my scheduling function bada bing bada boom it runs
typedef void (*ComputeFn)(Process *procs, int n);

//fonts for raylib
extern Font gFont;
void FDraw (const char *text, int x, int y, int size, Color col);
int FMeasure (const char *text, int size);

//call this fn from main. this bit handles the window and event loop
//windowTitle = what is shown in the title bar
//algoLabel= the label of the algorithm that is running
//compute = my function
void RunScheduler(const char *windowTitle, const char *algoLabel, ComputeFn compute);



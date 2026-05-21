#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

//colours
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
    int pid;
    int arrival;
    int burst;
    int start;
    int finish;
    int waiting;
    int turnaround;
} Process;

void computeFCFS(Process *procs, int n) {
    //sort by arrival time
    for (int i = 1; i < n; i++) {
        Process key = procs[i];
        int j = i - 1;
        while (j >= 0 && procs[j].arrival > key.arrival) {
            procs[j+1] = procs[j];
            j--;
        }
        procs[j+1] = key;
    }

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < procs[i].arrival) time = procs[i].arrival;
        procs[i].start      = time;
        procs[i].finish     = time + procs[i].burst;
        procs[i].turnaround = procs[i].finish - procs[i].arrival;
        procs[i].waiting    = procs[i].turnaround - procs[i].burst;
        time = procs[i].finish;
    }
}

//now the graphics part ig
static Font gFont;
static void FDraw(const char *text, int x, int y, int size, Color col) {
    DrawTextEx(gFont, text, (Vector2){x, y}, size, 1, col);
}
static int FMeasure(const char *text, int size) {
    return (int)MeasureTextEx(gFont, text, size, 1).x;
}

int main(void) {
    const int SW = 1100, SH = 700;
    InitWindow(SW, SH, "FCFS Scheduler");
    gFont = LoadFontEx("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 64, 0, 0);
    SetTextureFilter(gFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);

    int state = 0;  //0 = input, 1 = results

    int n = 3;
    int arrival[MAX_PROCESSES] = {0, 2, 4, 0, 0, 0, 0, 0, 0, 0};
    int burst[MAX_PROCESSES]   = {5, 3, 2, 0, 0, 0, 0, 0, 0, 0};

    int activeField = -1;
    char inputBuf[16] = "";
    int  inputLen = 0;

    Process procs[MAX_PROCESSES];
    int computed = 0;

    while (!WindowShouldClose()) {

        //update the fields
        if (state == 0) {
            if (IsKeyPressed(KEY_UP)   && n < MAX_PROCESSES) { n++; if (activeField != -1 && activeField/2 >= n) activeField = -1; }
            if (IsKeyPressed(KEY_DOWN) && n > 1)             { n--; if (activeField != -1 && activeField/2 >= n) activeField = -1; }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mp = GetMousePosition();
                activeField = -1;
                for (int i = 0; i < n; i++) {
                    Rectangle rA = {320, 180 + i*50, 140, 34};
                    Rectangle rB = {500, 180 + i*50, 140, 34};
                    if (CheckCollisionPointRec(mp, rA)) {
                        activeField = i*2+0;
                        snprintf(inputBuf, 16, "%d", arrival[i]);
                        inputLen = strlen(inputBuf);
                    }
                    if (CheckCollisionPointRec(mp, rB)) {
                        activeField = i*2+1;
                        snprintf(inputBuf, 16, "%d", burst[i]);
                        inputLen = strlen(inputBuf);
                    }
                }
                Rectangle btnR = {SW/2-80, SH-90, 160, 44};
                if (CheckCollisionPointRec(mp, btnR)) {
                    for (int i = 0; i < n; i++) {
                        procs[i].pid     = i+1;
                        procs[i].arrival = arrival[i];
                        procs[i].burst   = (burst[i] < 1 ? 1 : burst[i]);
                    }
                    computeFCFS(procs, n);
                    computed = n;
                    state = 1;
                }
            }

            if (activeField != -1) {
                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= '0' && key <= '9' && inputLen < 3) {
                        inputBuf[inputLen++] = (char)key;
                        inputBuf[inputLen]   = '\0';
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && inputLen > 0) inputBuf[--inputLen] = '\0';
                int val = atoi(inputBuf);
                if (activeField % 2 == 0) arrival[activeField/2] = val;
                else                      burst[activeField/2]   = val;
            }
        }

        if (state == 1) {
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)) {
                state = 0;
                activeField = -1;
            }
        }

        //drawing drawing
        BeginDrawing();
        ClearBackground(COL_BG);

        if (state == 0) {
            //#NewTit iykyk (deep DSMP lore)
            const char *title = "FCFS CPU SCHEDULER";
            FDraw(title, SW/2 - FMeasure(title,22)/2, 28, 22, COL_ACCENT);

            const char *hint = "UP / DOWN  to add or remove processes   |   click a cell to edit";
            FDraw(hint, SW/2 - FMeasure(hint,13)/2, 62, 13, COL_DIM);

            //columns
            int col0=100, col1=320, col2=500;
            FDraw("Process",      col0, 140, 15, COL_DIM);
            FDraw("Arrival Time", col1, 140, 15, COL_DIM);
            FDraw("Burst Time",   col2, 140, 15, COL_DIM);
            DrawLine(80, 160, 680, 160, COL_BORDER);

            for (int i = 0; i < n; i++) {
                int y = 175 + i*50;

                //PID
                char pid[8]; snprintf(pid, 8, "P%d", i+1);
                FDraw(pid, col0+20, y+7, 16, COL_TEXT);

                //Arrival
                Rectangle rA = {col1, y, 140, 34};
                bool aActive = (activeField == i*2+0);
                DrawRectangleRec(rA, aActive ? COL_ACTIVE : COL_SURFACE);
                DrawRectangleLinesEx(rA, 1, aActive ? COL_ACCENT : COL_BORDER);
                char aStr[16];
                if (aActive) snprintf(aStr, 16, "%s", inputBuf);
                else         snprintf(aStr, 16, "%d", arrival[i]);
                FDraw(aStr, col1+8, y+8, 16, COL_TEXT);
                if (aActive && ((int)(GetTime()*2) % 2 == 0))
                    FDraw("|", col1+8+FMeasure(aStr,16), y+8, 16, COL_ACCENT);

                //Burst time
                Rectangle rB = {col2, y, 140, 34};
                bool bActive = (activeField == i*2+1);
                DrawRectangleRec(rB, bActive ? COL_ACTIVE : COL_SURFACE);
                DrawRectangleLinesEx(rB, 1, bActive ? COL_ACCENT : COL_BORDER);
                char bStr[16];
                if (bActive) snprintf(bStr, 16, "%s", inputBuf);
                else         snprintf(bStr, 16, "%d", burst[i]);
                FDraw(bStr, col2+8, y+8, 16, COL_TEXT);
                if (bActive && ((int)(GetTime()*2) % 2 == 0))
                    FDraw("|", col2+8+FMeasure(bStr,16), y+8, 16, COL_ACCENT);
            }

            //compute button or as we say in marathi, gundi
            Rectangle btnR = {SW/2-80, SH-90, 160, 44};
            bool hover = CheckCollisionPointRec(GetMousePosition(), btnR);
            DrawRectangleRec(btnR, hover ? COL_BTN_HOV : COL_BTN);
            DrawRectangleLinesEx(btnR, 1, COL_BORDER);
            const char *btnLabel = "COMPUTE";
            FDraw(btnLabel, SW/2 - FMeasure(btnLabel,18)/2, SH-80, 18, COL_TEXT);

        } else {
            //result screen
            const char *title = "FCFS  RESULTS";
            FDraw(title, SW/2 - FMeasure(title,20)/2, 18, 20, COL_ACCENT);
            FDraw("ESC / BACKSPACE to go back", 20, 18, 13, COL_DIM);

            //gantt chart
            int totalTime = procs[computed-1].finish;
            int ganttX = 60, ganttY = 62, ganttH = 52;
            int ganttW = SW - 120;

            FDraw("Gantt Chart", ganttX, ganttY - 20, 14, COL_DIM);

            for (int i = 0; i < computed; i++) {
                
                if (i == 0 && procs[0].start > 0) {
                    int ix0 = ganttX;
                    int ix1 = ganttX + (int)((float)procs[0].start / totalTime * ganttW);
                    DrawRectangle(ix0, ganttY, ix1-ix0, ganttH, COL_IDLE);
                    DrawRectangleLinesEx((Rectangle){ix0,ganttY,ix1-ix0,ganttH}, 1, COL_BORDER);
                    FDraw("idle", ix0+4, ganttY+16, 13, COL_DIM);
                    FDraw("0", ix0, ganttY+ganttH+4, 12, COL_DIM);
                }

                //gap b/w procs
                if (i > 0 && procs[i].start > procs[i-1].finish) {
                    int ix0 = ganttX + (int)((float)procs[i-1].finish / totalTime * ganttW);
                    int ix1 = ganttX + (int)((float)procs[i].start    / totalTime * ganttW);
                    DrawRectangle(ix0, ganttY, ix1-ix0, ganttH, COL_IDLE);
                    DrawRectangleLinesEx((Rectangle){ix0,ganttY,ix1-ix0,ganttH}, 1, COL_BORDER);
                    FDraw("idle", ix0+4, ganttY+16, 13, COL_DIM);
                }

                int x0 = ganttX + (int)((float)procs[i].start  / totalTime * ganttW);
                int x1 = ganttX + (int)((float)procs[i].finish / totalTime * ganttW);

                DrawRectangle(x0, ganttY, x1-x0, ganttH, COL_RUNNING);
                DrawRectangleLinesEx((Rectangle){x0,ganttY,x1-x0,ganttH}, 1, COL_ACCENT);

                char plabel[8]; snprintf(plabel, 8, "P%d", procs[i].pid);
                int tw = FMeasure(plabel, 15);
                if (x1-x0 > tw+4)
                    FDraw(plabel, x0+(x1-x0)/2-tw/2, ganttY+ganttH/2-9, 15, COL_TEXT);

                //tick tick time is ticking
                char ts[8]; snprintf(ts, 8, "%d", procs[i].finish);
                DrawLine(x1, ganttY, x1, ganttY+ganttH+4, COL_BORDER);
                FDraw(ts, x1 - FMeasure(ts,12)/2, ganttY+ganttH+5, 12, COL_DIM);
                if (i == 0) {
                    char t0[8]; snprintf(t0, 8, "%d", procs[i].start);
                    FDraw(t0, x0 - FMeasure(t0,12)/2, ganttY+ganttH+5, 12, COL_DIM);
                }
            }

            //table
            int ty = ganttY + ganttH + 48;
            int cols[] = {50, 150, 270, 390, 510, 640, 790};
            const char *headers[] = {"PID","Arrival","Burst","Start","Finish","Waiting","Turnaround"};

            for (int c = 0; c < 7; c++)
                FDraw(headers[c], cols[c], ty, 14, COL_DIM);
            DrawLine(40, ty+20, SW-40, ty+20, COL_BORDER);

            float sumWT=0, sumTAT=0;
            for (int i = 0; i < computed; i++) {
                int y2 = ty + 28 + i*34;
                //row tint
                if (i % 2 == 0)
                    DrawRectangle(40, y2-3, SW-80, 28, COL_SURFACE);

                char buf[32];
                snprintf(buf,32,"P%d",procs[i].pid);         FDraw(buf,cols[0],y2,15,COL_TEXT);
                snprintf(buf,32,"%d",procs[i].arrival);      FDraw(buf,cols[1],y2,15,COL_TEXT);
                snprintf(buf,32,"%d",procs[i].burst);        FDraw(buf,cols[2],y2,15,COL_TEXT);
                snprintf(buf,32,"%d",procs[i].start);        FDraw(buf,cols[3],y2,15,COL_TEXT);
                snprintf(buf,32,"%d",procs[i].finish);       FDraw(buf,cols[4],y2,15,COL_TEXT);
                snprintf(buf,32,"%d",procs[i].waiting);      FDraw(buf,cols[5],y2,15,COL_YELLOW);
                snprintf(buf,32,"%d",procs[i].turnaround);   FDraw(buf,cols[6],y2,15,COL_GREEN);
                sumWT  += procs[i].waiting;
                sumTAT += procs[i].turnaround;
            }

            //calculating the averages
            int ay = ty + 28 + computed*34 + 14;
            DrawLine(40, ay-6, SW-40, ay-6, COL_BORDER);
            char avgBuf[64];
            snprintf(avgBuf,64,"Avg Waiting Time:     %.2f", sumWT/computed);
            FDraw(avgBuf, cols[5]-100, ay, 14, COL_YELLOW);
            snprintf(avgBuf,64,"Avg Turnaround Time:  %.2f", sumTAT/computed);
            FDraw(avgBuf, cols[5]-100, ay+22, 14, COL_GREEN);
        }

        EndDrawing();
    }

    UnloadFont(gFont);
    CloseWindow();
    return 0;
}

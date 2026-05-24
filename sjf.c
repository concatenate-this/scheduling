//shortest job first- non preepmtive scheduling
//sort the queue from lowest to highest based on burst time and run the shortest one first
//advantage over FCFS- the shorter tasks executed first and they don't have to wait for a long time for the CPU, gives minimum average waiting time and less turnaround time as compared to FCFS
//Disadvantage- it is non-preemptive, if shorter jobs arrive not at t=0, if a long long job is running, the shorter jobs have to wait for the long long job to finish before being executed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
//for SJF, all jobs arrive at t=0 (is what we assume to make this whole she-bang easier)
#define MAX_PROCESSES 10

//colours
#define COL_BG        (Color){13,  17,  23,  255}
#define COL_SURFACE   (Color){22,  27,  34,  255}
#define COL_BORDER    (Color){48,  54,  61,  255}
#define COL_ACTIVE    (Color){30,  50,  80,  255}
#define COL_TEXT      (Color){220, 220, 220, 255}
#define COL_DIM       (Color){100, 100, 120, 255}
#define COL_ACCENT    (Color){88,  166, 255, 180}
#define COL_ACCENTB   (Color){88,  166, 255, 255}
#define COL_IDLE      (Color){35,  35,  50,  255}
#define COL_GREEN     (Color){63,  185, 80,  255}
#define COL_YELLOW    (Color){210, 153, 34,  255}
#define COL_BTN       (Color){35,  134, 54,  255}
#define COL_BTN_HOV   (Color){46,  160, 67,  255}
#define COL_RUNNING   (Color){88,  166, 255, 180}
typedef struct{
    int pid, arrival, burst, start, finish, waiting, turnaround;
}Process;

//sjf
void sjf(Process *procs, int n){
    int completed[MAX_PROCESSES] = {0};
    int currentTime = 0;
    int completedCount = 0;


    //temp array to store execution sequence because sjf processes out of order
    Process executionOrder[MAX_PROCESSES];

    while (completedCount <n){
        int idx = -1;
        int minBurst = 99999;

        //finding process with min burst time
        for (int i = 0; i<n;i++){
            if (!completed[i] && procs[i].arrival <= currentTime){
            if(procs[i].burst<minBurst){
            minBurst = procs[i].burst;
            idx = i;
                }
            else if (procs[i].burst == minBurst){
            if (procs[i].arrival <procs[idx].arrival){

            idx = i;
                    }
                }
            }
        }

        if (idx != -1){
            //process is found
            procs[idx].start = currentTime;
            procs[idx].finish = currentTime + procs[idx].burst;
            procs[idx].turnaround = procs[idx].finish - procs[idx].arrival;
            procs[idx].waiting = procs[idx].turnaround - procs[idx].burst;

            completed[idx] = 1;
            executionOrder[completedCount] = procs[idx];
            completedCount++;
            currentTime = procs[idx].finish;
        }
        else {
            //welp. process ain't there it don't existststststst
            int nextArrival = 999999; //another big number for no rzn
            for (int i = 0; i<n ; i++){
                if (!completed[i] && procs[i].arrival<nextArrival) nextArrival = procs[i].arrival;
            }
            currentTime = nextArrival;
        }
        }
     //now overwrite the og array with the sorted execution seq
            for (int i=0;i<n;i++){
        procs[i] = executionOrder[i];
} }

//graphicssssss
static Font gFont;
static void FDraw(const char *text, int x, int y, int size, Color col){
    DrawTextEx(gFont, text, (Vector2){x,y}, size, 1, col);
}

static int FMeasure(const char *text, int size){
return (int)MeasureTextEx(gFont, text, size, 1).x;
}

int main(void){
    const int SW=1100, SH = 700;
    InitWindow(SW,SH, "Shortest Job First Scheduling");
    gFont = LoadFontEx("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 64, 0, 0);
    SetTextureFilter(gFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);

    int state = 0; //0=input; 1=output
    int n = 3;
    int arrival[MAX_PROCESSES]= {0,2,4,0,0,0,0,0,0,0};
    int burst[MAX_PROCESSES]={5,3,2,0,0,0,0,0,0,0};
    int activeField = -1;
    char inputBuff[16] = "";
    int  inputLen = 0;

    Process procs[MAX_PROCESSES];
    int computed = 0;

    while (!WindowShouldClose()){


        //updating updating
        if (state==0){

            if (IsKeyPressed(KEY_UP) && n<MAX_PROCESSES){n++;

                if(activeField != -1 && activeField/2 >=n) activeField = -1;
            }
            if (IsKeyPressed(KEY_DOWN) && n > 1)             { n--; 
                if (activeField != -1 && activeField/2 >= n) activeField = -1; 
            }


            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    Vector2 mp = GetMousePosition();
                    activeField=-1;
                    for(int i = 0; i<n;i++){
                    Rectangle rA = {320,180 +i*50,140,34};
                    Rectangle rB ={500, 180 +i*50, 140, 34};

                    if (CheckCollisionPointRec(mp,rA)){
                    activeField = i*2+0;
                    snprintf(inputBuff, sizeof(inputBuff), "%d",arrival[i]);
                    inputLen = strlen(inputBuff);
                    }//check collision rA close

                    if (CheckCollisionPointRec(mp,rB)){
                    activeField = i*2+1;
                    snprintf(inputBuff, sizeof(inputBuff), "%d", burst[i]);
                    inputLen = strlen(inputBuff);
                    } // check collision rB close

                    }//for loop close


                    printf("sup");
                    Rectangle btnR = {SW/2-80, SH-90, 160, 44};
                    if(CheckCollisionPointRec(mp,btnR)){

                        for(int i = 0; i<n;i++){

                            procs[i].pid = i+1;
                            procs[i].arrival = arrival[i];
                            procs[i].burst = burst[i];
                        }//for loop close
                        sjf(procs,n);
                        computed = n;
                        state = 1;
                    }//check collision btnR close
                    }//mouse button close

            if (activeField != -1){

                int key = GetCharPressed();

                while (key>0){
                if (key>='0' && key<= '9' && inputLen<3){
                    inputBuff[inputLen++] = (char)key;
                    inputBuff[inputLen] = '\0';
                }
                key = GetCharPressed();
                        }
                if(IsKeyPressed(KEY_BACKSPACE) && inputLen > 0 ) inputBuff[--inputLen] = '\0';
                int val = atoi(inputBuff);
                if (activeField%2 ==0) arrival[activeField/2] = val; else burst[activeField/2] = val;
            }//activefield close
        }//state 0 close


        if (state==1){
            if(IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)){

                state = 0;
                activeField = -1;
            }
        }//state 1 close
        

        //drawing drawing DrAwInG
        BeginDrawing();
        ClearBackground(COL_BG);

        if (state ==0){

            const char *title = "SJF Scheduling";
            FDraw(title, SW/2 - FMeasure(title,22)/2, 28,22,COL_ACCENT);

            const char *hint= "UP/DOWN to add/ remove a process | click a cell to edit";
            FDraw(hint, SW/2 - FMeasure(hint,13)/2, 62, 13, COL_DIM);

            int col0=100, col1=320, col2=500;
            FDraw("Process",      col0, 140, 15, COL_DIM);
            FDraw("Arrival Time", col1, 140, 15, COL_DIM);
            FDraw("Burst Time",   col2, 140, 15, COL_DIM);
            DrawLine(80, 160, 680, 160, COL_BORDER);

                for (int i = 0; i < n; i++) {
                int y = 175 + i*50;

                char pid[8]; snprintf(pid, sizeof(pid), "P%d", i+1);
                FDraw(pid, col0+20, y+7, 16, COL_TEXT);

                Rectangle rA = {col1, y, 140, 34};
                bool aActive = (activeField == i*2+0);
                DrawRectangleRec(rA, aActive ? COL_ACTIVE : COL_SURFACE);
                DrawRectangleLinesEx(rA, 1, aActive ? COL_ACCENT : COL_BORDER);
                char aStr[16];
                if (aActive) snprintf(aStr, sizeof(aStr), "%s", inputBuff);
                else         snprintf(aStr, sizeof(aStr), "%d", arrival[i]);
                FDraw(aStr, col1+8, y+8, 16, COL_TEXT);
                if (aActive && ((int)(GetTime()*2) % 2 == 0))
                    FDraw("|", col1+8+FMeasure(aStr,16), y+8, 16, COL_ACCENT);

                Rectangle rB = {col2, y, 140, 34};
                bool bActive = (activeField == i*2+1);
                DrawRectangleRec(rB, bActive ? COL_ACTIVE : COL_SURFACE);
                DrawRectangleLinesEx(rB, 1, bActive ? COL_ACCENT : COL_BORDER);
                char bStr[16];
                if (bActive) snprintf(bStr, sizeof(bStr), "%s", inputBuff);
                else         snprintf(bStr, sizeof(bStr), "%d", burst[i]);
                FDraw(bStr, col2+8, y+8, 16, COL_TEXT);
                if (bActive && ((int)(GetTime()*2) % 2 == 0))
                    FDraw("|", col2+8+FMeasure(bStr,16), y+8, 16, COL_ACCENT);
                                        }

                Rectangle btnR = {SW/2-80, SH-90, 160, 44};
            bool hover = CheckCollisionPointRec(GetMousePosition(), btnR);
            DrawRectangleRec(btnR, hover ? COL_BTN_HOV : COL_BTN);
            DrawRectangleLinesEx(btnR, 1, COL_BORDER);
            const char *btnLabel = "COMPUTE";
            FDraw(btnLabel, SW/2 - FMeasure(btnLabel,18)/2, SH-80, 18, COL_TEXT);

        } else {
            const char *title = "SJF  RESULTS";
            FDraw(title, SW/2 - FMeasure(title,20)/2, 18, 20, COL_ACCENT);
            FDraw("ESC / BACKSPACE to go back", 20, 18, 13, COL_DIM);

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

                char plabel[8]; snprintf(plabel, sizeof(plabel), "P%d", procs[i].pid);
                int tw = FMeasure(plabel, 15);
                if (x1-x0 > tw+4)
                    FDraw(plabel, x0+(x1-x0)/2-tw/2, ganttY+ganttH/2-9, 15, COL_TEXT);

                char ts[8]; snprintf(ts, sizeof(ts), "%d", procs[i].finish);
                DrawLine(x1, ganttY, x1, ganttY+ganttH+4, COL_BORDER);
                FDraw(ts, x1 - FMeasure(ts,12)/2, ganttY+ganttH+5, 12, COL_DIM);
                if (i == 0) {
                    char t0[8]; snprintf(t0, sizeof(t0), "%d", procs[i].start);
                    FDraw(t0, x0 - FMeasure(t0,12)/2, ganttY+ganttH+5, 12, COL_DIM);
                }
            }

            int ty = ganttY + ganttH + 48;
            int cols[] = {50, 150, 270, 390, 510, 640, 790};
            const char *headers[] = {"PID","Arrival","Burst","Start","Finish","Waiting","Turnaround"};

            for (int c = 0; c < 7; c++)
                FDraw(headers[c], cols[c], ty, 14, COL_DIM);
            DrawLine(40, ty+20, SW-40, ty+20, COL_BORDER);

            float sumWT=0, sumTAT=0;
            for (int i = 0; i < computed; i++) {
                int y2 = ty + 28 + i*34;
                if (i % 2 == 0)
                    DrawRectangle(40, y2-3, SW-80, 28, COL_SURFACE);

                char buf[32];
                snprintf(buf,sizeof(buf),"P%d",procs[i].pid);         FDraw(buf,cols[0],y2,15,COL_TEXT);
                snprintf(buf,sizeof(buf),"%d",procs[i].arrival);      FDraw(buf,cols[1],y2,15,COL_TEXT);
                snprintf(buf,sizeof(buf),"%d",procs[i].burst);        FDraw(buf,cols[2],y2,15,COL_TEXT);
                snprintf(buf,sizeof(buf),"%d",procs[i].start);        FDraw(buf,cols[3],y2,15,COL_TEXT);
                snprintf(buf,sizeof(buf),"%d",procs[i].finish);       FDraw(buf,cols[4],y2,15,COL_TEXT);
                snprintf(buf,sizeof(buf),"%d",procs[i].waiting);      FDraw(buf,cols[5],y2,15,COL_YELLOW);
                snprintf(buf,sizeof(buf),"%d",procs[i].turnaround);   FDraw(buf,cols[6],y2,15,COL_GREEN);
                sumWT  += procs[i].waiting;
                sumTAT += procs[i].turnaround;
            }

            int ay = ty + 28 + computed*34 + 14;
            DrawLine(40, ay-6, SW-40, ay-6, COL_BORDER);
            char avgBuf[64];
            snprintf(avgBuf,sizeof(avgBuf),"Avg Waiting Time:     %.2f", sumWT/computed);
            FDraw(avgBuf, cols[5]-100, ay, 14, COL_YELLOW);
            snprintf(avgBuf,sizeof(avgBuf),"Avg Turnaround Time:  %.2f", sumTAT/computed);
            FDraw(avgBuf, cols[5]-100, ay+22, 14, COL_GREEN);
        }

        EndDrawing();


        }
        UnloadFont(gFont);
        CloseWindow();
        return 0;
}

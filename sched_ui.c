//UI- common to all. Basically, I don't want to write and re-write the UI again for each algo + the interactivity bit
//this helps
//raylib bloat sucks

#include "sched.h"

Font gfont;
void FDraw(const char *text, int x, int y, int size Color col){
    DrawTextEx(gFont, text, (Vector2){x, y}, size, 1, col);
}

int FMeasure(const char *text, int size) {
    return (int)MeasureTextEx(gFont, text, size, 1).x;
}

//input screen
typedef struct {
    int  n;
    int  arrival[MAX_PROCESSES];
    int  burst[MAX_PROCESSES];
    int  activeField;   // field index: row i → arrival=i*2, burst=i*2+1; -1=none
    char buf[16];
    int  bufLen;
} InputState;

//initialising the initial inputs. user can modify and change these as they wish
static void InputState_init(InputState *s) {
    *s = (InputState){
        .n           = 3,
        .activeField = -1,
        .arrival     = {0, 2, 4},
        .burst       = {5, 3, 2},
    };
}

//input handler. basically increase/decrease number of processes + editing the content of arrival/ burst cells.
static int HandleInput(InputState *s, int SW, int SH) {
    //process count- key up = increase number of processes, key down = decrease number of processes
    if (IsKeyPressed(KEY_UP) && s->n < MAX_PROCESSES) {
        if (++s->n, s->activeField != -1 && s->activeField/2 >= s->n)
            s->activeField = -1;
    }
    if (IsKeyPressed(KEY_DOWN) && s->n > 1) {
        if (--s->n, s->activeField != -1 && s->activeField/2 >= s->n)
            s->activeField = -1;
    }
 
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp     = GetMousePosition();
        s->activeField = -1;
 
        for (int i = 0; i < s->n; i++) {
            Rectangle cells[2] = {
                {320, 180 + i*50, 140, 34},   //arrival
                {500, 180 + i*50, 140, 34},   //burst
            };
            for (int c = 0; c < 2; c++) {
                //check for "collision b/w mouse pointer and editable cell
                if (CheckCollisionPointRec(mp, cells[c])) {
                    s->activeField = i*2 + c;
                    int val = (c == 0) ? s->arrival[i] : s->burst[i];
                    snprintf(s->buf, sizeof(s->buf), "%d", val);
                    s->bufLen = strlen(s->buf);
                }
            }
        }
 
        Rectangle btnR = {SW/2 - 80, SH - 90, 160, 44};
        if (CheckCollisionPointRec(mp, btnR)) return 1;
    }
 //edit the um...editable cell ig
    if (s->activeField != -1) {
        for (int key = GetCharPressed(); key > 0; key = GetCharPressed()) {
            if (key >= '0' && key <= '9' && s->bufLen < 3) {
                s->buf[s->bufLen++] = (char)key;
                s->buf[s->bufLen]   = '\0';
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE) && s->bufLen > 0)
            s->buf[--s->bufLen] = '\0';
 
        int val = atoi(s->buf);
        if (s->activeField % 2 == 0) s->arrival[s->activeField/2] = val;
        else s->burst  [s->activeField/2] = val;
    }
    return 0;
}

//one editable cell be like- 
static void DrawCell(const InputState *s, int fieldIdx, int cx, int y) {
    bool active = (s->activeField == fieldIdx);
    int rawVal = (fieldIdx % 2 == 0) ? s->arrival[fieldIdx/2] : s->burst  [fieldIdx/2];
    Rectangle r= {cx, y, 140, 34};
    char val[16];
 
    DrawRectangleRec(r, active ? COL_ACTIVE : COL_SURFACE);
    DrawRectangleLinesEx(r, 1, active ? COL_ACCENT : COL_BORDER);
 
    if (active) snprintf(val, sizeof(val), "%s", s->buf);
    else        snprintf(val, sizeof(val), "%d", rawVal);
    FDraw(val, cx + 8, y + 8, 16, COL_TEXT);
 
    //blinky blinky
    if (active && ((int)(GetTime()*2) % 2 == 0))
        FDraw("|", cx + 8 + FMeasure(val, 16), y + 8, 16, COL_ACCENT);
}

//input screen
static void DrawInputScreen(const InputState *s, const char *label, int SW, int SH) {
    FDraw(label, SW/2 - FMeasure(label, 22)/2, 28, 22, COL_ACCENT);
 
    const char *hint = "UP/DOWN  add or remove processes  |  click a cell to edit";
    FDraw(hint, SW/2 - FMeasure(hint, 13)/2, 62, 13, COL_DIM);
 
    FDraw("Process",      100, 140, 15, COL_DIM);
    FDraw("Arrival Time", 320, 140, 15, COL_DIM);
    FDraw("Burst Time",   500, 140, 15, COL_DIM);
    DrawLine(80, 160, 680, 160, COL_BORDER);
 
    for (int i = 0; i < s->n; i++) {
        int y = 175 + i*50;
        char pid[8]; snprintf(pid, sizeof(pid), "P%d", i+1);
        FDraw(pid, 120, y + 7, 16, COL_TEXT);
        DrawCell(s, i*2+0, 320, y);
        DrawCell(s, i*2+1, 500, y);
    }
 
    Rectangle btnR = {SW/2 - 80, SH - 90, 160, 44};
    bool       hov  = CheckCollisionPointRec(GetMousePosition(), btnR);
    DrawRectangleRec(btnR, hov ? COL_BTN_HOV : COL_BTN);
    DrawRectangleLinesEx(btnR, 1, COL_BORDER);
    const char *lbl = "COMPUTE";
    FDraw(lbl, SW/2 - FMeasure(lbl, 18)/2, SH - 80, 18, COL_TEXT);
}

//mah  chart mah dearest gantt chart
static void DrawGanttChart(const Process *procs, int n, int gx, int gy, int gh, int gw) {
    int total = procs[n-1].finish;
    FDraw("Gantt Chart", gx, gy - 20, 14, COL_DIM);
 
#define GANTT_X(t) (gx + (int)((float)(t) / total * gw))
 
    for (int i = 0; i < n; i++) {
        //gap b4 the proc
        int gapStart = (i == 0) ? 0 : procs[i-1].finish;
        if (procs[i].start > gapStart) {
            int ix0 = GANTT_X(gapStart), ix1 = GANTT_X(procs[i].start);
            DrawRectangle(ix0, gy, ix1-ix0, gh, COL_IDLE);
            DrawRectangleLinesEx((Rectangle){ix0, gy, ix1-ix0, gh}, 1, COL_BORDER);
            FDraw("idle", ix0+4, gy+16, 13, COL_DIM);
            if (i == 0) FDraw("0", ix0, gy+gh+5, 12, COL_DIM);
        }
 
        int x0 = GANTT_X(procs[i].start), x1 = GANTT_X(procs[i].finish);
        DrawRectangle(x0, gy, x1-x0, gh, COL_RUNNING);
        DrawRectangleLinesEx((Rectangle){x0, gy, x1-x0, gh}, 1, COL_ACCENT);
 
        char lbl[8]; snprintf(lbl, sizeof(lbl), "P%d", procs[i].pid);
        int tw = FMeasure(lbl, 15);
        if (x1 - x0 > tw + 4)
            FDraw(lbl, x0 + (x1-x0)/2 - tw/2, gy + gh/2 - 9, 15, COL_TEXT);
 
        //tick tick and timestamp
        DrawLine(x1, gy, x1, gy+gh+4, COL_BORDER);
        char ts[8]; snprintf(ts, sizeof(ts), "%d", procs[i].finish);
        FDraw(ts, x1 - FMeasure(ts,12)/2, gy+gh+5, 12, COL_DIM);
 
        if (i == 0 && procs[0].start == 0) {
            FDraw("0", x0 - FMeasure("0",12)/2, gy+gh+5, 12, COL_DIM);
        }
    }
#undef GANTT_X 
    
//result table
static void DrawTable(const Process *procs, int n, int ty, int SW) {
    static const int  cx[] = {50, 150, 270, 390, 510, 640, 790};
    static const char *hd[]= {"PID","Arrival","Burst","Start","Finish","Waiting","Turnaround"};
 
    for (int c = 0; c < 7; c++) FDraw(hd[c], cx[c], ty, 14, COL_DIM);
    DrawLine(40, ty+20, SW-40, ty+20, COL_BORDER);
 
    float sumWT=0, sumTAT=0;
    char buf[32];
 
    for (int i = 0; i < n; i++) {
        int y = ty + 28 + i*34;
        if (i % 2 == 0) DrawRectangle(40, y-3, SW-80, 28, COL_SURFACE);

        snprintf(buf,32,"P%d",procs[i].pid);
        FDraw(buf,cx[0],y,15,COL_TEXT);

        snprintf(buf,32,"%d", procs[i].arrival);    
        FDraw(buf,cx[1],y,15,COL_TEXT);

        snprintf(buf,32,"%d", procs[i].burst);      
        FDraw(buf,cx[2],y,15,COL_TEXT);

        snprintf(buf,32,"%d", procs[i].start);      
        FDraw(buf,cx[3],y,15,COL_TEXT);

        snprintf(buf,32,"%d", procs[i].finish);     
        FDraw(buf,cx[4],y,15,COL_TEXT);

        snprintf(buf,32,"%d", procs[i].waiting);    
        FDraw(buf,cx[5],y,15,COL_YELLOW);

        snprintf(buf,32,"%d", procs[i].turnaround); 
        FDraw(buf,cx[6],y,15,COL_GREEN);
 
        sumWT  += procs[i].waiting;
        sumTAT += procs[i].turnaround;
    }
 
    int ay = ty + 28 + n*34 + 14;
    DrawLine(40, ay-6, SW-40, ay-6, COL_BORDER);
    snprintf(buf,32,"Avg Waiting Time:     %.2f", sumWT/n);
    FDraw(buf, cx[5]-100, ay,    14, COL_YELLOW);
    snprintf(buf,32,"Avg Turnaround Time:  %.2f", sumTAT/n);
    FDraw(buf, cx[5]-100, ay+22, 14, COL_GREEN);
}
 
static void DrawResultsScreen(const Process *procs, int n, const char *label, int SW, int SH) {
    (void)SH;
    FDraw(label, SW/2 - FMeasure(label,20)/2, 18, 20, COL_ACCENT);
    FDraw("ESC / BACKSPACE to go back", 20, 18, 13, COL_DIM);
 
    int gx=60, gy=62, gh=52, gw=SW-120;
    DrawGanttChart(procs, n, gx, gy, gh, gw);
    DrawTable(procs, n, gy + gh + 48, SW);
}
 
//main event loop. the RunScheduler function 
void RunScheduler(const char *windowTitle, const char *algoLabel, ComputeFn compute) {
    const int SW=1100, SH=700;
    InitWindow(SW, SH, windowTitle);
    gFont = LoadFontEx("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 64, 0, 0); //TODO: change this path later
    SetTextureFilter(gFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);
 
    InputState inp;
    InputState_init(&inp);
 
    Process procs[MAX_PROCESSES];
    int computed = 0;
    int state    = 0;   //states. o for input, 1 for result
 
    while (!WindowShouldClose()) {
        if (state == 0 && HandleInput(&inp, SW, SH)) {
            for (int i = 0; i < inp.n; i++) {
                procs[i] = (Process){
                    .pid= i+1,
                    .arrival= inp.arrival[i],
                    .burst= inp.burst[i] < 1 ? 1 : inp.burst[i],
                };
            }
            compute(procs, inp.n);
            computed= inp.n;
            state= 1;
        }
        if (state == 1 && (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)))
            state = 0;
 
        BeginDrawing();
        ClearBackground(COL_BG);
        if (state == 0) DrawInputScreen(&inp, algoLabel, SW, SH);
        else DrawResultsScreen(procs, computed, algoLabel, SW, SH);
        EndDrawing();
    }
 
    UnloadFont(gFont);
    CloseWindow();
}

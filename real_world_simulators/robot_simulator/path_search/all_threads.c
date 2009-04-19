#ifdef _NO_PRET_
#  include <stdio.h>
#  define END_SIMULATION return 0
#  define WAIT_FOR_END_SIMULATION return 0
#else
#  define putchar(x) WRITE(x)
#  include "deadline.h"
#endif

#define bool int
#define true 1
#define false 0

#define X_COORD ((volatile int*) 0x80000500)
#define Y_COORD ((volatile int*) 0x80000504)
#define HEADING ((volatile int*) 0x8000050C)

#define ENEMY_X ((volatile int*) 0x80000510)
#define ENEMY_Y ((volatile int*) 0x80000514)

#define WHEEL ((volatile int*) 0x80000400)
enum direction {LEFT = 1, RIGHT = 2};

#define MOTOR ((volatile int*) 0x80000404)
enum motor {STOP = 1, GO = 2};

#define NAV_COMMAND ((volatile int*) 0x3FFFF000)

#define BLOCK_LENGTH 1024
#define WIDTH 15
#define HEIGHT 11

char maze[HEIGHT][WIDTH] =
{
#include "grid.txt.h"
};

char dfsvisited[HEIGHT][WIDTH];
char bfsvisited[HEIGHT][WIDTH];
#if defined(THREAD_0)
char (*visited)[WIDTH] = bfsvisited;
#elif defined(THREAD_1)
char (*visited)[WIDTH] = dfsvisited;
#else
char (*visited)[WIDTH] = bfsvisited;
#endif

#ifdef _NO_PRET_
   int myx = 2;
   int myy = 2;
   int otherx = 12;
   int othery = 5;
#else
#  define myx (*X_COORD / BLOCK_LENGTH)
#  define myy (*Y_COORD / BLOCK_LENGTH)
#  define otherx (*ENEMY_X / BLOCK_LENGTH)
#  define othery (*ENEMY_Y / BLOCK_LENGTH)
#endif

#define MAXQUEUESIZE 80
int queue[MAXQUEUESIZE];
int qhead = 0;
int qtail = 0;

#define enqueue(y,x) queue[qhead] = (y); queue[qhead+1] = (x); \
                              qhead += 2; qhead %= MAXQUEUESIZE
#define dequeue(y,x) y = queue[qtail]; x = queue[qtail+1]; \
                              qtail += 2; qtail %= MAXQUEUESIZE
  
#define empty(y,x) (bfsvisited[y][x] == '0')
bool bfs(int y, int x) {
  enqueue(y, x);
  while (qtail != qhead) {
    dequeue(y, x);
    if (y == myy && x == myx) return true;

    if (empty(y+1,x)) {
      bfsvisited[y+1][x] = 'd';
      enqueue(y+1, x);
    }
    if (empty(y,x-1)) {
      bfsvisited[y][x-1] = 'r';
      enqueue(y, x-1);
    }
    if (empty(y-1,x)) {
      bfsvisited[y-1][x] = 'u';
      enqueue(y-1, x);
    }
    if (empty(y,x+1)) {
      bfsvisited[y][x+1] = 'l';
      enqueue(y, x+1);
    }
  }
  return false;

}

#undef empty
#define empty(y,x) (dfsvisited[y][x] == '0')
bool dfs(int y, int x) {
  // If we found the end, then we are finished
  if (y == myy && x == myx) return true;

  if (empty(y+1,x)) {
    dfsvisited[y+1][x] = 'd';
    if (dfs(y+1,x)) return true;
  }
  if (empty(y,x-1)) {
    dfsvisited[y][x-1] = 'r';
    if (dfs(y,x-1)) return true;
  }
  if (empty(y-1,x)) {
    dfsvisited[y-1][x] = 'u';
    if (dfs(y-1,x)) return true;
  }
  if (empty(y,x+1)) {
    dfsvisited[y][x+1] = 'l';
    if (dfs(y,x+1)) return true;
  }
  return false;
}

void printany(char any[HEIGHT][WIDTH]) {
  int i,j;
  for (i = HEIGHT-1; i >= 0; i--) {
    for (j = 0; j < WIDTH; j++) {
      putchar(any[i][j]);
    }
    putchar('\n');
  }
}

void printloc(int y, int x) {
  int i,j;
  for (i = HEIGHT-1; i >= 0; i--) {
    for (j = 0; j < WIDTH; j++) {
      if (x == j && y == i) putchar('m');
      else if (otherx == j && othery == i) putchar('b');
      else putchar(maze[i][j]);
    }
    putchar('\n');
  }
}

void printlocme() {
  printloc(myy, myx);
}

void printpath(int y, int x, char visited[HEIGHT][WIDTH]) {
  printloc(y, x);
  while (!(y == othery && x == otherx)) {
    switch (visited[y][x]) {
       case 'l':
         x -= 1;
         break;
       case 'r':
         x += 1;
         break;
       case 'd':
         y -= 1;
         break;
       case 'u':
         y += 1;
         break;
       case '^':
         return;
    }
    printloc(y, x);
  }
}

void prefire() {
  int i,j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      visited[i][j] = (maze[i][j] == 'w') ? 'w' : '0';
    }
  }
  qhead = 0;
  qtail = 0;
}

void logging_loop() {
  while (true) {
#   ifndef NO_MAP
    // Continuously print graph of current location
    printlocme();
#   endif
  }
}

void navigation_loop() {
  while (true) {
    char nav_command = *NAV_COMMAND;
    switch (nav_command) {
      case 'u': case 'd': case 'l': case 'r':
        while (nav_command != *HEADING) {
          *WHEEL = LEFT;
        }
        *NAV_COMMAND = 0;
        break;
      default:
        // Do nothing on invalid command
        break;
    }
  }
}

void turn() {
  putchar('0' + myy);
  putchar('0' + myx);
  putchar(visited[myy][myx]);
  putchar(*HEADING);
  putchar('\n');
  *NAV_COMMAND = visited[myy][myx];
}

int mainloop() {
  int goalx = otherx;
  int goaly = othery;
  bool (*search)(int,int);
  char name;
# if defined(THREAD_0)
  search = &bfs;
  name = 'B';
# elif defined(THREAD_1)
  search = &dfs;
  name = 'D';
# endif
  int i;
# define CYCLES_PER_METER 1000000
  for (i = 0; i < 10; ++i) {
    DEAD4(CYCLES_PER_METER/6);
    tryin (CYCLES_PER_METER/8) {
    prefire();
    visited[goaly][goalx] = '^';
      if (search(goaly, goalx)) {
        putchar(visited[myy][myx]);
        putchar(name);
        putchar('\n');
      }
    } catch {
#     if defined(THREAD_0)
      *MOTOR = STOP;
      putchar('F');
#     endif
    }
#   if defined(THREAD_0)
    turn();
    *MOTOR = GO;
#   endif
  }
  return 0;
}

int main() {
#if defined(THREAD_0) || defined(THREAD_1)
  mainloop();
  END_SIMULATION;
#elif defined(THREAD_4)
  navigation_loop();
#elif defined(THREAD_5)
  logging_loop();
#else
  WAIT_FOR_END_SIMULATION;
#endif
}

void sentinal() {}

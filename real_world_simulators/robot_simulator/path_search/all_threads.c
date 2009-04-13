#ifdef _NO_PRET_
#  include <stdio.h>
#  define END_SIMULATION return 0
#  define WAIT_FOR_END_SIMULATION return 0
#else
#  define putchar(x) WRITE(x)
#  include "deadline.h"
#endif


#define X_COORD ((volatile int*) 0x80000500)
#define Y_COORD ((volatile int*) 0x80000504)
#define HEADING ((volatile int*) 0x8000050C)

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
char bfsvisited[HEIGHT][WIDTH] =
{
"wwwwwwwwwwwwwww",
"w00uuuuuuulrruw",
"wuuuuuuuuullwuw",
"wuuuuuuuuulwulw",
"wuuuuuuuuuwuulw",
"wrrrrrrrrrrr^lw",
"wrdwrrrrrdwrdlw",
"wrrrrrrrrrrrdlw",
"wrrrrrrrrrrrdww",
"w0rdwrrrrrrrdlw",
"wwwwwwwwwwwwwww"
};

void printany(char any[HEIGHT][WIDTH]) {
  int i,j;
  for (i = HEIGHT-1; i >= 0; i--) {
    for (j = 0; j < WIDTH; j++) {
      putchar(any[i][j]);
    }
    putchar('\n');
  }
}

#define bool int
#define true 1
#define false 0

#ifdef _NO_PRET_
int myx = 2;
int myy = 2;
#else
#  define myx (*X_COORD / BLOCK_LENGTH)
#  define myy (*Y_COORD / BLOCK_LENGTH)
#endif
int otherx, othery;

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
      bfsvisited[i][j] = (maze[i][j] == 'w') ? 'w' : '0';
      dfsvisited[i][j] = (maze[i][j] == 'w') ? 'w' : '0';
    }
  }
}

// Forward declaration of turn required to pass arguments to DMAMV
void turn();
void navigation_loop() {
#ifndef _NO_PRET_
  void* addr;
  for (addr = (void*)&navigation_loop; addr < (void*)&turn; addr += 16) {
    DMAMV(addr, addr - (void*)&navigation_loop);
  }
#endif
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
  putchar(bfsvisited[myy][myx]);
  putchar(*HEADING);
  putchar('\n');
  /*
  while (bfsvisited[myy][myx] != *HEADING) {
    *WHEEL = LEFT;
  }
  */
  *NAV_COMMAND = bfsvisited[myy][myx];
}

int mainloop() {
  /*
  prefire();
  dfsvisited[othery][otherx] = '^';
  if (dfs(othery,otherx)) {
    putchar(dfsvisited[myy][myx]);
#   ifdef _NO_PRET_
    printany(dfsvisited);
    printpath(myy, myx, dfsvisited);
#   else
    putchar('D');
#   endif
  }
  bfsvisited[othery][otherx] = '^';
  if (bfs(othery,otherx)) {
    putchar(bfsvisited[myy][myx]);
#   ifdef _NO_PRET_
    printany(bfsvisited);
    printpath(myy, myx, bfsvisited);
#   else
    putchar('B');
#   endif
  }
  */
# ifndef _NO_PRET_
  *MOTOR = GO;
  while (myx != otherx || myy != othery) {
    turn();
  }
  *MOTOR = STOP;
# endif

  return 0;
}

int main() {
  otherx = 12, othery = 5;
#if defined(THREAD_0)
  printlocme();
  mainloop();
  END_SIMULATION;
#elif defined(THREAD_1)
  navigation_loop();
#endif
  WAIT_FOR_END_SIMULATION;
}

void sentinal() {}

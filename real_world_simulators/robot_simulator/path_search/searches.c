#include <stdio.h>

#define WIDTH 15
#define HEIGHT 11

char maze[HEIGHT][WIDTH] =
{
#include "grid.txt"
};

char dfsvisited[HEIGHT][WIDTH];
char bfsvisited[HEIGHT][WIDTH];

void printany(char any[HEIGHT][WIDTH]) {
  int i,j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      putchar(any[i][j]);
    }
    putchar('\n');
  }
}

#define bool int
#define true 1
#define false 0

int myx, myy;
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
      bfsvisited[y+1][x] = 'u';
      enqueue(y+1, x);
    }
    if (empty(y,x-1)) {
      bfsvisited[y][x-1] = 'r';
      enqueue(y, x-1);
    }
    if (empty(y-1,x)) {
      bfsvisited[y-1][x] = 'd';
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
    dfsvisited[y+1][x] = 'u';
    if (dfs(y+1,x)) return true;
  }
  if (empty(y,x-1)) {
    dfsvisited[y][x-1] = 'r';
    if (dfs(y,x-1)) return true;
  }
  if (empty(y-1,x)) {
    dfsvisited[y-1][x] = 'd';
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
  for (i = 0; i < HEIGHT; i++) {
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
       case 'u':
         y -= 1;
         break;
       case 'd':
         y += 1;
         break;
       case '^':
         return;
    }
    printloc(y, x);
  }
}

void initialize() {
  int i,j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      bfsvisited[i][j] = (maze[i][j] == 'w') ? 'w' : '0';
      dfsvisited[i][j] = (maze[i][j] == 'w') ? 'w' : '0';
    }
    putchar('\n');
  }
}

int main() {
  myx = 2; myy = 2;
  otherx = 12, othery = 5;
  initialize();
  printany(maze);
  dfsvisited[othery][otherx] = '^';
  if (dfs(othery,otherx)) {
    puts("DFS results:\n");
    printany(dfsvisited);
    printpath(myy, myx, dfsvisited);
  }
  bfsvisited[othery][otherx] = '^';
  if (bfs(othery,otherx)) {
    puts("BFS results:\n");
    printany(bfsvisited);
    printpath(myy, myx, bfsvisited);
  }
  return 0;
}

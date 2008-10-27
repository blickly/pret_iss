//#include <stdio.h>
//#include <string.h>

//int val=2;

int main()
{
  char str[40];
  printf("Hello World\n");
  srand48(20);
  printf("rand: %d\n", drand48());
  double db = 10.30;
  sprintf(str, "float before: %f\n", db);
  printf(str);
  printf("float before hex: %x\n", db);
  db = db + 1.1;
  printf("float: %f\n", db);
//  printf("Hello World %d\n", val);
  return 0;
}

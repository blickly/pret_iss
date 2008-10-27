#define ARRAY_SIZE 10

int main()
{
  int iarray[10]= {5,3,600,302,1,62,14,6,2,0};
  int x, y, holder;
 
  for(x = 0; x < ARRAY_SIZE; x++)
     for(y = 0; y < ARRAY_SIZE-1; y++)
     if(iarray[y] > iarray[y+1]) {
       holder = iarray[y+1];
       iarray[y+1] = iarray[y];
       iarray[y] = holder;
     }

  printf("iarray: {");
  for (x = 0; x < ARRAY_SIZE -1 ; x++) {
    printf("%d,",iarray[x]);    
  }
  printf("%d}\n",iarray[ARRAY_SIZE - 1]);
  
  //  asm(".byte 0xC1, 0x60, 0x00, 0x00");

  return 1;
}

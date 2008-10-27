void insertionSort(int numbers[], int array_size);

int main()
{
  
   int iarray[10]= {5,3,600,302,1,62,14,6,2,0};
   int x = 0;
   
   insertionSort ( iarray, 10);

   printf("iarray: {");
   for (x = 0; x < 10 -1 ; x++) {
     printf("%d,",iarray[x]);    
   }
   printf("%d}\n",iarray[10 - 1]);
   
   asm(".byte 0xC1, 0x60, 0x00, 0x00");

  return 1;


}

void insertionSort(int numbers[], int array_size)
{
  int i, j, index;

  for (i=1; i < array_size; i++)
  {
    index = numbers[i];
    j = i;
    while ((j > 0) && (numbers[j-1] > index))
    {
      numbers[j] = numbers[j-1];
      j = j - 1;
    }
    numbers[j] = index;
  }
}

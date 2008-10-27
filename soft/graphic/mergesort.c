#define MAX 10
 
void ShiftTo(int *ar, int iDest, int iSrc);
void CombineAr(int *ar, const int left, const int right, const int pivot);
void MergeS(int *ar, int left, int right, int pivot);
 
int main(void)
{
  int ar[MAX] = {11,3,600,302,1,62,14,6,2,0};
  int x = 0;
  MergeS(ar, 0, MAX - 1, MAX / 2);   // Call to merge sort the array

  
  printf("ar: {");
  for (x = 0; x < 10 -1 ; x++) {
    printf("%d,",ar[x]);    
  }
  printf("%d}\n",ar[10 - 1]);
  
   
  return 0;
}
void ShiftTo(int *ar, int iDest, int iSrc) //this method is highly innefective, and can't be used for big arrays
{
  int i = iSrc;
  int StoreSrc = ar[iSrc];     
  for(; i > iDest; i --) ar[i] = ar[i - 1];    // Shifts numbers from iDest to iSrc one step forward
  ar[iDest] = StoreSrc;                         // Puts final element in the right place
}
 
void CombineAr(int *ar, const int left, const int right, const int pivot)
{
  int i = left, j = pivot + 1;
  while (j != right + 1 && i != j)        // continue until either list runs out                                              
    {
      if(ar[j] <= ar[i])            // Move the jth element in front of the ith element
	ShiftTo(ar, i ++ , j ++);     // ShiftTo(array, destination index, source index)
      else i ++;                     // Skip to the next element
    }
}
 
void MergeS(int *ar, int left, int right, int pivot)
{
  if(left == right) return;
  else
    {
      MergeS(ar, left, pivot, (left + pivot) / 2);               // First half
      MergeS(ar, pivot + 1, right, (pivot + right + 1) / 2);   // Second half
    }
  CombineAr(ar, left, right, pivot);    // Combines two sorted arrays (left -> pivot) and (pivot+1 -> right)
}

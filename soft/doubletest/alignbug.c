
int main()
{
  double db = 10.30;
  double* i0 = &db;
//  int * i1 = (int*) i0 + 1;

  printf("%f\n", db);
 
  printf("addr1 = %x \t\t addr2 = %x\n", &db, i0);

  printf("%x\n", db);
  printf("%x\n", *i0);

//  printf("%x\n", *i1);

  return 0;
}

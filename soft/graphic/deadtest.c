int main()
{
  volatile int a = 3;
  
  printf("address of a is 0x%x\n", &a);
  
  while (1)
    {
      asm(".byte 0xC3, 0x60, 0x20, 0x10");
      a++;
    }
  return 0;
}

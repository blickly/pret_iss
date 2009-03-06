#include <stdio.h>

double f(double x) {
  return x*x*x + 3.0*x*x + x + 3.0;
}
double fprime(double x) {
  return 3.0*x*x + 6.0*x + 1.0;
}

void newton(int num_cycles) {
  double guess = 5.0;
  printf("Initial guess of %f running for %d cycles\n", guess, num_cycles);
  tryin (num_cycles) {
    for (;;) {
      guess = guess - f(guess)/fprime(guess);
      //printf("new guess %f\n", guess);
    }
  } catch {
    printf("Final value is %f\n", guess);
  }
}

int main() {
  int num_cycles = 1000000;
  printf("Starting Newton's Method\n");
//  for (num_cycles = 0; num_cycles < 5000000; num_cycles += 1000000) {
    newton(num_cycles);
//  }
  printf("done with main\n");
  exit(0);
}

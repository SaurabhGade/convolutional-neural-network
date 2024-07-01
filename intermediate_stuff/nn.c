#define NN_IMPLEMENTATION 
#include "nn.h"
#include<time.h>
int main(void){
  srand(time(NULL));
  Mat m = mat_alloc(3,3);
  Mat n = mat_alloc(3,3);
  mat_fill(m, 1);
  mat_fill(n,2);
  mat_sum(m, n);
  mat_print(m);
  return 0;
}

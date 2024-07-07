#define NN_IMPLEMENTATION 
#include "nn.h"
#include<time.h>
#include<math.h>
float td_xor[] = {
  0, 0, 0,
  0, 1, 1,
  1, 0, 1,
  1, 1, 0 
};
float td_xnor[] = {
  0, 0, 1,
  0, 1, 0,
  1, 0, 0,
  1, 1, 1 
};
float td_or[] = {
  0, 0, 0,
  0, 1, 1,
  1, 0, 1,
  1, 1, 1 
};
float td_nor[] = {
  0, 0, 1,
  0, 1, 0,
  1, 0, 0,
  1, 1, 0 
};

float td_and[] = {
  0, 0, 0,
  0, 1, 0,
  1, 0, 0,
  1, 1, 1 
};
float td_nand[] = {
  0, 0, 1,
  0, 1, 1,
  1, 0, 1,
  1, 1, 0 
};

size_t converge(float x){
  if(x > 0.5) return 1;
  else return 0;
}


int main(void){
  srand(time(NULL));

  size_t strid = 3;
 // size_t n = sizeof(td)/sizeof(td[0])/strid; //because we have to find out total number of rows in one dimentional array.
  float *td = td_xnor;  
  size_t n = 4; 
  Mat train_input = {
      .rows = n,
      .cols = 2,
      .strid = strid,
      .es = td
  }; 
  Mat train_output = {
      .rows = n,
      .cols = 1,
      .strid = strid,
      .es = td+2 
  };

  size_t arch[] = {2,4,1};
  NN nn = nn_alloc(arch , ARRAY_LEN(arch));  
  NN g = nn_alloc(arch , ARRAY_LEN(arch));  
  nn_rand(nn,0,1);

  float eps = 1e-1;
  float rate = 1e-1;
  for(size_t i = 0 ; i < 1000*10 ; i++){
    printf("%f\n",nn_cost(nn,train_input, train_output));
    nn_f_diff(nn, g, eps,train_input, train_output); 
    nn_learn(nn, g, rate);
  }
  
  NN_PRINT(nn);

  for(size_t i = 0 ; i < 2 ; i++){
    for(size_t j = 0 ; j < 2 ; j++){
      MAT_AT(NN_INPUT(nn), 0, 0) = i;
      MAT_AT(NN_INPUT(nn), 0, 1) = j;
      nn_forward(nn);
      printf("%zu op %zu = %zu\n",i,j,converge(MAT_AT(NN_OUTPUT(nn),0,0)));
    }
  }
  return 0;
}

/*
 in this project I'll try to implement universal logic gate 
 using matrices and linear algebra.

 (x^y) = (x|y) & nand(x,y);

 architecture consists two layers tow input x1, x2;
 these two input connected with two neurons (nand , or) they are not actually nand and or but for convinience I'll calling them .
 i.e. in first layer we have 4 waights.
 so we require 2x2 matrix of waights in first layer.
 two connection means 2 biases. so we also require vector of size 2 of biases.
  
 * */

#define NN_IMPLEMENTATION 
#include "nn.h"
#include<time.h>
#include<math.h>

typedef struct{
  Mat a0, a1, a2;
  //you can say input matrix is intermidate result and treat is as same way.
  //so here we neaming input as a0.


  //first layer. 
  Mat w1, b1;

  //second layer.
  Mat w2, b2;

} Xor;
Xor xor_alloc(void){

  Xor m; 
  m.a0 = mat_alloc(1,2);    //input vector.
  /* //first layer consist 4 waights, so allocate matrix of 4 rand values.
  //and row vector of size 2 for biases.
  //naming conventions : w -> waight matrix , b -> bias vector. , a -> intermidate. 
  //digit values after matrix name indicate layer number. */
  
  m.w1 = mat_alloc(2,2);    //waight matrix of first layer. 
  m.b1 = mat_alloc(1,2);    //bias vector of first layer.
  
  m.a1 = mat_alloc(1,2);    //intermidate matrix to store the result of first result. 
  //result of first layer will be fed in second layer.
  //result of input_mat * w1 + b will be the matrx of size 1x2 
  //so now to want column vector of size two i.e. 2x1 so matrix multiplication will define.
  // in second layer we only have one neuron so bias matrix will be size of 1x1
  
  m.w2 = mat_alloc(2,1);
  m.b2 = mat_alloc(1,1);
  m.a2 = mat_alloc(1,1);
   
   return m;
}

//this function will help to pass values in model.
//so it will take model and input vales.
void forward_xor(Xor m){
  
  mat_dot(m.a1, m.a0, m.w1);   //pass the input through first layer i.e. matrix multiplication of input and waights matrix. 
  mat_sum(m.a1, m.b1);       //add biases in result 
  mat_sig(m.a1);          //apply activation function on result. 
  // a1 contain first intermidkate result.
  /* 
     just pass first intermidate result in second layer.  
   */
  mat_dot(m.a2 , m.a1, m.w2);
  mat_sum(m.a2, m.b2);
  mat_sig(m.a2);    //got second intermidiate result.
}



float cost(Xor m , Mat train_input , Mat train_output){
  assert((train_input).rows == (train_output).rows);
  //because of m.a2 contain intermidate result which is also the last layer.
  //so number of train_output in each row must be equel to m.as2.cols 
  assert(train_output.cols == m.a2.cols);
  size_t n = train_input.rows;

  float result = 0.0f;
  for(size_t i = 0 ; i < n ; i++){
    //initilizing input matrix. 
    Mat x = mat_row(train_input , i);
    //initilizing output matrix. 
    Mat y = mat_row(train_output, i);

    mat_copy(m.a0, x);
    forward_xor(m);

    size_t q = train_output.cols;
    for(size_t j = 0 ; j < q ; j++){
      float d = MAT_AT(m.a2, 0 , j) - MAT_AT(y, 0, j);
      result += d*d;
    }
  }
  return result/n;
}

void finite_diff(Xor m, Xor g, float eps, Mat ti, Mat to){
  float saved;
  float c = cost(m , ti, to); 
  for(size_t i = 0 ; i < m.w1.rows ; i++){
    for(size_t j = 0 ; j < m.w1.cols ; j++){
      saved = MAT_AT(m.w1 , i, j);
      MAT_AT(m.w1, i, j) += eps;
      MAT_AT(g.w1, i, j) = (cost(m, ti, to) - c)/eps;
      MAT_AT(m.w1 , i, j) = saved;
    }
  } 
  for(size_t i = 0 ; i < m.b1.rows ; i++){
    for(size_t j = 0 ; j < m.b1.cols ; j++){
      saved = MAT_AT(m.b1 , i, j);
      MAT_AT(m.b1, i, j) += eps;
      MAT_AT(g.b1, i, j) = (cost(m, ti, to) - c)/eps;
      MAT_AT(m.b1 , i, j) = saved;
    }
  } 
  for(size_t i = 0 ; i < m.w2.rows ; i++){
    for(size_t j = 0 ; j < m.w2.cols ; j++){
      saved = MAT_AT(m.w2 , i, j);
      MAT_AT(m.w2, i, j) += eps;
      MAT_AT(g.w2, i, j) = (cost(m, ti, to) - c)/eps;
      MAT_AT(m.w2 , i, j) = saved;
    }
  } 
  for(size_t i = 0 ; i < m.b2.rows ; i++){
    for(size_t j = 0 ; j < m.b2.cols ; j++){
      saved = MAT_AT(m.b2 , i, j);
      MAT_AT(m.b2, i, j) += eps;
      MAT_AT(g.b2, i, j) = (cost(m, ti, to) - c)/eps;
      MAT_AT(m.b2 , i, j) = saved;
    }
  }
}


void xor_learn(Xor m, Xor g, float rate){

  for(size_t i = 0 ; i < m.w1.rows ; i++){
    for(size_t j = 0 ; j < m.w1.cols ; j++){
      MAT_AT(m.w1 , i, j) -= rate*MAT_AT(g.w1 , i, j);
    }
  }
  for(size_t i = 0 ; i < m.b1.rows ; i++){
    for(size_t j = 0 ; j < m.b1.cols ; j++){
      MAT_AT(m.b1 , i, j) -= rate*MAT_AT(g.b1 , i, j);
    }
  } 
  for(size_t i = 0 ; i < m.w2.rows ; i++){
    for(size_t j = 0 ; j < m.w2.cols ; j++){
      MAT_AT(m.w2 , i, j) -= rate*MAT_AT(g.w2 , i, j);
    }
  }
  for(size_t i = 0 ; i < m.b2.rows ; i++){
    for(size_t j = 0 ; j < m.b2.cols ; j++){
      MAT_AT(m.b2 , i, j) -= rate*MAT_AT(g.b2 , i, j);
    }
  }

} 


float td[] = {
  0, 0, 0,
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
  size_t n = sizeof(td)/sizeof(td[0])/strid; //because we have to find out total number of rows in one dimentional array.

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

  size_t arch[] = {2,2,1};
  NN nn;
  nn = nn_alloc(arch , ARRAY_LEN(arch));  
  nn_rand(nn,0,1);

  Mat input = mat_row(train_input, 1);
  MAT_PRINT(input);
  mat_copy(NN_INPUT(nn), input);
  nn_forward(nn);
  MAT_PRINT(NN_OUTPUT(nn));
 
 return 0;

  Xor m = xor_alloc();
  Xor g = xor_alloc();
  mat_rand(m.w1, 0, 1);
  mat_rand(m.b1, 0, 1);
  mat_rand(m.w2, 0, 1);
  mat_rand(m.b2, 0, 1);

  float eps = 1e-1;
  float rate = 1e-1;

  for(size_t i = 0 ; i < 1000*100 ; i++){ 
   // printf("%f\n",cost(m,train_input, train_output));
    finite_diff(m, g, eps, train_input, train_output);
    xor_learn(m, g, rate);
  }
  for(size_t i = 0 ; i < 2 ; i++){
    for(size_t j = 0 ; j < 2 ; j++){
      MAT_AT(m.a0, 0, 0) = i;
      MAT_AT(m.a0, 0, 1) = j;
      forward_xor(m);
      float y = *m.a2.es;
      printf("%zu ^ %zu = %zu\n",i,j,converge(y));
    } 
  }
 return 0;
}

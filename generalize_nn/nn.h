#ifndef NN_H_
#define NN_H_

#ifndef NN_STDDEF
#include <stddef.h>
#endif //NN_STDDEF
/*IN SOME INVIRNOMENT THERE WILL BE NO AVAILIBILITY SO WE CAN PROVIDE .
 **/ 
#ifndef NN_MALLOC     //only if NN_MALLOC IS NOT DEFINED.
#include <stdlib.h>
#define NN_MALLOC malloc
#endif  //NN_MALLOC 

#ifndef NN_ASSERT
#include<assert.h>
#define NN_ASSERT assert
#endif  //NN_ASSERT

#ifndef NN_PRINTF
#include<stdio.h>
#define NN_PRINTF printf
#endif  //NN_PRINTF
//
#ifndef NN_MATH
#include<math.h>
#endif  //NN_MATH


#define ARRAY_LEN(xs) sizeof((xs))/sizeof((xs)[0])

//this is header part ... all declerations will be here
typedef struct{
  size_t rows;
  size_t cols;
  size_t strid;
  float *es;
} Mat;

#define MAT_AT(m, i, j) (m).es[(i)*(m).strid + (j)]    //this micor will help to get element at i,jth index. 
/* // Put all this (m).es[(i)*(m).cols + (j)] in parenthesis because.
// and of the variable can be expression.
//  for ex when calling macro i write MAT_AT(m, i+1 , j);
//  then the micro will be replaced with m.es[i+1*m.cols+j]
//  and because of operator precidence answer will be wrong.
//  so putting them in the parenthesis will be handle such setuations.
// */

float rand_float(void);
float sigmoidf(float x);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, float low , float high);
Mat mat_alloc(size_t rows, size_t cols);
Mat mat_row(Mat m , size_t row);
void mat_copy(Mat dst, Mat src);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst , Mat a);
void mat_print(Mat m, const char *name, size_t padding);
void mat_sig(Mat m);

#define MAT_PRINT(m) mat_print(m, #m, 0)    //this very cool feature of c priprocessing.
  //                                  //Where we can litterly convert input token into string. :))  
  //  MAT_PRINT MACRO TAKES A TOKEN AND PROVIDE IT TO mat_print function and convert same token in string and pass as second arguement.







//Neural network related stuff.
typedef struct {
  size_t count;     //number of layers in neural network.
  Mat *ws;          //array of waight matrices.
  Mat *bs;          //array of bias matrices.
  Mat *as;          //intermidate matrices. this array also contain the initial input so 
  //// so amount of activations will count+1 because it also contain the initial input.
} NN;

#define NN_PRINT(nn) nn_print(nn, #nn);
#define NN_INPUT(nn) (nn).as[0]
#define NN_OUTPUT(nn) (nn).as[(nn).count]

NN nn_alloc(size_t *arch, size_t arch_count);
void nn_print(NN nn, const char *name);
void nn_rand(NN nn, float lower_bound , float upper_bound);
void nn_forward(NN nn);
void nn_f_diff(NN m, NN g, float eps , Mat in, Mat out);
float nn_cost(NN m, Mat in, Mat out); 
#endif //NN_H

#ifdef NN_IMPLEMENTATION
//defination of declerations will be here
//use     #define NN_IMPLEMENTATION     TO USE definations in .c FILE.

void mat_sig(Mat m){
  for(size_t i = 0 ; i < m.rows ; i++){
    for(size_t j = 0 ; j < m.cols ; j++){
      MAT_AT(m,i,j) = sigmoidf(MAT_AT(m,i,j));
    }
  }
}
Mat mat_alloc(size_t rows, size_t cols){
  Mat m;
  m.rows = rows;
  m.cols = cols;
  m.strid = cols;
 /* //now we have to allocate memory for es , I'm trying to make it generealised as possible 
  //to achieve that I'm dereferecing es and then calculating the it's size by the help of sizeof function, 
  //so in feture I change datatype fo es it will autometically adopt to it. */
  m.es = NN_MALLOC(sizeof(*m.es)*rows*cols);
  NN_ASSERT(m.es != NULL);   //if assertion is failed program should be aborted.
  return m; 
}

float rand_float(void){
  return (float)rand()/RAND_MAX;    //generates random number between [0,1]
}

float sigmoidf(float x){
  return 1.f/(1.f + expf(-x)); 
}

void mat_fill(Mat m, float x){
  for(size_t i = 0 ; i < m.rows ; i++){
    for(size_t j = 0 ; j < m.cols ; j++){
      MAT_AT(m,i,j) = x; 
    } 
  } 
}


void mat_rand(Mat m , float low , float high){   //to allocate matrix with some random stuff.
  for(size_t i = 0 ; i < m.rows ; i++){
    for(size_t j = 0 ; j < m.cols ; j++){
      MAT_AT(m,i,j) = rand_float() *(high-low)+low;   //to get number in given rang between high and low.
      ////only work when rand_float() = [0,1],  number are between 0-1.
    } 
  } 
}

Mat mat_row(Mat m , size_t row){   //return return the specific row from given matrix. 
  // besically returns the matrix which contain  
//  NN_ASSERT(m.rows <= row);
  return (Mat){
    .rows = 1,
    .cols = m.cols,
    .strid = m.strid,
    .es = &MAT_AT(m,row,0)
  };
}

void mat_copy(Mat dst, Mat src){
  NN_ASSERT(dst.rows == src.rows);
  NN_ASSERT(dst.cols == src.cols);
  for(size_t i = 0 ; i < src.rows ; i++){
    for(size_t j = 0 ; j < src.cols ; j++){
      MAT_AT(dst, i, j) = MAT_AT(src, i, j);
    }
  }
}


void mat_dot(Mat dst, Mat a, Mat b){
  /*
    MATRIX MULTIPLICATION IS NOT DEFINED IF A_mxn * B_pxq if n != p 
    if m == p THEN IT'S DEFINED, AND RESULT OF THE MULTIPLICATION WILL BE
    R_mxq
    i.e. dst.rows == m && dis.cols = q;
   */
  NN_ASSERT(a.cols == b.rows);
  NN_ASSERT(dst.rows == a.rows);
  NN_ASSERT(dst.cols == b.cols);
  size_t n = a.cols; 
  for(size_t i = 0 ; i < dst.rows ; i++){
    for(size_t j = 0 ; j < dst.cols ; j++){
      MAT_AT(dst,i,j) = 0;
      for(size_t k = 0 ; k < n ; k++){
        MAT_AT(dst,i,j) +=  MAT_AT(a,i,k) * MAT_AT(b,k,j);
      }       
    } 
  }
}

void mat_sum(Mat dst , Mat a){
  /*
    //YOU CAN ONLY ADD TWO MATRIX IF THERE SHAPE IS SAME.
    //i.e. THERE DIMENSIONS MUST BE SAME.
    //and it this condition met
    //THEN YOU JSUT HAVE TO ADD CURRSPONDING ELEMENTS. 
   */
  NN_ASSERT(dst.rows == a.rows);    //abort if dst.rows != a.rows
  NN_ASSERT(dst.cols == a.cols);
  for(size_t i = 0 ; i < dst.rows ; i++){
    for(size_t j = 0 ; j < dst.cols ; j++){
      MAT_AT(dst,i,j) += MAT_AT(a,i,j); 
    }
  }
}


void mat_print(Mat m, const char *name, size_t padding){
  NN_PRINTF("%*s%s = [\n",(int)padding, "", name);
  for(size_t i = 0 ; i < m.rows ; i++){
    NN_PRINTF("%*s",(int)padding, ""); 
    for(size_t j = 0 ; j < m.cols ; j++){
      NN_PRINTF("%f ", MAT_AT(m,i,j));   
      //because actuall data stored as array but we interrpreting it as matrix.
      //for this I've written a micros MAT_AT 
      /*
        00 01 02    0*3+0 = 0 , 0*3+1 = 1 , 0*3+2 = 2
        10 11 12    1*3+0 = 3 , 1*3+1 = 4 , 1*3+2 = 5
        20 21 22    2*3+0 = 6 , 2*3+1 = 7 , 2*3+2 = 8 
       */ 
    }
    NN_PRINTF("\n");
  }
  NN_PRINTF("%*s]\n", (int)padding, "");
}



//Neural network related stuff.
NN nn_alloc(size_t *arch, size_t arch_count) {

  NN_ASSERT(arch_count > 0);  //arch_count must be greater than 0.

  NN nn;
  nn.count = arch_count - 1;     //because arch count also include input layer.
  //we only want count of intermidate layers. 
  nn.ws = NN_MALLOC(sizeof(*nn.ws) * nn.count);   // nn.count matrices allcated dinamically. 
  NN_ASSERT(nn.ws != NULL);

  nn.bs = NN_MALLOC (sizeof(*nn.bs) * nn.count);  // nn.count matrices allcated dinamically  
  NN_ASSERT(nn.bs != NULL);
 
  nn.as = NN_MALLOC(sizeof(*nn.as) * nn.count + 1);  // nn.as contain intermidate result but it also contain input so arch_count. 
  NN_ASSERT(nn.as != NULL);
  
  nn.as[0] = mat_alloc(1, arch[0]);   //nn.as[0] it's input vector, 
  //i.e number of row = 1 and number are arch[0].

  for(size_t i = 1 ; i < arch_count ; i++){
    nn.ws[i-1] = mat_alloc(nn.as[i-1].cols, arch[i]);
    nn.bs[i-1] = mat_alloc(1, arch[i]);
    nn.as[i] = mat_alloc(1, arch[i]);
  }
  return nn;
}

void nn_print(NN nn, const char *name){
  printf("%s = [\n",name);
  char buff[256];
  for(size_t i = 0 ; i < nn.count ; i++){
    snprintf(buff , sizeof(buff) , "ws%zu",i);
    mat_print(nn.ws[i], buff , 5);
    snprintf(buff , sizeof(buff) , "bs%zu",i);
    mat_print(nn.bs[i], buff , 5);
  } 
  printf("]\n");
}

void nn_rand(NN nn, float lower_bound , float upper_bound){
  NN_ASSERT(lower_bound <= upper_bound);
  for(size_t i = 0 ; i < nn.count ; i++){
      mat_rand(nn.ws[i], lower_bound, upper_bound);
      mat_rand(nn.bs[i], lower_bound, upper_bound);
  } 
}

void nn_forward(NN nn){
  for(size_t i = 0 ; i < nn.count ; i++){
    mat_dot(nn.as[i+1], nn.as[i], nn.ws[i]);
    mat_sum(nn.as[i+1], nn.bs[i]);
    mat_sig(nn.as[i+1]);
  }
}
float nn_cost(NN m, Mat ti, Mat to){
  NN_ASSERT(ti.rows == to.rows);
  NN_ASSERT(to.cols == NN_OUTPUT(m).cols);

  float c = 0;
  for(size_t i = 0 ; i < n ; i++){
    Mat x = mat_row(ti, i);   //input. 
    Mat y = mat_row(to, i);   //output.
    
    mat_copy(NN_INPUT(nn), x);
    nn_forward(nn);
    NN_OUTPUT(nn);
  }
} 
void nn_f_diff(NN m, NN g, float eps , Mat in, Mat out){
  (void) nn;
  (void)g;
  (void)eps;
  (void)ti;
  (void)to;
}

#endif //NN_IMPLEMENTATION

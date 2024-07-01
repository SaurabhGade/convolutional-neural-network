#ifndef NN_H_
#define NN_H_

#include <stddef.h>
/*IN SOME INVIRNOMENT THERE WILL BE NO AVAILIBILITY SO WE CAN PROVIDE .
 **/ 
#ifndef NN_MALLOC     //only if NN_MALLOC IS NOT DEFINED.
#include <stdlib.h>
#define NN_MALLOC malloc
#endif  //NN_MALLOC 

#ifndef NN_ASSERT
#include<assert.h>
#define NN_ASSERT assert
#endif

#ifndef NN_PRINTF
#include<stdio.h>
#define NN_PRINTF printf
#endif

//this is header part ... all declerations will be here
typedef struct{
  size_t rows;
  size_t cols;
  float *es;
} Mat;

#define MAT_AT(m, i, j) (m).es[(i)*(m).cols + (j)]    //this micor will help to get element at i,jth index. 
/* // Put all this (m).es[(i)*(m).cols + (j)] in parenthesis because.
// and of the variable can be expression.
//  for ex when calling macro i write MAT_AT(m, i+1 , j);
//  then the micro will be replaced with m.es[i+1*m.cols+j]
//  and because of operator precidence answer will be wrong.
//  so putting them in the parenthesis will be handle such setuations.
// */

float rand_float(void);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, float low , float high);
Mat mat_alloc(size_t rows, size_t cols);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst , Mat a);
void mat_print();



#endif //NN_H

#ifdef NN_IMPLEMENTATION
//defination of declerations will be here
//use     #define NN_IMPLEMENTATION     TO USE definations in .c FILE.

Mat mat_alloc(size_t rows, size_t cols){
  Mat m;
  m.rows = rows;
  m.cols = cols;
 /* //now we have to allocate memory for es , I'm trying to make it generealised as possible 
  //to achieve that I'm dereferecing es and then calculating the it's size by the help of sizeof function, 
  //so in feture I change datatype fo es it will autometically adopt to it. */
  m.es = NN_MALLOC(sizeof(*m.es)*rows*cols);
  NN_ASSERT(m.es != NULL);   //if assertion is failed program should be aborted.
  return m; 
}

/*
  // 
 
 * */
float rand_float(void){
  return (float)rand()/RAND_MAX;    //generates random number between [0,1]
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
      ////only work when number are between 0-1.
    } 
  } 
}


void mat_dot(Mat dst, Mat a, Mat b){
  (void) dst;
  (void) a;
  (void) b;
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


void mat_print(Mat m){
  for(size_t i = 0 ; i < m.rows ; i++){
    for(size_t j = 0 ; j < m.cols ; j++){
      NN_PRINTF("%f ",MAT_AT(m,i,j));   
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
}


#endif //NN_IMPLEMENTATION

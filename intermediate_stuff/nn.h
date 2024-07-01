#ifndef NN_H_
#define NN_H_

#include <stddef.h>


/*IN SOME INVIRNOMENT THERE WILL BE NO AVAILIBILITY SO WE CAN PROVIDE .
 **/ 
#ifndef NN_MALLOC     //only if NN_MALLOC IS NOT DEFINED.
#include <stdlib.h>
#define NN_MALLOC malloc
#endif  //NN_MALLOC 

//this is header part ... all declerations will be here
typedef struct{
  size_t rows;
  size_t cols;
  float *es;
} Mat;

Mat mat_alloc(size_t rows, size_t cols);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst , Mat a);
void mat_print();



#endif //NN_H

#ifdef NN_IMPLEMENTATION
//defination of declerations will be here
//use     #define NN_IMPLEMENTATION     TO USE IMIPLIMENTATION IN .c FILE.

Mat mat_alloc(size_t rows, size_t cols){
  Mat m;
  m.rows = rows;
  m.cols = cols;
 /* //now we have to allocate memory for es , I'm trying to make it generealised as possible 
  //to achieve that I'm dereferecing es and then calculating the it's size by the help of sizeof function, 
  //so in feture I change datatype fo es it will autometically adopt to it. */
  m.es = NN_MALLOC(sizeof(*m.es)*rows*cols);
  assert(m.es != NULL);   //if assertion is failed program should be aborted.
  return m; 
}

void mat_dot(Mat dst, Mat a, Mat b){
  (void) dst;
  (void) a;
  (void) b;
}
void mat_sum(Mat dst , Mat a){
  (void) dst;
  (void) a;
}
void mat_print();



#endif //NN_IMPLEMENTATION

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//suppose these is our dataset and we want to prident ouput any given x
float train[][2] = {
  {0,0},
  {1,2},
  {2,4},
  {3,6},
  {4,8},
  {5,10}
};
#define train_count (sizeof(train)/sizeof(train[0]))
float rand_float(void){
  //as we know rand return random number between 0 to RAND_MAX , so if we devide 
  //random number by RAND_MAX it will git us randome floting point between 0 to 1.
//  srand(time(0));
  return (float)rand()/(float)RAND_MAX; 
}

//if cost > 0 i.e there is error , so cost should be near zero.
float cost(float w, float b){      //cost function will return average squred error between expected any our result.
  float result = 0.0f; 
  for(size_t i = 0 ; i < train_count ; i++){
    float x = train[i][0];
    float y = x*w+b;
    float d = y - train[i][1];    //actual difference between expected result and model resutl
    result += d*d;      //taking squre because it will remove negative values and amplify error.
  }
  result /= (float)train_count;   //taking average
  return result;
}
//y = x*w; this is our model and train is dataset.
int main(){
//  srand(time(0)); 
  srand(time(0));
  float w = rand_float()*10.0f;   //randome floating point between 0-10.
  float b = rand_float()*5.0f;    //bias , I currently don't know why they used bias here, but it improves the accuricy of the neural network. 
  //now our goal is to minimize the cost
  //our goal is to actually drive the coast function to zero. we want to minimize it, 
  //the closer it is to zero, the better model behave. on that specific training data.

    /* 
   * TO MINIMIZE THE COST WHAT CAN WE DO?
   * AS WE KNOW A DERIVATIVE OF A FUNCTION TELLS US IN WHICH DIRECTION FUNCTION GROWS.
   * SO IF YOU MOVE OPPOSITE OF THAT DIRECTION, YOU EVENTUALLY END UP TO A MINIMA.
   * we can directly code the formula of derivative 
   * i.e (f(x+h)-f(x))/h. :))
   * */
  float eps = 1e-3;   //it is very small value 10^-3.
  float rate = 1e-3;  //although we're using derivatives but to ensure that jump are notk to big we use rate which is also a very small values.
  for(size_t i = 0 ; i < 9999 ; i++){
    float c = cost(w,b); 
    float dw = (cost(w + eps , b) - c )/eps;
    float db = (cost(w, b + eps) - c )/eps; 
    w -= rate*dw;
    b -= rate*db;
    printf("cost = %f , w  = %f, b = %f\n",cost(w , b) , w , b);
  }
  printf("------------------------------------------\n");
  printf("w = %f , b = %f\n",w , b);
  return 0;
}

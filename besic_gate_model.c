#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
typedef float sample[3];  //to make syntax lillbit easier.

//// some function life sin , cos , sqrt ,... work with double
/// there float version also declearted as sinf , cosf , sqrtf ,...
float sigmoidf(float x){    //in c if function end with f that means we are working on flaot.
  return 1.f/(1.f+expf(-x));  //this will besically map every value between -inf to +inf -> 0 to 1
//1/(1+e^-x)
}
// OR-gate
// AND-gate
// NAND-gate
// work with any of these gates. but not xor gate.
sample or_train[] = {
  {0, 0, 0},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 1}
};
sample and_train[] = {
  {0, 0, 0},
  {0, 1, 0},
  {1, 0, 0},
  {1, 1, 1}
};
sample nand_train[] = {
  {0, 0, 1},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 0}
};

sample *train = nand_train;
size_t train_count = 4; 
/* Now we have to input , and one output
 * so we have to change cost function accordingly. 
 * */

  float cost(float w1, float w2 , float b){
  float result = 0.0f;
  for(size_t i = 0 ; i < train_count ; i++){
    float x1 = train[i][0];
    float x2 = train[i][1];
    float y = sigmoidf(x1*w1 + x2*w2 + b);    //using sigmoidf to map value between 0 to 1
    float d = y - train[i][2];
    result += d*d;
  }
  result /= train_count;
  return result;
}
float rand_float(void){
  return (float)rand()/RAND_MAX; 
}
int main(void){
/*  
  for(float x = -100.f; x <= 100.f ; x+= 1.f)
    printf("%f => %f\n",x,sigmoidf(x));

  return 0;
*/
  srand(time(0));
  float w1 = rand_float();
  float w2 = rand_float();
  float b = rand_float();

  float eps = 1e1;//e-3;
  float rate = 1e1;//-3;

  for(size_t i = 0 ; i < 1000*1000 ; ++i){
  float c = cost(w1 , w2, b);
  printf("w1 = %f , w2 = %f, b = %f, c = %f\n",w1 , w2 , b, c); 
  //printf("%f\n",c);
  float dw1 = (cost(w1+eps, w2, b) - c)/eps;
  float dw2 = (cost(w1, w2 + eps, b) - c)/eps;
  float db = (cost(w1, w2, b + eps) - c)/eps;
  w1 -= rate*dw1;
  w2 -= rate*dw2;
  b -= rate*db;
  }
 printf("w1 = %f , w2 = %f, b = %f, c = %f\n", w1, w2, b, cost(w1,w2,b));
  
  for(size_t i = 0 ; i < 2 ; i++){
   for(size_t j = 0 ; j < 2 ; j++){
      printf("%zu | %zu = %f\n",i,j,sigmoidf(i*w1 + j*w1+b));
    }
  }
  return 0;
}
//to represent non-linear function like xor, you need more complex neural network architectures such as multi-layer preceptrons (mlps)
//or other type of networks like convolutional neural networkds (CNN) or recurrent neural networkd(RNN) these arch. are capable of learning non-leaner
//relationships between inputs, making 
//but xor get can be rep in terums of and , or.
//a^b = (a|b) & ~(a&b)
int main22(){
  //x^y = (x|y) & ~(x&y);
  for(size_t x = 0 ;x  < 2 ; x++){
    for(size_t y = 0 ; y < 2 ; y++){
      printf("%zu ^ %zu = %zu\n",x,y,(x|y) & (~(x&y)));
    }
  }
  return 0;
}

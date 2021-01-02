#include "common/hash.h"
#include<iostream>
int main(){
   size_t seed= 20;
   int v=4;
   marian::util::hash_combine(seed,v);
   std::cout<<"将20hash之后的值为:"<<seed<<std::endl;
   return 0;

}
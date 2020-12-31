
#include<iostream>
#include "device.h"
using namespace marian;

void deviceTest(){
    Device device;
    std::cout<<device.size()<<std::endl;
    device.reserve(128);
    std::cout<<"分配了128的内存之后的size为:"<<device.size()<<std::endl;

}
int main(){
    deviceTest();
    return 0;
}
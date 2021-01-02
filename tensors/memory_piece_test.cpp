
#include<iostream>
#include "allocator.h"
#include "device.h"
using namespace marian;

// void deviceTest(){
//     Device device;
//     std::cout<<device.size()<<std::endl;
//     device.reserve(128);
//     std::cout<<"分配了128的内存之后的size为:"<<device.size()<<std::endl;

// }

void allocatorTest(){
    size_t bytes=300;
    size_t step=128;
    Ptr<Device> device = std::make_shared<Device>();
    std::cout<<"这里也是正常的"<<std::endl;
    Allocator allocator(device,bytes,step);
    std::cout<<"难道是我这里初始化出问题了吗？"<<std::endl;
    std::cout<<"分配的内存总共大小为:"<<allocator.size()<<std::endl;
    std::cout<<"内存可用空间为: "<<allocator.available()<<std::endl;
    auto memory = allocator.memory();
    std::cout<<*memory<<std::endl;


}
int main(){
    //deviceTest();
    allocatorTest();
    return 0;
}
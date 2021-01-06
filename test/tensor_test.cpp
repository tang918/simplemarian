#include "tensors/tensor_allocator.h"
#include "tensors/device.h"
#include "common/definitions.h"
#include "tensors/tensor.h"
#include <iostream>
#include "tensors/rand.h"
#include "functional/tensor.h"
using namespace marian;

void tensor_alloctor_test(){
    Ptr<Device> device = New<Device>();
    //std::cout<<device->size()<<std::endl;

    TensorAllocator tensorallocator(device);
    auto allocator = tensorallocator.allocator();

    std::cout<<allocator->size()<<std::endl;
    std::cout<<allocator->gapSize()<<std::endl;
    std::cout<<allocator->alloctedSize()<<std::endl;
    //tensorallocator.reserve(128);
    //tensorallocator.reserveExact(128);
   // std::cout<<device->size()<<std::endl;
    //std::cout<<tensorallocator.size()<<std::endl;
    Shape shape({3,4});
    std::cout<<"shape 的形状为:";
    std::cout<<shape.elements()<<std::endl;

    Tensor tensor = nullptr;
    std::cout<<"开始为tensor分配内存了...."<<std::endl;
    tensorallocator.allocate(tensor,shape);
    std::cout<<"完成内存分配后的各个参数为:"<<std::endl;
    std::cout<<"tensor的形状为:"<<tensor->shape()<<std::endl;
    std::cout<<"device申请的内存大小为"<<device->size()<<std::endl;
    std::cout<<"allocator的Gap大小为："<<allocator->gapSize()<<"allocator申请的总内存为:"<<allocator->size()<<std::endl;
    std::cout<<"alloactor的allocted_大小为: "<<allocator->alloctedSize()<<std::endl;
    std::cout<<"tensor的内存大小为："<<tensor->memory()->size()<<std::endl;

}
void view_test(){
     Ptr<Device> device = New<Device>();
     TensorAllocator tensorallocator(device);
     Shape shape({3,2,4});
     std::cout<<"shape的形状为: "<<shape.elements()<<std::endl;

     Tensor tensor = nullptr;
     tensorallocator.allocate(tensor,shape);
     std::cout<<"tensor的形状为:"<<tensor->shape()<<std::endl;
     size_t seed=2;
     StdlibRandomGenerator generator(seed);
     generator.uniform(tensor,0.f,1.f);
     tensor->printtensor();

     //定义视图对象
     marian::functional::View<float,3> view(tensor);
     std::cout<<"尝试一下tensor[i]是个什么玩意儿"<<std::endl;
     std::cout<<view[1]<<std::endl;



}

int main(){
    view_test();
    //tensor_alloctor_test();
    return 0;
}
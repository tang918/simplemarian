#pragma once
#include<deque>
#include<set>
#include<iostream>
#include "common/definitions.h"
#include "allocator.h"
#include "tensor.h"
#include "common/types.h"

namespace marian{
class TensorAllocator{
private:
    const size_t CHUNK = 128;
    const size_t MBYTE = 1024*1024;
    const size_t GROW = CHUNK * MBYTE;
    const size_t ALING = 256;
    Ptr<Device> device_;
    Ptr<Allocator> allocator_;

public:
    TensorAllocator(Ptr<Device> device):device_(device),allocator_(New<Allocator>(device,0,GROW,ALING)){}
    ~TensorAllocator(){clear();}
    //以MB为单位进行内存申请
    void reserve(size_t bytes=0){
        auto mult = bytes / GROW + 1;
        std::cout<<"memory extending reserved space to"<<mult * CHUNK<<" MB"<<std::endl;
        allocator_->reserve(mult * GROW);
    }

    void reserveExact(const std::vector<size_t>& bytes){
        size_t total=0;
        for(auto part:bytes){
            total += allocator_->alignedSize(part);
        }
        reserveExact(total);
    }
    //以bytes为单位进行内存分配
    void reserveExact(size_t bytes=0){
        size_t mbytes = bytes / MBYTE;
        if(mbytes==0){
            std::cout<<"memory resrving"<<bytes<<" B"<<std::endl;
        }
        else{
            std::cout<<"[memory] Reserving "<<mbytes<<" MB, "<<std::endl;
        }
        allocator_->reserve(bytes);
    }

    size_t capacity(Shape shape){
        return allocator_->capacity<char>(requiredBytes(shape));
    }

    void allocate(Tensor& t,Shape shape){
        if(!t || t->shape()!=shape){
            auto mem = allocator_->alloc(requiredBytes(shape));
            t = Tensor(TensorBase::New(mem,shape,device_));
        }
    }
    void free(const Tensor& t){allocator_->free(t->memory());}

    void clear(){allocator_->clear();}

    Tensor asTensor(){
        auto mem = allocator_->memory();
        auto size = mem->size() / sizeof(float);
        return TensorBase::New(mem,Shape({1,(int)size}),device_);
    }

    size_t size() {return allocator_->size() /sizeof(float);}
    Ptr<Allocator> allocator(){return allocator_;}
};
}//namespace marian
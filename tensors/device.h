#pragma once

#include <cmath>
#include <cstdint>
#include<iostream>
#include "common/definitions.h"

namespace marian{

/*
*在指定设备上申请内存，调用posix_memalign函数，由于此处简化了设备处理，因此没有设备信息，
**/
class Device{
public:
    Device(size_t alignment=256):alignment_(alignment){}
    uint8_t* data() { return data_; }
    size_t size() { return size_; }
    ~Device(){
        genericFree(data_);
    };
    void reserve(size_t size){
        size=align(size);
        uint8_t *temp = static_cast<uint8_t*>(genericMalloc(alignment_, size));
        if(data_) {
            std::copy(data_, data_ + size_, temp);
            genericFree(data_);
        }
        data_ = temp;
        size_ = size;
    }
private:
    uint8_t* data_{0};
    size_t size_{0};
    size_t alignment_;
    size_t align(size_t size){
        return (size_t)(ceil(size / (float)alignment_) * alignment_);
    }
    void *genericMalloc(size_t alignment,size_t size){
        void *result;
        int ret = posix_memalign(&result,alignment,size);
        if(ret){
            std::cout<<"内存分配失败！"<<std::endl;
            return nullptr;
        }
        return result;
    }
    void genericFree(void* ptr){
        free(ptr);
    }
};



}//namespace marian
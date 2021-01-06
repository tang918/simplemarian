#pragma once

#include "common/definitions.h"
#include "common/shape.h"
#include "device.h"
#include "memory_piece.h"
#include "common/except.h"
#include<algorithm>
#include<iomanip>
#include<iostream>
#include<memory>
#include<sstream>
namespace marian{
class TensorBase{
    MemoryPiece::PtrType memory_;
    Shape shape_;
    Ptr<Device> device_;
    ENABLE_INTRUSIVE_PTR(TensorBase)
    TensorBase(MemoryPiece::PtrType memory,Shape shape,Ptr<Device> device)
        : memory_(memory),shape_(shape),device_(device){}
public:
    typedef IPtr<TensorBase> PtrType;
    template <class ...Args>
    static PtrType New(Args&& ...args){
        return PtrType(new TensorBase(std::forward<Args>(args)...));
    }
    virtual ~TensorBase(){}
    virtual void reset(MemoryPiece::PtrType memory){memory_ = memory;}
    virtual MemoryPiece::PtrType memory() { return memory_; }
    virtual Shape& shape(){return shape_;}
    virtual float* data(){return memory_->data<float>();}
    template <typename T>
    T* data(){
        return memory_->data<T>();
    }
    virtual size_t size(){return shape_.elements();}
    template<typename T>
    T scalar(){
        if(size()!=1) MARIAN_RUNTIME_ERR("Tensor is not a scalar");
        return get<T>(0);
    }

    // Tensor subtensor(size_t offset, size_t size) {
    //     auto mem = MemoryPiece::New(memory_->data() + sizeof(float) * offset, sizeof(float) * size);
    //     return TensorBase::New(mem, Shape{1, (int)size}, device_);
    // }

    template <typename T>
    T get(size_t i){
         T temp = 0;
         std::copy(data<T>()+i,data<T>()+i+1,&temp);
         return temp;
    }

    float get(size_t i){
        return get<float>(i);
    }

    template<typename T>
    void get(std::vector<T>& v){
        v.resize(size());
        std::copy(data<T>(),data<T>()+size(),v.data());
    }

    template<typename T>
    void set(size_t i,T value){
        std::copy(&value,&value+1,data<T>()+i);
    }

    template<typename T>
    void set(const T* begin,const T* end){
        std::copy(begin,end,data<T>());
    }

    template <typename T>
    void set(const std::vector<T>&v){
        set(v.data(),v.data()+v.size());
    }

    template<typename T>
    void set(T value){
        std::fill(data<T>(),data<T>()+size(),value);
    }

    void setSparse(const std::vector<size_t>&k,const std::vector<float>&v){
        for(size_t i=0;i<k.size();++i){
            data()[k[i]]=v[i];
        }
    }

    template <typename T>
    void copyFrom(Tensor in){
        if(in->shape()!=shape_) MARIAN_RUNTIME_ERR("shape不匹配！");
        std::copy(in->data<T>,in->data<T>() + in->size(),data<T>());
    }

    template <typename T>
    void swap(Tensor swapee){
        if(swapee->shape()!=shape_) MARIAN_RUNTIME_ERR("要交换的两个tensor的类型不匹配!");
            std::swap_ranges(swapee->data<T>(),swapee->data<T>()+swapee->size(),data<T>());
    }

    template<typename Tas,typename Tval>
    void setAs(Tval value){set((Tas)value);}


    
    void printtensor(){
        std::cout<<"经过赋值后的tensor各项值为: "<<std::endl;
        auto p = data();
        while(p<data()+size()){
            std::cout<<*p<<",";
            p++;
        }
        std::cout<<std::endl;

    }
};
}//namespace marian
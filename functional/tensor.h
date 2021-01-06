#pragma once

#include "functional/array.h"
#include "functional/shape.h"
#include "tensors/tensor.h"
#include <iostream>
namespace marian
{
namespace functional
{
template <typename T, const int D>
struct View{
    T* data_;
    ConstantShape<D> shape_;
    View(){}
    View(T* ptr,const ConstantShape<D>& shape):data_(ptr),shape_(shape){}
    View(marian::Tensor t):data_(t->data<T>()),shape_(t->shape()){
    }
    T& operator[](size_t i){
        return data_[shape_.index((int)i)];
    }

    const T& operator[](size_t i) const{
        return data_[shape_.index(i)];
    }
    T& operator[](const Array<int,D>& indices){
        return data_[shape_.index(indices)];
    }

    const T& operator[](const Array<int,D>& indices) const{
        return data_[shape_.index(indices)];
    }
    T* data(){return data_;}
    ConstantShape<D>& shape(){return shape_;}
    const ConstantShape<D>& shape() const {return shape_;}
    size_t size() const {return shape_.elements();}
};   

template <typename T>
using Tensor = View<T,CONST_SHAPE_DIMS>;

} // namespace functional
} // namespace marian

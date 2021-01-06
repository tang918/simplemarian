#pragma once
#include "tensors/tensor.h"
namespace marian
{

template <size_t I=0>
struct E{
    template<size_t numArg,class Functor,typename ElementType>
    static inline void element(const Functor& functor,functional::Array<F::Tensor<ElementType>,numArg>& tensors,
        F::Array<int,numArg> indices){

        const auto& shape = tensors[0].shape();
        for(int i=0;i<shape[I];++i){
            E<I+1>::element(functor,tensors,indices);

            for(size_t k = 0;k<numArg;++k){
                indices[k] += tensors[k].shape().bstride(I);
            }
        }

        }
};

template<>
struct E<functional::Shape::size()>{
    template <size_t numArg,class Functor,typename ElementType>
    static inline void element(const Functor& functor,functional::Array<functional::Tensor<ElementType>, numArg>& tensors,
        const functional::Array<int,numArg>& indices){
        
        tensors[0].data()[indices[0]] = functional::apply(functor,tensors,indices);
    }
};

template <typename ElementType,class Functor,class... Tensors>
void element(const Functor& functor,marian::Tensor out,Tensors... tensors){
    constexpr size_t argNum = sizeof...(tensors) +1;
    functional::Array<int,argNum> indices;
    indices.fill(0);
    functional::Array<functional::Tensor<ElementType>,argNum> gTensors = {out,tensors...};
    E<0>::element(functor,gTensors,indices);
}

template<>

template <class Functor,class... Tensors>
void Element(const Functor& functor,marian::Tensor out,Tensors... Tensors){
    elementFloat(functor,out,tensors...);
}

} // namespace marian

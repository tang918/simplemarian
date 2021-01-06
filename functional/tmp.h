#pragma once
#include "functional/array.h"
#include "functional/tensor.h"

namespace marian
{
namespace functional
{

template <size_t K, class Functor,typename AccType>
struct FApply {};

template <class Functor,typename AccType>
struct FApply<1,Functor,AccType>{
    template <typename ElementType>
    static AccType apply(Functor functor,
        functional::Array<functional::Tensor<ElementType>,1>& in,
        const functional::Array<int,1>& indices){
        
        return functor((AccType)in[0].data()[indices[0]]);
    }
    static AccType apply(Functor functor,functional::Array<functional::Tensor<ElementType>,1>& in,
        int index){
        
        return functor((AccType)in[0].data()[index]);
    }
};

template <class Functor,typename AccType>
struct FApply<2,Functor,AccType>{
    template<typename ElementType>
    static AccType apply(
        Functor functor,
        functional::Array<functional::Tensor<ElementType>,2>& in,
        const functional::Array<int,2>& indices){
        
       return functor((AccType)in[0].data()[indices[0]],(AccType)in[1].data()[indices[1]]);
    }

    template <typename ElementType>
    static AccType apply(
         Functor functor,
         functional::Array<functional::Tensor<ElementType>,2>& in,
         int index){
        return functor((AccType)in[0].data()[index],(AccType)in[1].data()[index]);
    }
};

template <class Functor,typename AccType>
struct FApply<3,Functor,AccType>{
    template <typename ElementType>
    static AccType apply(
        Functor functor,
        functional::Array<functional::Tensor<ElementType>, 3>& in,
        const functional::Array<int, 3>& indices){
      
      return functor((AccType)in[0].data()[indices[0]],
                   (AccType)in[1].data()[indices[1]],
                   (AccType)in[2].data()[indices[2]]);
    }

    template<typename ElementType>
    static AccType apply(
         Functor functor,
         functional::Array<functional::Tensor<ElementType>, 3>& in,
         int index) {
        return functor((AccType)in[0].data()[index],
                   (AccType)in[1].data()[index],
                   (AccType)in[2].data()[index]);
    }
};
template <class Functor, typename AccType>
struct FApply<4, Functor, AccType> {
  template <typename ElementType>
  static AccType apply(
      Functor functor,
      functional::Array<functional::Tensor<ElementType>, 4>& in,
      const functional::Array<int, 4>& indices) {
     return functor((AccType)in[0].data()[indices[0]],
                   (AccType)in[1].data()[indices[1]],
                   (AccType)in[2].data()[indices[2]],
                   (AccType)in[3].data()[indices[3]]);
   }

  template <typename ElementType>
  static AccType apply(
      Functor functor,
      functional::Array<functional::Tensor<ElementType>, 4>& in,
      int index) {
    return functor((AccType)in[0].data()[index],
                   (AccType)in[1].data()[index],
                   (AccType)in[2].data()[index],
                   (AccType)in[3].data()[index]);
   }
};
template <class Functor, typename AccType>
struct FApply<5, Functor, AccType> {
  template <typename ElementType>
  static AccType apply(
      Functor functor,
      functional::Array<functional::Tensor<ElementType>, 5>& in,
      const functional::Array<int, 5>& indices) {
    return functor((AccType)in[0].data()[indices[0]],
                   (AccType)in[1].data()[indices[1]],
                   (AccType)in[2].data()[indices[2]],
                   (AccType)in[3].data()[indices[3]],
                   (AccType)in[4].data()[indices[4]]);
  }

  template <typename ElementType>
  static AccType apply(
      Functor functor,
      functional::Array<functional::Tensor<ElementType>, 5>& in,
      int index) {
    return functor((AccType)in[0].data()[index], 
                   (AccType)in[1].data()[index], 
                   (AccType)in[2].data()[index], 
                   (AccType)in[3].data()[index], 
                   (AccType)in[4].data()[index]);
  }
};

template <typename ElementType,size_t K,class Functor>
ElementType apply(Functor functor,
                  functional::Array<functional::Tensor<ElementType>,K>&in,
                  const functional::Array<int,K>& indices){
    return FApply<K,Functor,ElementType>::apply(functor,in,indices);
 }

template <typename ElementType, size_t K, class Functor>
ElementType apply(Functor functor,
                    functional::Array<functional::Tensor<ElementType>, K>& in,
                    int index) {
  return FApply<K, Functor, ElementType>::apply(functor, in, index); // functor is applied to same type as input ElementType, no casting required
}

template <typename AccType, typename ElementType, size_t K, class Functor>
AccType applyWithCast(Functor functor,
                    functional::Array<functional::Tensor<ElementType>, K>& in,
                    const functional::Array<int, K>& indices) {
  return FApply<K, Functor, AccType>::apply(functor, in, indices); // ElementType and AccType are potentially different, cast to AccType before applying functor.
                                                                   // This is useful when accumulating e.g. 16-bit into 32-bit and we want to case to 32-bit before
                                                                   // the functor is applied. L2-Norm is a good use-case since the square can be large. 
}
template <typename AccType, typename ElementType, size_t K, class Functor>
AccType applyWithCast(Functor functor,
                    functional::Array<functional::Tensor<ElementType>, K>& in,
                    int index) {
  return FApply<K, Functor, AccType>::apply(functor, in, index); // ElementType and AccType are potentially different, cast to AccType before applying functor
}

} // namespace functional
} // namespace marian

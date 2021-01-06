#pragma once
#include "common/intrusive_ptr.h"
#include<functional>
#include<iostream>
#include<memory>
#include<string>
#include<vector>

namespace marian{
    
typedef uint32_t IndexType;

template <class T>
using IPtr = IntrusivePtr<T>;

template <class T>
using UPtr = std::unique_ptr<T>;

template <class T>
using IWeak = T*;

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
using Weak = std::weak_ptr<T>;

template <class T, typename... Args>
Ptr<T> New(Args&&... args) {
  return Ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
Ptr<T> New(Ptr<T> p) {
  return Ptr<T>(p);
}

template <class T, typename... Args>
IPtr<T> INew(Args&&... args) {
  return IPtr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
IPtr<T> INew(Ptr<T> p) {
  return IPtr<T>(p);
}

class TensorBase;
typedef IPtr<TensorBase> Tensor;



}//namespace marian
#pragma once

namespace marian{
namespace functional{

template <typename T,size_t N>
struct Array{
    typedef T value_type;
    T data_[N];

    inline const T* data() const {return data_;}

    inline T* data() {return data_;}

    inline constexpr static size_t size() {return N;}

    inline T& operator[](size_t i){return data_[i];}
    inline const T& operator[](size_t i) const {return data_[i];}
    inline T* begin(){return data_;}
    inline const T* begin() const {return data_;}

    inline T* end(){return data_ + N;}
    inline const T* end() const {return data_ + N;}

    inline void fill(T val){
        for(int i=0;i<N;++i) data_[i] = val;
    }

    inline T& back() {return data_[N-1];}
    inline const T& back() const {return data_[N-1];}
    inline bool operator==(const Array<T,N>& other){
        for(int i=0;i<N;++i){
            if(data_[i] != other[i]) return false;
        }
        return true;
    }
};
}//namespace functional
}//namespace marian
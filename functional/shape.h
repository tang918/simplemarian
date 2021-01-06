#pragma once
#include<cstdint>
#include<string>
#include<iostream>
#include "common/shape.h"
#include "array.h"
#include "common/except.h"

namespace marian{

namespace functional{

#define CONST_SHAPE_DIMS 4

template<const int N>
struct ConstantShape{
    Array<int,N> shape_;
    Array<int,N> stride_;
    Array<int,N> bstride_;

    size_t elements_{1};
    size_t offset_{0};

    ConstantShape(){
        shape_.fill(1);
        stride_.fill(1);
        bstride_.fill(0);
    }

    ConstantShape(const ConstantShape& shape):
        shape_(shape.shape_),stride_(shape.stride_),bstride_(shape.bstride_),elements_(shape.elements_),offset_(shape.offset_){}

    template<size_t M>
    ConstantShape(const Array<int,M>& shape){
        if(M>N) MARIAN_RUNTIME_ERR("M>N");
        std::copy(shape.begin(),shape.end(),shape_.begin()+N-M);
        if(N-M) std::fill_n(shape_.begin(),N-M,1);
        updateStrides();
        updateElements();
    }

    ConstantShape(const Array<int,N>& shape,const Array<int,N>& stride,size_t offset)
    :shape_(shape),stride_(stride),offset_(offset){
        updateElements();
    }

    ConstantShape(const marian::Shape& shape){
        size_t filled = shape.size();
        if(filled>N) MARIAN_RUNTIME_ERR("shape 大小大于N");
        std::copy(shape.begin(),shape.end(),shape_.begin()+N-filled);
        if(N-filled) std::fill_n(shape_.begin(),N-filled,1);
        updateStrides();
        updateElements();
    }
    void updateStrides(){
        stride_[N-1]=1;
        bstride_[N-1] = shape_[N-1]==1?0:stride_[N-1];
        for(int i=N-2;i>=0;--i){
            stride_[i] = stride_[i+1]*shape_[i+1];
            bstride_[i] = shape_[i] == 1 ? 0:stride_[i];
        }
    }

    void updateElements(){
        elements_=1;
        for(int i=0;i<N;++i){
            elements_ *= shape_[i];
        }
    }

    void set(int i,int dim){
        shape_[i] = dim;
        updateStrides();
        updateElements();
    }

    const int& dim(int i) const {return shape_[i];}
    const int& back() const {return dim(N-1);}
    const int& operator[](int i) const {return dim(i);}
    const int& stride(int i) const {return stride_[i];}
    const int& bstride(int i) const {return bstride_[i];}
    static constexpr size_t size() {return N;}
    int elements() const {return (int)elements_;}

    template<const int K,const int D> struct I{
        static int index(const Array<int,D>& dims,const Array<int,D>& stride){
            return dims[K]* stride[K] + I<K-1,D>::index(dims,stride);
        }
        static int index(int si,const Array<int,D>& shape,const Array<int,D>& stride){
            return (si % shape[K])* stride[K] + I<K-1,D>::index(si / shape[K],shape,stride);
        }

        static void dims(int si,Array<int,D>& dims,const Array<int,D>& shape){
            dims[K] = si % shape[K];
            I<K-1,D>::dims(si/shape[K],dims,shape);
        }
    };

    template<const int D> struct I<0,D>{
        static int index(const Array<int,D>& dims,const Array<int,D>& stride){
            return dims[0]*stride[0];
        }
        static int index(int si,const Array<int,D>& shape,const Array<int,D>& stride){
            return (si%shape[0])*stride[0];
        }
        static void dims(int si,Array<int,D>& dims,const Array<int,D>& shape){
            dims[0] = si % shape[0];
        }
    };

    int index(const Array<int,N>&dims) const{
        return (int)offset_ + I<N-1,N>::index(dims,stride_);
    }
    int index(int si) const{
        return (int)offset_ + I<N-1,N>::index(si,shape_,stride_);
    }
    void dims(int si,Array<int,N>& dims) const{
        I<N-1,N>::dims(si,dims,shape_);
    }

    int bindex(const Array<int,N>& dims)const{
        int i=0;
        for(int j=0;j<N;++j){
            i += dims[j]* bstride_[j];
        return i;
        }
    }

    bool operator==(const ConstantShape& other) const{
        for(int i=0;i<N;++i){
            if(shape_[i]!=other[i]) return false;
        }
        return true;
    }

    bool operator !=(const ConstantShape& other)const{
        return !(*this==other);
    }

    std::string toString() const{
        std::stringstream strm;
        strm<<"shape="<<(*this)[0];
        for(int i=1;i<size();++i){
            strm<<"x"<<(*this)[i];
        }
        strm <<" size="<<elements();
        return strm.str();
    }

    friend std::ostream& operator<<(std::ostream& strm,const ConstantShape<N>& shape){
        strm<<shape.toString();
        return strm;
    }
};

typedef ConstantShape<CONST_SHAPE_DIMS> Shape;

}//namespace functional
}//namespace marian
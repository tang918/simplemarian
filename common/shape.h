#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include<climits>
#include "common/hash.h"

namespace marian{
struct Slice
{
    Slice(int b,int e,int s):begin(b),end(e),stride(s){}
    Slice(int b,int e):Slice(b,e,1){}
    Slice():Slice(0,END){}
    explicit Slice(int i):Slice(i,i+1){}
    Slice(const Slice& other):Slice(other.begin,other.end,other.stride){}
    const Slice& operator=(const Slice& other){begin = other.begin;end=other.end;stride=other.stride;return *this;}
    const Slice& operator=(int i) { begin = i; end = i + 1; stride = 1; return *this; }
    bool operator==(const Slice& other)const {return begin==other.begin && end==other.end && stride==other.stride;}
    bool operator !=(const Slice& other) const {return !(*this == other);}
    int begin,end,stride;
    static const int END = INT_MAX;
};

typedef std::vector<Slice> Slices;

struct Shape{
private:
    std::vector<int> shape_;
public:
    Shape():shape_({1}){}
    Shape(std::initializer_list<int>il):Shape(){
        shape_.resize(il.size());
        std::copy(il.begin(),il.end(),begin());
    }
    Shape(std::vector<int>&& shape):shape_(std::move(shape)){}
    Shape(const Shape& shape):Shape(){
        shape_.resize(shape.size());
        std::copy(shape.begin(),shape.end(),begin());
    }
    Shape& operator=(const Shape& p)=default;
    
    void resize(size_t n){shape_.resize(n,1);}//新空间的值为1

    const int* data() const {return shape_.data();}
    int* data() {return shape_.data();}

    inline int& dim(int i){
        if(i>=0){
            if(i>=(int)size()){
                std::cout<<"数组越界！"<<std::endl;
                exit(1);
            }
            return shape_[i];
        }else{
            if((int)size()+i<0){
                std::cout<<"数组越界"<<std::endl;
                exit(1);
            }
            return shape_[size()+i];
        }
    }

    inline const int& dim(int i) const{
        return const_cast<Shape&>(*this).dim(i);
    }

    inline int& dim(size_t i) { return dim(int(i));}
    inline const int& dim(size_t i)const {return dim(int(i));}

    inline int operator[](int i) const {return dim(i);}
    inline int operator[](int i) {return dim(i);}
    inline int operator[](size_t i) const {return dim(i);}
    inline int operator[](size_t i) {return dim(i);}

    inline int back() const {return shape_.back();}
    inline int& back() {return shape_.back();}



    inline void set(int i,int val){dim(i)=val;}
    inline void set(size_t i,int val) {dim(i) = val;}
    inline void set(int i, size_t val) { dim(i) = (int)val;}
    inline void set(size_t i,size_t val) { dim(i) =  (int)val;}

    inline int stride(int i) const{
        std::vector<int> stride(shape_.size(),1);
        for(int j=(int)shape_.size()-2;j>=0;--j)
            stride[j] = stride[j+1]*shape_[j+1];
        if(i>=0) return stride[i];
        else return stride[size()+i];
    }

    template<typename T=int>
    inline T elements() const {
        T el=1;
        for(auto s:shape_)
            el *= (T)s;
        return el;
    }

    inline void dims(int i,std::vector<int>& d) const{
        d.resize(shape_.size());
        std::vector<int> stride(shape_.size(),1);
        for(int j=(int)shape_.size()-2;j>=0;--j)
            stride[j] = stride[j+1] * shape_[j+1];
        for(size_t j=0;j<d.size();++j)
            d[j] = (i / stride[j]) % shape_[j];
    }
    inline size_t size() const {return shape_.size();}
    auto begin()->decltype(shape_.begin()){return shape_.begin();}
    auto begin() const ->decltype(shape_.begin()){return shape_.begin();}
    auto end()->decltype(shape_.end()){return shape_.end();}
    auto end() const ->decltype(shape_.end()){return shape_.end();}
    auto rbegin()->decltype(shape_.rbegin()){return shape_.rbegin();}
    auto rbegin() const -> decltype(shape_.rbegin()) { return shape_.rbegin(); }

    auto rend() -> decltype(shape_.rend()) { return shape_.rend(); }
    auto rend() const -> decltype(shape_.rend()) { return shape_.rend(); }

    bool operator ==(const Shape& other)const {
        return size()==other.size() && std::equal(begin(),end(),other.begin());
    }

    bool operator!=(const Shape& other) const {return !(*this == other);}

    std::string toString() const {
        std::stringstream strm;
        strm <<"shape="<<(*this)[0];
        for(int i=1;i<size();++i)
            strm <<"x "<<(*this)[i];
        strm <<"size="<<elements();
        return strm.str();
    }

    friend std::ostream& operator<<(std::ostream& os,const Shape& shape){
        os<<shape.toString();
        return os;
    }

    operator std::string() const{
        std::stringstream ss;
        ss<<*this;
        return ss.str();
    }

    int axis(int ax) const{
        if(ax < 0)
            return (int)size()+ax;
        else 
            return ax;
    }

    Slice slice(Slice slice,int ax) const{
        int n = dim(ax);
        if(slice.begin < 0)
            slice.begin += n;
        if(slice.end <0)
            slice.end += n;
        else if(slice.end == Slice::END)
            slice.end=n;
        return slice;
    }

    static Shape broadcast(const std::vector<Shape>& shapes){
        size_t maxDims = 0;
        for(auto& s: shapes)
            if(s.size()> maxDims)
                maxDims = s.size();
        
        Shape shape;
        shape.resize(maxDims);
        for(auto& s: shapes){
            //要求每个维度大小一致或者某一个维度值为1才能进行广播
            for(int i=1;i<=(int)s.size();++i){
                if(shape[-i] !=s[-i] && shape[-i]!=1 && s[-i] !=1){
                    std::cout<<"Shapes" <<shape<<"and"<<s<<"cannot be broadcast"<<std::endl;
                    exit(1);
                }
                shape.set(-i,std::max(shape[-i],s[-i]));
            }
        }
        return shape;
    }

    template <typename T>
    static Shape broadcast(const std::initializer_list<T>& il){
        return broadcast(std::vector<T>(il));
    }

    template <typename T>
    static Shape broadcast(const std::vector<T>& nodes){
        size_t maxDims = 0;
        for(auto& n : nodes)
        if(n->shape().size() > maxDims)
            maxDims = n->shape().size();

        Shape shape;
        shape.resize(maxDims);

        for(auto& node:nodes){
            const Shape& shapen = node->shape();
            for(int i=1;i<=(int)shapen.size();++i){
                if(shape[-i] != shapen[-i] && shape[-i] != 1 && shapen[-i] != 1){
                    std::cout<<"广播的维度不匹配！"<<std::endl;
                    exit(1);
                }
                shape.set(-i,std::max(shape[-i],shapen[-i]));
            }
        }
        return shape;
    }

    size_t hash() const{
        size_t seed = util::hash<int>()(shape_[0]);
        for(size_t i=1;i<shape_.size();++i)
            util::hash_combine(seed,shape_[i]);
        return seed;
    }
};

}//namespace marian
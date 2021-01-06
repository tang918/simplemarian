#pragma once

#include "common/definitions.h"
#include<random>

namespace marian{
class TensorBase;
typedef IPtr<TensorBase> Tensor;
class RandomGenerator{
protected:
    size_t seed_;
public:
    RandomGenerator(size_t seed):seed_(seed){}
    virtual ~RandomGenerator(){}
    virtual void uniform(Tensor,float a,float b)=0;
    virtual void normal(Tensor,float mean,float stddev)=0;
};

class StdlibRandomGenerator: public RandomGenerator{
private:
    std::mt19937 engine_;
public:
    StdlibRandomGenerator(size_t seed): RandomGenerator(seed),engine_((unsigned int)seed){}
    virtual void uniform(Tensor tensor,float a,float b) override;
    virtual void normal(Tensor, float mean, float stddev) override;
};
void StdlibRandomGenerator::uniform(Tensor tensor,float a,float b){
    auto dist = std::uniform_real_distribution<float>(a,b);
    auto gen = bind(dist,std::ref(engine_));
    auto begin = tensor->data<float>();
    auto end = tensor->data<float>()+tensor->size();
    std::generate(begin, end, gen);
}
void StdlibRandomGenerator::normal(Tensor tensor,float mean,float stddev){
    auto dist = std::normal_distribution<float>(mean, stddev);
    auto gen = bind(dist, std::ref(engine_)); // does not change engine state without std::ref

    auto begin = tensor->data<float>();
    auto end   = tensor->data<float>() + tensor->size();
    std::generate(begin, end, gen);
}
}//namesapce marian
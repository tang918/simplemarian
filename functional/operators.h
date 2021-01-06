#pragma once
#include <cmath>
#include "common/except.h"
namespace marian
{
namespace functional
{
template <typename T>
struct Ops{
    static T tanh(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T sin(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T cos(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T tan(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T log(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T exp(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T abs(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T sqrt(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T neg(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T sgn(const T&){MARIAN_RUNTIME_ERR("Unknown type");}


    static T add(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T sub(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T mul(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T div(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}

    static T max(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T min(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T pow(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}

    static T negate(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T eq(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T neq(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T gt(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T lt(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T geq(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T leq(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T _and(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T _or(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}

    static T sigmoid(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T logaddexp(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T clip(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}

    static T bump(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T relu(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T reluBack(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T prelu(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T preluBack(const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}

    static T if_then_else(const T&,const T&,const T&){MARIAN_RUNTIME_ERR("Unknown type");}

    static T sumReduce(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T maxReduce(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
    static T minReduce(const T&){MARIAN_RUNTIME_ERR("Unknown type");}
};

template<>
struct Ops<float>
{
    typedef float Single;

    static float tanh(const float& x){return tanhf(x);}
    static float sin(const float& x){return sinf(x);}
    static float cos(const float& x){return cosf(x);}
    static float tan(const float& x){return tanf(x);}
    static float log(const float& x){return logf(x);}
    static float exp(const float& x){return expf(x);}
    static float abs(const float& x){return fabs(x);}
    static float sqrt(const float& x){return sqrtf(x);}
    static float neg(const float& x){return -x;}
    static float sgn(const float& x){return (float)((0<x)-(x<0));}
    static float add(const float& x,const float& y) {return x + y;}
    static float sub(const float&x,const float& y){return x-y;}
    static float mul(const float& x,const float& y){return x*y;}
    static float div(const float& x,const float& y){return x / y;}

    static float max(const float& x,const float& y){return x < y ? y : x;}
    static float min(const float& x,const float& y){return x < y ? x:y;}
    static float pow(const float& x,const float& y){return powf(x,y);}

    static float negate(const float& x){return !(bool)x;}
    static float eq(const float& x,const float& y){return x==y;}
    static float neq(const float& x,const float& y){return x != y;}
    static float gt(const float& x,const float& y){return x > y;}
    static float lt(const float& x,const float& y){return x < y;}
    static float geq(const float& x,const float& y){return x >= y;}
    static float leq(const float& x,const float& y){return x<=y;}
    static float and_(const float&x,const float&y){return x && y;}
    static float or_(const float& x,const float& y){return x || y;}

    static float sigmoid(const float& x){
        return x > 0? (1.f/(1.f+exp(-x))):(exp(x)/(1.f+exp(x)));
    }
    static float logaddexp(const float& x,const float& y){
        return x < y ?(y + log1pf(exp(x-y))) : (x+log1pf(exp(y-x)));
    }
    static float clip(const float& x,const float& y){return abs(x) >= y ? sgn(x)*y : x;}

    static float bump(const float& x,const float& y){return abs(x)>=y? 0.f:1.f;}
    static float relu(const float& x){return x>0.f?x:0.f;}
    static float reluBack(const float& x){return x>0.f?1.f : 0.f;}
    static float prelu(const float& x,const float& y){return x>0.f?x:x*y;}
    static float preluBack(const float& x,const float& y){ return x > 0.f ? 1.f : y;}
    static float if_then_else(const float& x,const float& y,const float& z){return x ? y : z;}//x不为0返回y,否则返回z

    static float sumReduce(const float& x){return x;}
    static float maxReduce(const float& x){ return x;}
    static float minReduce(const float& x){ return x;}

};

} // namespace functional

    
} // namespace marian

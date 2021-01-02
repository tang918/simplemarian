#include<iostream>
#include "common/shape.h"

using namespace std;

void test(){
    marian::Shape shape({1,2,3,4,5});
    cout<<"shape 的大小为:"<<shape.size()<<endl;
    vector<marian::Shape> vec;
    marian::Shape shape1({1,2,3,4,5});
    marian::Shape shape2({1,2,3,1,1});
    vec.push_back(shape1);
    vec.push_back(shape2);
    auto shape_ = marian::Shape::broadcast(vec);
    cout<<shape_<<endl;
    cout<<"helloworld"<<endl;

    cout<<"测试一下hash"<<shape.hash()<<std::endl;
}

int main(){
    test();
    return 0;
}
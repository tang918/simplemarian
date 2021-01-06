#include<algorithm>
#include<iostream>
#include<vector>
using namespace std;

int main(){
    vector<int> vec({3,5,1,9,7,6,2,0});
    sort(vec.begin(),vec.end(),[](int a,int b){return a<b;});
    for(auto v:vec){
        cout<<v<<",";
        
    }
    return 0;
}
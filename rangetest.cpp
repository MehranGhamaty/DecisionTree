//https://greek0.net/boost-range/boost-adaptors-unique.html

#include <vector>
#include <iostream>
#include <boost/range/adaptors.hpp>

using namespace std;

int main(){
    vector<int> v ={1,1,1,1,1};
    int i = 0;
    boost::range::for_each(v | boost::adaptors::unique, [&](auto v){++i;});

    cout << (boost::adaptors::unique(v)).size() << endl;
}



#include<vector>
#include<iostream>
#include<unordered_map>

#include <boost/range/numeric.hpp>
#include <boost/range/adaptors.hpp>

using namespace std;
using boost::accumulate;

int main(){
     vector<vector<int> > test = {{1},{},{}};
     bool isempty = accumulate(test, true, [](bool p, auto v){return p and v.empty();});
     cout << isempty << endl;

     vector<int> modetest = {1,2,3,4,5,1,2,3,5,1,1,1,2};
     unordered_map<int,int> m;
     for(const auto &v:modetest){ if(m.find(v) == m.end()) { m[v] = 1; } else { m[v]++; }}
     //for(const auto& v : m ) {cout << "Key:[" << v.first << "] Value:[" << v.second << "]\n";}
     auto maxpair = accumulate(m, pair<int,int>(-1,-1), [](auto p, auto n) -> pair<const int, int>{ if(n.second > p.second) return n; else return p;});

     cout << "[" << maxpair.first << "][" << maxpair.second << "]" << endl;

     vector<int> sametest = {1,1,1,1,2};
     auto same = accumulate(sametest, pair<bool, int>(true,sametest.at(0)), [](auto p, auto e){return pair<bool,int>(p.first and (e == p.second),p.second);}).first;
     cout << "vector is same " << same << endl;
}
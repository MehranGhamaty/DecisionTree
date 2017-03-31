#include<unordered_map>
#include<vector>
#include<iostream>

using namespace std;

int main() {
     vector<int> modetest = {1,2,3,4,5,1,2,3,5,1,1,1,2};
     unordered_map<int,int> m;

     for(const auto &v:modetest){
        if(m.find(v) == m.end()) {
            m[v] = 1;
        } else {
            m[v]++;
        }
     }

     for( const auto& v : m ) {
        cout << "Key:[" << v.first << "] Value:[" << v.second << "]\n";
    }

}
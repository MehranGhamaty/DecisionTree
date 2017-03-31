#include <iostream>
#include <vector>
#include <utility> //for std::pair
#include <iterator>
#include <iostream>
#include "dtree.cpp"


using namespace std;

auto loadfile() {

}

int main(int argc, char* argv[]) {
  vector<vector<float> > Xtrain = 
  {
    {1, 1, 2, 3, 3, 3, 1, 2, 2, 2}, //attribute 0
    {1, 2, 3, 4, 1, 1, 4, 2, 3, 3}, //attribute 1 split at 2.5 means true classes are: 0,0,1,0,2,
    {2, 2, 1, 0, 2, 1, 1, 2, 2, 1}, //attribute 2
  //{0, 0, 1, 1, 1, 0, 1, 2, 2, 2};
  };
  vector<int> ytrain = {0, 0, 1, 1, 1, 0, 1, 2, 2, 2};

  dtree::TreeNode<float, vector, vector, vector, int, vector> tree(Xtrain, ytrain);

  int prediction = tree.predictexample({2,1,1});

  cout << prediction << endl;

  cout << tree;

  vector<vector<float> > Xprune = 
  {
    {0, 3, 2, 9, 2, 1, 4, 0, 2, 1},
    {0, 2, 1, 3, 3, 1, 0, 4, 1, 3},
    {3, 4, 3, 3, 0, 0, 1, 1, 2, 1},
  };
  vector<int> yprune = {0, 1, 1, 2, 0, 0, 0, 1, 1, 1};


  tree.prunetree(Xprune, yprune);

  cout << "Pruned Tree:" << endl << tree;


}



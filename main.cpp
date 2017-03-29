#include <iostream>
#include <vector>
#include <utility> //for std::pair
#include <iterator>
#include <iostream>
#include "dtree.cpp"

/*
Need to figure out how to use 
iterator<teplate<typename>>


 */

int main(int argc, char* argv[])
{
  std::vector<std::vector<float> > Xtrain = 
  {
    {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
    {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
    {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
  };
  std::vector<int> Ytrain = {0, 0, 1, 1, 1, 0, 1, 2, 2, 2};

  dtree::TreeNode<float, int> tree(Xtrain, Ytrain);

  int prediction = tree.predictexample({2,1,1});

  std::cout << prediction << std::endl;
  /*
  for(const auto& row : Xtrain)
  {
    for(const auto& val : row)
      std::cout << val << "\t";
    std::cout << std::endl;
  }
  */

}



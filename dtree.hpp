
#ifndef DTREE_H
#define DTREE_H

#include <vector>

namespace dtree
{
  template <typename Label>
  int numdifferentelements(const std::vector<int> &);

  template <typename Label>
  float calcscore(const std::vector<Label> &,const int &);

  template <typename Label>
  std::vector<Label> countclasses(const std::vector<Label> &,const int &);

  template <typename T, typename O> 
  inline bool compare(T, O);

  template <typename Number>
  std::vector<Number> findsplits(const std::vector<Number>&);
  template <typename Number>
  std::vector<std::vector<Number> > findallsplits(const std::vector<std::vector<Number> >&);

  template<typename Number, typename Label>
  float gini(const std::vector<Number>& ,const std::vector<Label>&, int, Number);

  template<class Number, class Label>
  class TreeNode
  {
    public:
      TreeNode(const std::vector<std::vector<Number> >&,const std::vector<Label>&);
      Label predictexample(std::vector<Number>);
      void prunetree(TreeNode);
    private:
      //do we need this?
      std::vector<Number> splits; //keeps the remaining splits if we were to build from this node

      bool stump; //if stump is true we will predict this stump value
      Label prediction; //value to predict if it is a stump
      int attribute; // the attribute we will use to check against
      Number breakpoint; // the breakpoint at that attribute

      TreeNode *left;  //pointer to the node if our split is false
      TreeNode *right; //pointer to the node if our split is true
  };
}

#endif

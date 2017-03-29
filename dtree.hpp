
#ifndef DTREE_H
#define DTREE_H

#include <memory>


  // template<
  //     typename T, 
  //     template<typename=T, typename=allocator<T> > typename A,
  //     template<typename=A<T>, typename=allocator<A<T> > > typename M > 


namespace dtree
{
  using std::allocator;

  template<
      typename T, 
      template<typename=T, typename=allocator<T> > typename A>
  int numdifferentelements(const A<T> &);

  template<typename T>
  bool compare(const T&, const T&);

  template<
      typename T, 
      template<typename=T, typename=allocator<T> > typename A>
  float calcscore(const A<T> &,const int &);

  template<
      typename T, 
      template<typename=T, typename=allocator<T> > typename A>
  A<T> countclasses(const A<T> &,const int &);

  template<
      typename T, 
      template<typename=T, typename=allocator<T> > typename A,
      template<typename=A<T>, typename=allocator<A<T> > > typename M > 
  M<A<T> > findallsplits(const M<A<T> > &);

  template<
      typename T, 
      template<typename=T, typename=allocator<T> > typename A >
  float gini(const A<T>& ,const A<T>&, int, T);

  
  template<
      typename T, 
      template<typename=T, typename=allocator<T> > typename A,
      template<typename=A<T>, typename=allocator<A<T> > > typename M > 
  class TreeNode
  {
    public:
      TreeNode(const M<A<T> >&,const A<T>&);
      T predictexample(A<T>); //A is an example
      void prunetree(TreeNode);
    private:
      //do we need this? seems like after we find the splits we wont need to keep track of them
      //A<T> splits; //keeps the remaining splits if we were to build from this node, we won't have to keep calculating (meh too much to worry about)
      //int numclasses; 
      
      bool stump; //if stump is true we will predict this stump value
      T prediction; //value to predict if it is a stump
      int featuer; // the attribute we will use to check against
      T breakpoint; // the breakpoint at that attribute

      TreeNode *left;  //pointer to the node if our split is false
      TreeNode *right; //pointer to the node if our split is true
  };


}

#endif

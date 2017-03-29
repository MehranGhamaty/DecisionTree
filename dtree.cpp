
#include <vector>
#include <algorithm>  //for std::max_element, std::sort
#include <limits>     //for std::numeric_limits
#include <iostream>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/mpl/math/is_even.hpp>

#include "dtree.hpp"

namespace dtree {

using std::vector;
using std::cout;
using std::endl;
namespace ada = boost::adaptors;
namespace ran = boost::range;

template<typename Label>
float calcscore(const vector<Label> &classcounts,const int& totalsize) {
  int score = 0;
  for(const Label& count: classcounts)
    score += count * (totalsize - count);
  return (score/totalsize);
}

template<typename Label>
vector<Label> countclasses(const vector<Label>& classes, const int& numclasses){
  vector<Label> count(numclasses, 0);
  ran::for_each(classes, [&](Label i) {count.at(i)++;});
  return count;
}

template <typename T>
inline bool compare(T a, T b)
{
  return a > b;
}

vector<vector<float> > findallsplits(const vector<vector<float> > &X)
{
  vector<vector<float> > allsplits(X.size());
  int i = 0;
  for(auto attrib : X) {
    boost::sort(attrib);
    attrib.erase( unique( attrib.begin(), attrib.end() ), attrib.end() );

    vector<float> splits(attrib.size()-1);
    for(int i = 0; i < attrib.size()-1; i++) 
        splits[i] = (float)((attrib.at(i) + attrib.at(i+1))/2);
    allsplits[i++] = splits;
  }
  return allsplits;
}

//This x is the attribute
template <typename Number, typename Label>
float gini(const vector<Number> &x,const vector<Label> &y,
                         int numclasses, Number split) {
  
  vector<int> trueatt; vector<int> falseatt;

  for(int i = 0; i < x.size();i++)
    if(dtree::compare(x.at(i), split))
      trueatt.push_back(y.at(i));
    else
      falseatt.push_back(y.at(i));

  //probably a better way to do this
  int trueval = calcscore(countclasses(trueatt, numclasses), trueatt.size());
  int falseval = calcscore(countclasses(falseatt, numclasses), falseatt.size());
  std::cout << "t/f: " << trueatt.size() << ", " << falseatt.size() << std::endl;
  std::cout << "t/f: " << trueval << ", " << falseval << std::endl;
  return trueval + falseval;
}


template<typename Number, typename Label>
void TreeNode<Number, Label>::prunetree(
  TreeNode<Number, Label> tree)
{
  //TODO
}

template<typename Number,typename Label>
TreeNode<Number, Label>::TreeNode(const vector< vector<Number> >& X,const vector<Label> &y)
{
  //lets get the number of classes
  int numclasses = 0; auto ytmp = y;
  boost::sort(ytmp); 
  ran::for_each(ytmp | ada::uniqued ,[&] (int i) { ++numclasses; });

  //lets get the occurance of each class
  cout << "number of classes " << numclasses << endl;
  vector<int> classcounts = countclasses(y, numclasses);

  //get the location of the max element (not sure if below is correct)
  this->prediction = *std::max_element(y.begin(), y.end());

  //now we need to find all our splits in X
  vector<vector<Number> > allsplits = findallsplits(X);
  
  /*
  for(const auto& row : allsplits)
  {
    for(const auto& val : row)
      std::cout << val << "\t";
    std::cout << std::endl;
  }
  */
  //TODO: there is a problem with integers, we can also remove duplicates
  //That means fixing findallsplits

  int bestsplit = 0;
  this->attribute = 0;
  this->breakpoint = 0;

  Number currscore = std::numeric_limits<Number>::max(),bestscore=std::numeric_limits<Number>::max();

  std::cout << "test" << allsplits.size() << std::endl << std::flush;

  for(int atti = 0; atti < X.size(); atti++)
  {
    std::cout << "atti " << atti << std::endl;
    //never gets inside of this
    for(int spliti = 0; spliti < allsplits.at(atti).size(); spliti++)
    {
      std::cout << "spliti" << spliti << " atti"<< atti << std::endl << std::flush;

      //error thrown here at the second .at 
      currscore = gini(X.at(atti), y, numclasses, allsplits.at(atti).at(spliti));
      //lower is better
      if(bestscore > currscore)
      {
        bestscore = currscore;
        bestsplit = spliti;
        this->attribute = atti;
        this->breakpoint = allsplits.at(atti).at(spliti);
      }
    }
  }

  //need a better way of passing values
  vector<vector<Number> > trueX; vector<Label> truey;
  vector<vector<Number> > falseX; vector<Label> falsey;
  vector<Number> tmp;

  for(int i = 0; i < X.at(this->attribute).size(); i++)
  {
    if(dtree::compare(X.at(this->attribute).at(i), this->breakpoint))
    {
      for(int t = 0; t < X.size(); t++)
        tmp.push_back(X.at(t).at(i));
      trueX.push_back(tmp);
      truey.push_back(y.at(i));
    } else {
      for(int f = 0; f < X.size(); f++)
        tmp.push_back(X.at(f).at(i));
      falseX.push_back(tmp);
      falsey.push_back(y.at(i));
    }
    tmp.clear();
  }

  this->left = new TreeNode(trueX, truey);
  this->right = new TreeNode(falseX, falsey);   
}

template<typename Number,typename Label>
Label TreeNode<Number,Label>::predictexample(vector<Number> x)
{
  if(this->stump)
    return this->prediction;
  if(dtree::compare(x.at(this->attribute),this->breakpoint))
    return this->right->predictexample(x);
  else 
    return this->left->predictexample(x);
}

}


#include <vector>
#include <algorithm>  //for std::max_element, std::sort
#include <limits>     //for std::numeric_limits
#include <set>
#include <iostream>
#include "dtree.hpp"

namespace dtree
{

template<typename Label>
int numdifferentelements(const std::vector<Label> &v)
{
  std::set<int> s(v.begin(), v.end());
  return s.size();
}


template<typename Label>
float calcscore(const std::vector<Label> &classcounts,const int& totalsize)
{
  int score = 0;
  for(const Label& count: classcounts){
    score += count * (totalsize - count);
  }
  std::cout<< "score" << score << std::endl;
  std::cout<< "totalsize" << totalsize << std::endl;
  return (score/totalsize);
}

template<typename Label>
std::vector<Label> countclasses(const std::vector<Label>& classes,const int& numclasses)
{
  std::vector<Label> count(numclasses, 0);
  for(const Label& val: classes)
    count.at(val) += 1;
  return count;
}

template <typename T, typename O>
inline bool compare(T a, O b)
{
  return a > b;
}


//assumes sorted vector
template <typename Number>
std::vector<Number> findsplits(const std::vector<Number> &input)
{
  std::vector<Number> splits(input.size()-1);  
  for(int i = 0; i < input.size()-1; i++)
    splits[i] = (Number)((input.at(i) + input.at(i+1))/2);
  splits.erase( unique( splits.begin(), splits.end() ), splits.end() );
  return splits;
}

template <typename Number>
std::vector<std::vector<Number> > findallsplits(const std::vector<std::vector<Number> > &X)
{
  std::vector<std::vector<Number> > allsplits(X.size());
  int i = 0;
  for(std::vector<Number> attrib : X)
  {
    std::cout << "test inside our unique attrib" << std::endl;
    //remove duplicates then sort or remove duplicates while sorting
    std::sort(attrib.begin(), attrib.end());
    attrib.erase( unique( attrib.begin(), attrib.end() ), attrib.end() );
    allsplits[i] = findsplits(attrib);
    ++i;
  }
  return allsplits;
}

//This x is the attribute
template <typename Number, typename Label>
float gini(const std::vector<Number> &x,const std::vector<Label> &y,
                         int numclasses, Number split)
{
  std::vector<int> trueatt; std::vector<int> falseatt;

  for(int i = 0; i < x.size();i++)
    if(compare(x.at(i), split))
      trueatt.push_back(y.at(i));
    else
      falseatt.push_back(y.at(i));

  std::cout << "t/f: " << trueatt.size() << ", " << falseatt.size() << std::endl;
  int trueval = calcscore(countclasses(trueatt, numclasses), trueatt.size());
  int falseval = calcscore(countclasses(falseatt, numclasses), falseatt.size());
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
TreeNode<Number, Label>::TreeNode(const std::vector< std::vector<Number> >& X,const std::vector<Label> &y)
{
  int numclasses = numdifferentelements(y);
  std::vector<int> classcounts = countclasses(y, numclasses);
  //get the location of the max element (not sure if below is correct)
  this->prediction = *std::max_element(y.begin(), y.end());

  std::vector<std::vector<Number> > allsplits = findallsplits(X);
  
  for(const auto& row : allsplits)
  {
    for(const auto& val : row)
      std::cout << val << "\t";
    std::cout << std::endl;
  }
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

  std::vector<std::vector<Number> > trueX; 
  std::vector<Label> truey;
  std::vector<std::vector<Number> > falseX; 
  std::vector<Label> falsey;
  std::vector<Number> tmp;

  //Error is below this->attribute = -1312321312 aka junk
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
Label TreeNode<Number,Label>::predictexample(std::vector<Number> x)
{
  if(this->stump)
    return this->prediction;
  if(compare(x.at(this->attribute),this->breakpoint))
    return this->right->predictexample(x);
  else 
    return this->left->predictexample(x);
}

}

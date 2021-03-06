
#include <vector>
#include <algorithm>  //for std::max_element, std::sort
#include <limits>     //for std::numeric_limits
#include <iostream>

#include <unordered_map>
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptors.hpp>

#include "dtree.hpp"

namespace dtree {
  using std::allocator;

  template <typename T, typename O>
  inline bool compare(T a, O b) {return a < b;}

  template<typename Label, template<typename=Label, typename=allocator<Label> > typename Labels>
  int numdifferentelements(const Labels<Label> &v) {
    std::set<int> s(v.begin(), v.end()); return s.size();
  }

  template<typename Label, template<typename=Label, typename=allocator<Label> > typename Labels>
  float calcscore(const Labels<Label> &classcounts,const int& totalsize) {
    float score = 0;
    for(const Label& count: classcounts){score += count * (totalsize - count);}
    return (score/totalsize);
  }

  template<typename Label, template<typename=Label, typename=allocator<Label> > typename Labels>
  Labels<Label> countclasses(const Labels<Label>& classes,const int& numclasses) {
    Labels<Label> count((*std::max_element(classes.begin(), classes.end())) + 1, 0); //well should be max of classes
    boost::range::for_each(classes, [&](Label i) {++count.at(i);});
    return count;
  }

  //Should change this a bit, so it returns an allocator ordered map of numeric
  template <typename Number,
            template <typename=Number, typename=allocator<Number> > typename Attribute,
            template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X>
    X<Attribute<Number> > findallsplits(const X<Attribute<Number> > &Xtofindsplits) {
    X<Attribute<Number> > allsplits(Xtofindsplits.size());
    int i = 0;
    for(Attribute<Number> attrib : Xtofindsplits) {
      // std::cout << "test inside our unique attrib" << std::endl;
      //remove duplicates then sort or remove duplicates while sorting
      std::sort(attrib.begin(), attrib.end());
      attrib.erase( unique( attrib.begin(), attrib.end() ), attrib.end() );
      Attribute<Number> splits(attrib.size()-1);  
      for(int i = 0; i < attrib.size()-1; i++)
        splits[i] = (Number)((attrib.at(i) + attrib.at(i+1))/2);
      splits.erase( unique( splits.begin(), splits.end() ), splits.end() );
      allsplits[i] = splits;
      ++i;
    }
    return allsplits;
  }

  //This x is the attribute y should be the same size
  template <typename Number, 
            template<typename=Number, typename=allocator<Number> > typename Attribute,
            typename Label,
            template<typename=Label, typename=allocator<Label> > typename Labels>
  float gini(const Attribute<Number> &x,const Labels<Label> &y, int numclasses, Number split) {
    std::vector<int> trueatt; std::vector<int> falseatt;
    // std::cout << "x size " << x.size() << std::endl;
    // std::cout << "y size " << y.size() << std::endl;
    for(int i = 0; i < x.size();++i) 
      if(dtree::compare(x.at(i), split))
        trueatt.push_back(y.at(i));
      else
        falseatt.push_back(y.at(i));

    float trueval = calcscore(countclasses(trueatt, numclasses), trueatt.size());
    float falseval = calcscore(countclasses(falseatt, numclasses), falseatt.size());
    // std::cout << "t/f size: " << trueatt.size() << ", " << falseatt.size() << std::endl; //this is correct
    // std::cout << "t/f: " << trueval << ", " << falseval << std::endl;
    return trueval + falseval;
  }

  template<
        typename Number, 
        template<typename=Number, typename=allocator<Number> > typename Example,
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X,
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename Labels>
  int TreeNode<Number, Example, Attribute, X, Label, Labels>::getchildrenerrors(){
    if(this->stump)
      return this->errorswithstump;
    return this->left->getchildrenerrors() + this->right->getchildrenerrors();
  }

  //How do I prune again?
  //compare error with subtree vs just taking the stub
  template<
        typename Number, 
        template<typename=Number, typename=allocator<Number> > typename Example,
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X,
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename Labels>
  void TreeNode<Number, Example, Attribute, X, Label, Labels>::prunetree(const X<Attribute<Number> >& Xprune, const Labels<Label> &y) {
    //count all of the errors if we were to take the stub value
    this->errorswithstump = 0;
    boost::range::for_each(y, [&](auto p){if(p != this->prediction) ++this->errorswithstump;});
    std::cout << "errors at this node is " << this->errorswithstump << std::endl;
    if(!this->stump){
      std::vector<std::vector<Number> > trueX(Xprune.size(), std::vector<Number>()); std::vector<Label> truey(0);
      std::vector<std::vector<Number> > falseX(Xprune.size(), std::vector<Number>()); std::vector<Label> falsey(0);
      for(int i = 0; i < Xprune.at(this->attribute).size(); i++) {
        if(dtree::compare(Xprune.at(this->attribute).at(i), this->breakpoint)) {
          for(int t = 0; t < Xprune.size(); t++) trueX.at(t).push_back(Xprune.at(t).at(i));
          truey.push_back(y.at(i));
        } else {
          for(int f = 0; f < Xprune.size(); f++) falseX.at(f).push_back(Xprune.at(f).at(i));
          falsey.push_back(y.at(i));
        }
      }

      this->right->prunetree(trueX, truey);
      this->left->prunetree(falseX, falsey);
      // std::cout << "errors at this node is " << this->errorswithstump
      //   << " errors at true child " << this->right->errorswithstump
      //   << " errors at false child " << this->left->errorswithstump << std::endl;
        //we don't want the childrens errorwith stump but the errors on all stumps that are below the current node

      if(this->errorswithstump < this->right->getchildrenerrors() + this->left->getchildrenerrors()){
        delete this->right;
        delete this->left;
        this->stump = true;
        // std::cout << "making stump" << std::endl;
      }
    }
  }

  template<
        typename Number, 
        template<typename=Number, typename=allocator<Number> > typename Example,
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X,
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename Labels>
  TreeNode<Number, Example, Attribute, X, Label, Labels>::TreeNode(const X<Attribute<Number> >& Xtrain,const Labels<Label> &y) {
    // std::cout << "Making a Tree Node======================================================" << std::endl;
    int numclasses = numdifferentelements(y); std::vector<int> classcounts = countclasses(y, numclasses);

    //check if only one label left here and make it a stump if so
    if(boost::accumulate(y, std::pair<bool, int>(true,y.at(0)), [](auto p, auto e){return std::pair<bool,int>(p.first and (e == p.second),p.second);}).first){
      this->prediction = y.at(0); this->stump = true; return;
    }

    //find the mode of y...
    std::unordered_map<Label,int> m; for(const auto &v:y){ if(m.find(v) == m.end()) { m[v] = 1; } else { m[v]++; }}
    auto maxpair = boost::accumulate(m, std::pair<int,int>(-1,-1), [](auto p, auto n) -> std::pair<const int, int>{ if(n.second > p.second) return n; else return p;});
    this->prediction = maxpair.first;

    //find all splits
    std::vector<std::vector<Number> > allsplits = findallsplits(Xtrain);
    
    //print out all the splits, if none of the attributes have splits we should end the construction
    if(boost::accumulate(allsplits, true, [](bool p, auto v){return p and v.empty();})) {
      this->stump = true; return;
    } else { this->stump = false; }

    int bestsplit = -1; this->attribute = 0; this->breakpoint = 0;
    Number currscore,bestscore=std::numeric_limits<Number>::max();
    for(int atti = 0; atti < Xtrain.size(); atti++) {
      for(int spliti = 0; spliti < allsplits.at(atti).size(); spliti++) {
        currscore = gini(Xtrain.at(atti), y, numclasses, allsplits.at(atti).at(spliti)); //gini score calc is wrong
        std::cout << "gini score for attribute " << atti << " with split " << allsplits.at(atti).at(spliti) << " is " << currscore << std::endl;
        //lower is better for gini higher is better for entropy
        if(bestscore > currscore){
          bestscore = currscore; bestsplit = spliti;
          this->attribute = atti; this->breakpoint = allsplits.at(atti).at(spliti);
        }
      }
    }

    std::vector<std::vector<Number> > trueX(Xtrain.size(), std::vector<Number>()); std::vector<Label> truey(0);
    std::vector<std::vector<Number> > falseX(Xtrain.size(), std::vector<Number>()); std::vector<Label> falsey(0);
    // std::cout << "picking attribute " << this->attribute << std::endl;
    // std::cout << "split is " << this->breakpoint << std::endl; //breakpiont not set?
    for(int i = 0; i < Xtrain.at(this->attribute).size(); i++) {
      if(dtree::compare(Xtrain.at(this->attribute).at(i), this->breakpoint)) {
        for(int t = 0; t < Xtrain.size(); t++) trueX.at(t).push_back(Xtrain.at(t).at(i));
        truey.push_back(y.at(i));
      } else {
        for(int f = 0; f < Xtrain.size(); f++) falseX.at(f).push_back(Xtrain.at(f).at(i));
        falsey.push_back(y.at(i));
      }
    }
    
    // std::cout << "making True stuff" << std::endl;
    // std::cout << "truey" << '\t'; boost::range::copy(truey, std::ostream_iterator<Number>(std::cout, "\t")); std::cout<< std::endl;
    // std::cout << "trueX" << '\n'; for(const auto& feat: trueX){ boost::range::copy(feat, std::ostream_iterator<Number>(std::cout, "\t")); std::cout << std::endl;}
    if(truey.size() > 0) this->right = new TreeNode(trueX, truey);
    // std::cout << "making False stuff" << std::endl;
    // std::cout << "falsey" << '\t'; boost::range::copy(falsey, std::ostream_iterator<Number>(std::cout, "\t")); std::cout<< std::endl;
    // std::cout << "falseX" << '\n'; for(const auto& feat: falseX){boost::range::copy(feat, std::ostream_iterator<Number>(std::cout, "\t")); std::cout << std::endl;}
    if(falsey.size() > 0) this->left = new TreeNode(falseX, falsey);   
  }

  template<
        typename Number, 
        template<typename=Number, typename=allocator<Number> > typename Example,
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X,
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename Labels>
  Label TreeNode<Number, Example, Attribute, X, Label, Labels>::predictexample(Example<Number> x) {
    if(this->stump)
      return this->prediction;
    if(dtree::compare(x.at(this->attribute),this->breakpoint))
      return this->right->predictexample(x);
    else 
      return this->left->predictexample(x);
  }

}
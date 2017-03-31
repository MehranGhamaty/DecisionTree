#include <vector>
#include <iostream>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/adaptors.hpp>

using std::cout;
using std::endl;
using std::vector;

namespace ada = boost::adaptors;
namespace ran = boost::range;

vector<vector<float> >  test1(vector<vector<float> > Xtrain) {
    vector<vector<float> > allsplits(Xtrain.size());
    int i = 0;
    //this is expensive and not good
    for(vector<float> attrib : Xtrain){
        //allocating a lot of stuff maybe not the best
        //cout << "loop" << endl;
        vector<float> uniquevals; vector<float> splits;  
        //we get the unique set of values
        boost::sort(attrib); 
        ran::transform(attrib | ada::uniqued, std::back_inserter(uniquevals), [](float i) {return i;}); 
        //now we rotate them, so we can get use a binary operation to get the break points
        auto uniquevals2 = uniquevals; ran::rotate(uniquevals2, uniquevals2.begin()+1);
        // ran::erase(uniquevals, boost::make_iterator_range(uniquevals.end()-1,uniquevals.end())); 
        // ran::erase(uniquevals2, boost::make_iterator_range(uniquevals2.end()-1,uniquevals2.end())); //its actually slower maybe there is a better way to do this?
        //now lets actually get the break points
        ran::transform(uniquevals, uniquevals2, std::back_inserter(splits), [](float x, float y) {return (x+y)/2;}); 
        //the last  element is useless (we might want to chop values off before doing calc....)

        // ran::copy(attrib, std::ostream_iterator<float>(cout, " ")); cout << endl;
        // ran::copy(uniquevals, std::ostream_iterator<float>(cout, " ")); cout << endl;
        // ran::copy(uniquevals2, std::ostream_iterator<float>(cout, " ")); cout << endl;
        // ran::copy(splits, std::ostream_iterator<float>(cout, " ")); cout << endl;
        allsplits[i] = splits;
        ++i;
    }

    return allsplits;
}

vector<vector<float> > test2(const vector<vector<float> > &X)
{
  vector<vector<float> > allsplits(X.size());
  int i = 0;
  for(auto attrib : X) {
    boost::sort(attrib);
    attrib.erase( unique( attrib.begin(), attrib.end() ), attrib.end() );

    vector<float> splits(attrib.size()-1);
    for(int i = 0; i < attrib.size()-1; i++) 
        splits[i] = (float)((attrib.at(i) + attrib.at(i+1))/2);
    //splits.erase( unique( splits.begin(), splits.end() ), splits.end() ); //no need
    allsplits[i++] = splits;
    //++i;
  }
  return allsplits;
}



//need to compare to old solution∀
int main() {
    vector<vector<float> > Xtrain = 
    {
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
        {1, 1, 2, 3, 3, 3, 1, 2, 2, 2},
        {1, 2, 3, 4, 1, 1, 4, 2, 3, 3},
        {2, 2, 1, 0, 2, 1, 1, 2, 2, 1},
    };
    clock_t start = clock();
    auto t1 = test1(Xtrain);
    clock_t end = clock();
    double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
    cout << "Test1 Took: " << time << "s" << endl;
    
    start = clock();
    auto t2 = test2(Xtrain);
    end = clock();
    time = (double) (end-start)/ CLOCKS_PER_SEC * 1000.0;
    cout << "Test2 Took: " << time << "s" << endl;


    ran::copy(t1[0], std::ostream_iterator<float>(cout, " ")); cout << endl; // has an extra item in each row
    ran::copy(t2[0], std::ostream_iterator<float>(cout, " ")); cout << endl; // doesn't have extra'
    
    
    //vector<vector<float> > allsplits(Xtrain.size());
}
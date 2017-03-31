
#ifndef DTREE_H
#define DTREE_H

#include <ostream>
#include <memory> // for allocator

namespace dtree
{
    using std::allocator;
    
    template <typename T, typename O> 
    inline bool compare(T, O);

    template <
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename y>
    int numdifferentelements(const y<Label> &);

    template <
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename y>
    float calcscore(const y<Label> &,const int &);

    template <
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename y>
    y<Label> countclasses(const y<Label> &,const int &);


    template <
        typename Number,
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X> 
    X<Attribute<Number> > findallsplits(const X<Attribute<Number> >&);
    //maybe use a hashtable instead of sorting and get the splits in set? (might need to change template)

    template<
        typename Number, 
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename Labels>
    float gini(const Attribute<Number>& ,const Labels<Label>&, int, Number);


    template<
        typename Number, 
        template<typename=Number, typename=allocator<Number> > typename Example,
        template<typename=Number, typename=allocator<Number> > typename Attribute,
        template<typename=Attribute<Number>, typename=allocator<Attribute<Number> > > typename X,
        typename Label,
        template<typename=Label, typename=allocator<Label> > typename Labels>
    class TreeNode {
        public:
        TreeNode(const X<Attribute<Number> >&,const Labels<Label>&);
        Label predictexample(Example<Number>);
        void prunetree(const X<Attribute<Number> >&,const Labels<Label>&);
        int getchildrenerrors();
        friend std::ostream& operator<<(std::ostream& stream, const TreeNode& tree){
            if(tree.stump){
                stream << "predict "<< tree.prediction << std::endl;
            } else {
                stream << "if X(" << tree.attribute << ") < " << tree.breakpoint << " then " << std::endl 
                    << *tree.right << "else" << std::endl << *tree.left;
            }
            return stream;
        }
        private:
        //std::vector<Number> splits; //keeps the remaining splits if we were to build from this node
        int errorswithstump; //used for pruning
        bool stump; //if stump is true we will predict this stump value
        Label prediction; //value to predict if it is a stump
        int attribute; // the attribute we will use to check against
        Number breakpoint; // the breakpoint at that attribute

        TreeNode *left;  //pointer to the node if our split is false
        TreeNode *right; //pointer to the node if our split is true
    };

}

#endif

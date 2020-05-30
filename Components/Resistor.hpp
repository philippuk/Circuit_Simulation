#ifndef Resistor_hpp
#define Resistor_hpp

#inlcude "Component.hpp"
#include "Node.hpp"

using namespace std;

class Resistor:public Component
{
protected:
    double resistance;
    Node node_head;
    Node node_tail;
public:
    Resistor(double res,Node node1, Node node2)
    {
        resistance=res;
        node_head=node1;
        node_tail=node2;
    }

    double conductance(){
        return 1/resistance;
    }
    
    //How to define +ve or -ve
    double current(){
        return (node_head.voltage - node_tail.voltage)/resistance;
    }
}

#endif

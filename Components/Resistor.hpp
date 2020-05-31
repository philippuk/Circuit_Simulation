#ifndef Resistor_hpp
#define Resistor_hpp

#include "Component.hpp"
#include "Node.hpp"

using namespace std;

class Resistor:public Component
{
protected:
    double resistance;
public:
    Resistor(string name, double res,Node node1, Node node2)
    {
        com_name=name;
        resistance=res;
        node_pos=node1;
        node_neg=node2;
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

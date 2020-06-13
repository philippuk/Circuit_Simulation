#ifndef Diode_hpp
#define Diode_hpp

#include "Component.hpp"
#include "Resistor.hpp"
#inlcude "VoltageSource.hpp"

#include <cmath>

using namespace std;

class Diode:public Component
{
protected:
    string model_name;
    VoltageSource Vsrc;
    Resistor rd;
public:
    Diode(string name, Node * anode, Node * cathode)
    {x
        com_name=name;
        node_pos=cathode;
        node_neg=anode;
    }

    double voltage(){
        return node_neg->voltage-node_pos->voltage;
    }
    
    double current(double voltage){
        return pow(10,-15)*exp(voltage/(25*pow(10,-3)));
    }

    vector<double> access(){
        vector<double> tmp;
        return tmp;
    }

};


#endif /* Diode_hpp */

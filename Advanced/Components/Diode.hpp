#ifndef Diode_hpp
#define Diode_hpp

#include "Component.hpp"

#include <cmath>
#include <vector>

using namespace std;

class Diode:public Component
{
protected:
    Resistor * Rd;
public:
    vector<double> guess_voltage={0};
    Diode(string name, Node *anode, Node *cathode, Resistor* R)
    {
        com_name=name;
        node_pos=cathode;
        node_neg=anode;
        Rd=R;
    }

    double voltage(){
        return node_neg->voltage-node_pos->voltage;
    }
    
    double current(){
        double i=pow(10,-15)*exp(guess_voltage.back()/(25*pow(10,-3)));
        return i;
    }

    void d_resistance(){
        double i=pow(10,-15)*exp(guess_voltage.back()/(25*pow(10,-3)));
        Rd->resistance=25e-3/i;
    }

    vector<double> access(){
        return {};
    }

};


#endif /* Diode_hpp */

#ifndef Diode_hpp
#define Diode_hpp

#include "Component.hpp"
#include "VoltageSource.hpp"
#include "Resistor.hpp"

#include <cmath>
#include <vector>

using namespace std;

class Diode:public Component
{
protected:
    VoltageSource* Vth;
    Resistor * Rth;
public:
    vector<double> guess_voltage={1};
    Diode(string name, Node *anode, Node *cathode, Resistor* R, VoltageSource* V)
    {
        com_name=name;
        node_pos=cathode;
        node_neg=anode;
        Rth=R;
        Vth=V;
    }

    double voltage(){
        return node_neg->voltage-node_pos->voltage;
    }
    
    double current(){
        return pow(10,-15)*exp(guess_voltage.back()/(25*pow(10,-3)));
    }

    void th_resistance(){
        double i=pow(10,-15)*exp(guess_voltage.back()/(25*pow(10,-3)));
        Rth->resistance=25e-3/i;
    }

    void th_voltage(){
        Vth->Offset=guess_voltage.back()-25e-3;
    }

    vector<double> access(){
        return {};
    }

};


#endif /* Diode_hpp */

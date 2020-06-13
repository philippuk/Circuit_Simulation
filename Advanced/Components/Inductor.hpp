#ifndef Inductor_hpp
#define Inductor_hpp

#include "Component.hpp"

#include <vector>
#include <unordered_map>
#include <cassert>

using namespace std;

class Inductor: public Component
{
protected:
    double inductance;
    double voltage_history=0;
public:
    Inductor(string name,double ind, Node* node1, Node* node2)
    {
        com_name=name;
        inductance=ind;
        node_pos=node1;
        node_neg=node2;
    }


    //calculate the value of current source at particular instant
    double current(){
        return voltage_history/inductance;
    }

    //override change_time to input voltage history
    void change_time(){
        voltage_history += (node_neg->voltage-node_pos->voltage)*timestep();
        com_time += com_timestep;
    }

    vector<double> access(){
        vector<double> tmp;
        tmp.push_back(inductance);
        return tmp;
    }
};
#endif /* Inductor_hpp */

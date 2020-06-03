#ifndef Inductor_hpp
#define Inductor_hpp

#include "Component.hpp"

#include <vector>

using namespace std;

class Inductor: public Component
{
protected:
    double inductance;
    vector<double> voltage_history;
public:
    Inductor(string name,double ind, Node node1, Node node2)
    {
        com_name=name;
        inductance=ind;
        node_pos=node1;
        node_neg=node2;
        voltage_history.push_back(0);
    }
    
    //calculate the value of current source at particular instant
    double current(){
        double sum;
        if (voltage_history.size()==1){
            return 0;
        }else{
            for(int i=0; i<voltage_history.size();i++){
                sum += voltage_history[i]*timestep();
            }
            return sum/inductance;
        }
    }

    double voltage(){
        return 0;
    }
    
    //input voltage history
    void voltage_input(double voltage){
        voltage_history.push_back(voltage);
    }
};
#endif /* Inductor_hpp */

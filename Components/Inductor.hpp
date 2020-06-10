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
    Inductor(string name,double ind, Node* node1, Node* node2)
    {
        com_name=name;
        inductance=ind;
        node_pos=node1;
        node_neg=node2;
    }
    
    //calculate the value of current source at particular instant
    double current(){
        double sum;
        if (voltage_history.size()==0){
            return 0;
        }else{
            for(int i=0; i<voltage_history.size();i++){
                sum += voltage_history[i]*timestep();
            }
            return sum/inductance;
        }
    }

    //override change_time to input voltage history
    void change_time(){
        com_time += com_timestep;
        cerr<<node_pos->voltage-node_neg->voltage<<endl;
        voltage_history.push_back(node_pos->voltage-node_neg->voltage);
    }

     vector<double> access(){
        vector<double> tmp;
        tmp.push_back(inductance);
        return tmp;
    }
};
#endif /* Inductor_hpp */

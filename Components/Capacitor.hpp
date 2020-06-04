#ifndef Capacitor_hpp
#define Capacitor_hpp

#include "Component.hpp"

#include <vector>

using namespace std;

class Capacitor:public Component
{
protected:
    double capacitance;
    double voltage_history;
    vector<double> current_history;
public:
    Capacitor(string name,double cap, Node* node1, Node* node2)
    {
        com_name=name;
        capacitance=cap;
        node_pos=node1;
        node_neg=node2;
        current_history.push_back(0);
    }

    double current(){
        return (voltage()-voltage_history)/timestep()*capacitance;
    }
   
    //calculate the value of voltage source at particular instant
    double voltage() override{
        double sum;
        if (current_history.size()==1){
            return 0;
        }else{
            for(int i=0; i<current_history.size();i++){
                sum += current_history[i]*timestep();
            }
            return sum/capacitance;
        }
    }
    
    //input current history
    void current_input(){
        current_history.push_back(current());
    }
    
    //input voltage history
    void voltage_input(){
        voltage_history=voltage();
    }
};

#endif /* Capacitor_hpp */

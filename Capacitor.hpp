#ifndef Capacitor_hpp
#define Capacitor_hpp

#include "Node.hpp"
#include "Component.hpp"

#include <vector>

using namespace std;

class Capacitor:public Component
{
protected:
    double capacitance;
    Node node_head;
    Node node_tail;
    double voltage_history;
    vector<double> current_history;
public:
    Capacitor(double cap, Node node1, Node node2)
    {
        capacitance=cap;
        node_head=node1;
        node_tail=node2;
        current_history.push_back(0);
    }

    double current(){
        return (voltage()-voltage_history)/timestep*capacitance;
    }
   
    //calculate the value of voltage source at particular instant
    double voltage(){
        double sum;
        if (current_history.size()==1){
            return 0;
        }else{
            for(int i=0; i<current_history.size();i++){
                sum += current_history[i]*timestep
            }
            return sum/capacitance;
        }
    }
    
    //input current history
    void current_input(double current){
        current_history.push_back(current);
    }
    
    //input voltage history
    void voltage_input(double voltage){
        voltage_history=voltage();
    }
    
}

#endif /* Capacitor_hpp */

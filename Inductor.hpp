#ifndef Inductor_hpp
#define Inductor_hpp

#include "Node.hpp"
#include "Component.hpp"

#include <vector>

using namespace std;

class Inductor: public Component
{
protected:
    double inductance;
    Node node_head;
    Node node_tail;
    vector<double> voltage_history;
public:
    Inductor(double ind, Node node1, Node node2)
    {
        inductance=ind;
        node_head=node1;
        node_tail=node2;
        voltage_history.push_back(0);
    }
    
    //calculate the value of current source at particular instant
    double current(){
        double sum;
        if (voltage_history.size()==1){
            return 0;
        }else{
            for(int i=0; i<voltage_history.size();i++){
                sum += voltage_history[i]*timestep
            }
            return sum/inductance;
        }
    }
    
    //input voltage history
    void voltage_input(double voltage){
        voltage_history.push_back(voltage);
    }
    
#endif /* Inductor_hpp */

#ifndef VoltageSource_hpp
#define VoltageSource_hpp

#include "Component.hpp"

#include <cmath>

using namespace std;

class VoltageSource:public Component
{
protected:
    double Amplitude;
    double Frequency;
    double Offset;
public:
    VoltageSource(string name, double a, double f, double o, Node node1, Node node2)
    {
        com_name=name;
        Amplitude=a;
        Frequency=f;
        Offset=o;
        node_pos=node1;
        node_neg=node2;
    }
    
    //calculate the value of voltage source
    double voltage(){
        return Amplitude*sin(2*M_PI*Frequency*time())+Offset ;
    }
    
    // double current(){
        
    // }
};
#endif /* VoltageSource_hpp */

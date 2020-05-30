#ifndef VoltageSource_hpp
#define VoltageSource_hpp

#include "Node.hpp"
#include "Component.hpp"

#include <cmath>

using namespace std;

class VoltageSource:public Component
{
protected:
    double Amplitude;
    double Frequency;
    double Offset;
    Node node_positive;
    Node node_negative;
public:
    VoltageSource(double a, double o, double f, Node node1, Node node2)
    {
        Amplitude=A;
        Frequency=f;
        Offset=o;
        node_positive=node1;
        node_negative=node2;
    }
    
    //calculate the value of voltage source
    double voltage(){
        return Amplitude*sin(2*M_PI*Frequency*time)+Offset ;
    }
    
    double current(){
        double sum
        for (int i=0;i<node_positive.component.size();i++){
            sum += node_positive.component[i].current;
        }
        return sum
    }
   

#endif /* VoltageSource_hpp */

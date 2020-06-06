#ifndef CurrentSource_hpp
#define CurrentSource_hpp

#include "Component.hpp"

#include <cmath>

using namespace std;

class CurrentSource:public Component
{
protected:
    double Amplitude;
    double Frequency;
    double Offset;
public:
    CurrentSource(string name,double a, double f, double o, Node* node1, Node* node2)
    {
        com_name=name;
        Amplitude=a;
        Frequency=f;
        Offset=o;
        node_pos=node2;
        node_neg=node1;
    }
    
    //calculate the value of current source
    double current(){
        return Amplitude*sin(2*M_PI*Frequency*time())+Offset;
    }
    
};
#endif /* CurrentSource_hpp */

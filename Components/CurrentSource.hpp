#ifndef CurrentSource_hpp
#define CurrentSource_hpp

#include "Node.hpp"
#include "Component.hpp"

#include <cmath>

using namespace std;

class CurrentSource:public Component
{
protected:
    double Amplitude;
    double Frequency;
    double Offset;
    Node node_in;
    Node node_out;
public:
    CurrentSource(double a, double o, double f, Node node1, Node node2)
    {
        Amplitude=A;
        Frequency=f;
        Offset=o;
        node_in=node1;
        node_out=node2;
    }
    
    //calculate the value of current source
    double current(){
        return Amplitude*sin(2*M_PI*Frequency*time)+Offset ;
    }
    
#endif /* CurrentSource_hpp */

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
    VoltageSource(string name, double a, double f, double o, Node* node1, Node* node2)
    {
        com_name=name;
        Amplitude=a;
        Frequency=f;
        Offset=o;
        node_pos=node1;
        node_neg=node2;
    }
    
    //calculate the value of voltage source
    double voltage() override{
        return Amplitude*sin(2*M_PI*Frequency*time())+Offset ;
    }
    
    double source_current(vector<Component *> list) override {
        double sum;
        for(int a=0;a<list.size();a++){
            if(list[a]->name()[0]=='V' && list[a]->node_positive()->name==node_pos->name){
                //todo
                continue;
            }else if(list[a]->name()[0]=='V' && list[a]->node_negative()->name==node_pos->name){
                sum = list[a]->source_current(list);
            }else if(list[a]->name()[0]=='I' && list[a]->node_positive()->name==node_pos->name){
                sum -= list[a]->current();
            }else if(list[a]->name()[0]=='I' && list[a]->node_negative()->name==node_pos->name){
                sum += list[a]->current();
            }else if(list[a]->node_negative()->name==node_pos->name||list[a]->node_positive()->name==node_pos->name){
                sum += list[a]->current();
            }
        }
        return sum;
    }
};
#endif /* VoltageSource_hpp */

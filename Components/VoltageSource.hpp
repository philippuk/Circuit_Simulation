#ifndef VoltageSource_hpp
#define VoltageSource_hpp

#include "Component.hpp"

#include <cmath>
#include <unordered_map>
#include <cassert>

using namespace std;

class VoltageSource:public Component
{
protected:
    double Amplitude;
    double Frequency;
    double Offset;
    bool circuit_is_valid(vector<Component *> list){
        unordered_map<Node*, int> nodes;
        for (int b = 0; b < list.size(); b++){
            nodes[list[b]->node_positive()]++;
            nodes[list[b]->node_negative()]++;
        }
        for (auto e : nodes){
            if (e.second == 1)
                return true;
        }
        return false;
    }
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
    double voltage(){
        return Amplitude*sin(2*M_PI*Frequency*time())+Offset ;
    }

    double source_current(vector<Component *> list){
        assert(circuit_is_valid(list));
        double sum;

        //search positive node have voltage source connected
        for(int a=0;a<list.size();a++){
            if(list[a]->name()[0]=='V' && (list[a]->node_positive()->name==node_pos->name||list[a]->node_negative()->name==node_pos->name)){
                if(list[a]->name()==com_name){
                    continue;
                }else{
                    break;
                }
            }
        }

        //search negative node have voltage source connected
        for(int a=0;a<list.size();a++){
            if(list[a]->name()[0]=='V' && (list[a]->node_positive()->name==node_neg->name||list[a]->node_negative()->name==node_neg->name)){
                if(list[a]->name()==com_name){
                    continue;
                }else{
                    break;
                }
            }
        }
            if(list[a]->name()[0]=='V' && list[a]->node_negative()->name==node_pos->name){
                sum += list[a]->source_current(list);
            }
            if(list[a]->name()[0]=='I' && list[a]->node_positive()->name==node_pos->name){
                sum -= list[a]->current();
            }
            if(list[a]->name()[0]=='I' && list[a]->node_negative()->name==node_pos->name){
                sum += list[a]->current();
            }
            if(list[a]->node_negative()->name==node_pos->name||list[a]->node_positive()->name==node_pos->name){
                sum += list[a]->current();
            }
        }
        return sum;
    }
};
#endif /* VoltageSource_hpp */

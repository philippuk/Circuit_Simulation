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
            if(list[b]->name()[0]=='V'||list[b]->name()[0]=='C'){
                nodes[list[b]->node_positive()]++;
                nodes[list[b]->node_negative()]++;
            }
        }
        for (auto e : nodes){
            if (e.second == 1)
                return true;
        }
        return false;
    }
    double node_current(vector<Component *> list, Node* node, Component* source){
        double res=0;
        for(int a=0;a<list.size();a++){
            if((list[a]->name()[0]=='V'||list[a]->name()[0]=='C') && list[a]->node_positive()->name==node->name && list[a]!=source){
                res += node_current(list, list[a]->node_negative(),list[a]);
            }
            if((list[a]->name()[0]=='V'||list[a]->name()[0]=='C') && list[a]->node_negative()->name==node->name && list[a]!=source){
                res += node_current(list, list[a]->node_positive(),list[a]);
            }
            if((list[a]->name()[0]=='I'||list[a]->name()[0]=='L') && list[a]->node_positive()->name==node->name){
                res -= list[a]->current();
            }
            if((list[a]->name()[0]=='I'||list[a]->name()[0]=='L') && list[a]->node_negative()->name==node->name){
                res += list[a]->current();
            }
            if(list[a]->name()[0]=='R' && (list[a]->node_negative()->name==node->name||list[a]->node_positive()->name==node->name)){
                res += list[a]->current();
            }
        }
        return res;
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

    //calculate the value of current by iterative method
    double source_current(vector<Component *> list){
        assert(circuit_is_valid(list));
        vector<Component*>pos_connect;
        vector<Component*>neg_connect;
        double sum=0;

        //search positive node have voltage source connected
        for(int a=0;a<list.size();a++){
            if((list[a]->name()[0]=='V'||list[a]->name()[0]=='C') && (list[a]->node_positive()->name==node_pos->name||list[a]->node_negative()->name==node_pos->name)){
                if(list[a]->name()==com_name){
                    continue;
                }else{
                    pos_connect.push_back(list[a]);
                }
            }
        }

        //search negative node have voltage source connected
        for(int a=0;a<list.size();a++){
            if((list[a]->name()[0]=='V'||list[a]->name()[0]=='C') && (list[a]->node_positive()->name==node_neg->name||list[a]->node_negative()->name==node_neg->name)){
                if(list[a]->name()==com_name){
                    continue;
                }else{
                    neg_connect.push_back(list[a]);
                }
            }
        }

        //use positive node for current calculation if there is less voltage source attached
        if(pos_connect.size()<=neg_connect.size()){
            sum=node_current(list, node_pos,this);
        }else{
            sum=node_current(list, node_neg,this);
        }

        return sum;
    }

    vector<double> access(){
        vector<double> tmp;
        tmp.push_back(Amplitude);
        tmp.push_back(Frequency);
        tmp.push_back(Offset);
        return tmp;
    }
};
#endif /* VoltageSource_hpp */

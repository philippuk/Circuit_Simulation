#ifndef Capacitor_hpp
#define Capacitor_hpp

#include "Component.hpp"

#include <vector>
#include <unordered_map>
#include <cassert>

using namespace std;

class Capacitor:public Component
{
protected:
    double capacitance;
    double current_history=0;
    double node_current(vector<Component *> list, Node* node, Component* source){
        double res;
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
public:
    Capacitor(string name,double cap, Node* node1, Node* node2)
    {
        com_name=name;
        capacitance=cap;
        node_pos=node1;
        node_neg=node2;
    }

    //calculate the value of current by iterative method
    double current(vector<Component *> list){
        assert(circuit_is_valid(list));
        vector<Component*>pos_connect;
        vector<Component*>neg_connect;
        double sum;

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

        if(com_time!=0){
            current_history += sum*timestep();
        }
        return sum;
    }

    //calculate the value of voltage source at particular instant
    double voltage(){
        return -current_history/capacitance;
    }

    void change_time(){
        com_time += com_timestep;
    }

    vector<double> access(){
        vector<double> tmp;
        tmp.push_back(capacitance);
        return tmp;
    }
};

#endif /* Capacitor_hpp */

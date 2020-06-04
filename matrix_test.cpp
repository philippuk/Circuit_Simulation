#include <iostream>
#include <vector>

#include "Node.hpp"
#include "Eigen/Dense"

#include "Components/CurrentSource.hpp"
#include "Components/VoltageSource.hpp"
#include "Components/Resistor.hpp"
#include "Components/Inductor.hpp"
#include "Components/Capacitor.hpp"
#include "Components/Component.hpp"

using namespace std;
using namespace Eigen;

//input part
double stoptime=10e-3;
double timestep=1e-6;
bool change=false;

Node N000={"N000",0};
Node N001={"N001",0};
Node N002={"N002",0};
Node N003={"N003",0};

VoltageSource VS("V1",0,0,1,N001,N000);
CurrentSource CS("I1",0,0,2e-3,N002,N003); 
Resistor R1("R1",10e3,N001,N002);
Resistor R2("R2",10e3,N003,N000);
Resistor R3("R3",10e3,N002,N000);

vector<Node> v_of_nodes={N001,N002,N003};
vector<Component *> component_list={&VS,&CS,&R1,&R2,&R3};

//calculate the value for the current vector
double v_current(Node node){
    for(int j=0;j<component_list.size();j++){
        if(component_list[j]->name()[0]=='V' && component_list[j]->node_positive().name==node.name){
            change=true;
            return component_list[j]->voltage();
        }else if(component_list[j]->name()[0]=='V' && component_list[j]->node_negative().name==node.name){
            change=true;
            return -component_list[j]->voltage();
        }
    }
    double sum;
    for(int j=0;j<component_list.size();j++){
        if(component_list[j]->name()[0]=='I' && component_list[j]->node_positive().name==node.name){
            sum += component_list[j]->current();
        }else if(component_list[j]->name()[0]=='I' && component_list[j]->node_negative().name==node.name){
            sum -= component_list[j]->current();
        }
    }
    return sum;
}

//calculate the value for the conductance matrix
double v_conductance(Node node1, Node node2){
    double sum=0;
    if(node1.name==node2.name){
        for(int j=0;j<component_list.size();j++){
            if(component_list[j]->name()[0]=='R' && (component_list[j]->node_positive().name==node1.name||component_list[j]->node_negative().name==node1.name)){
                sum += component_list[j]->conductance();
            }
        }
    }else{
        for(int j=0;j<component_list.size();j++){
            if(component_list[j]->name()[0]=='R' && component_list[j]->node_positive().name==node1.name && component_list[j]->node_negative().name==node2.name){
                sum += component_list[j]->conductance();
            }else if (component_list[j]->name()[0]=='R' && component_list[j]->node_positive().name==node2.name && component_list[j]->node_negative().name==node1.name){
                sum += component_list[j]->conductance();
            }
        }
    }
    return sum;
}

//input the value for the conductance matrix
vector<double> v_conductance_input(Node node,bool overtake){
    vector<double> res;
    if(overtake){
        for(int h=0;h<v_of_nodes.size();h++){
            res.push_back(0);
            res[stoi(node.name.substr(1,3))-1]=1;
        }
        return res;
    }else{
        for(int h=0;h<v_of_nodes.size();h++){
            if (v_of_nodes[h].name==node.name){
                res.push_back(v_conductance(v_of_nodes[h],node));
            }else{
                res.push_back(-v_conductance(v_of_nodes[h],node));
            }
        }
        return res;
    }
}
 
int main()
{
    for (int i=0;i<component_list.size();i++){
        component_list[i]->set_timestep(timestep);
    }

    for (int i=0;timestep*i<=1e-6;i++){
        //initialize three matrix
        MatrixXd  m_conductance(v_of_nodes.size(), v_of_nodes.size());
        VectorXd m_current(v_of_nodes.size());
        VectorXd m_voltage(v_of_nodes.size());

        //calculation for the three matrix
        for (int k=0;k<v_of_nodes.size();k++){
            m_current(k)=v_current(v_of_nodes[k]);
            vector<double> tmp=v_conductance_input(v_of_nodes[k],change);
            for (int l=0;l<tmp.size();l++){
                m_conductance(k,l)=tmp[l];
            }
            change=false;
        }
        cerr << "Here is the conductance matrix:\n" << m_conductance << endl;
        cerr << "Here is the current vector:\n" << m_current << endl;
        m_voltage = m_conductance.colPivHouseholderQr().solve(m_current);
        cerr << "The voltage vector is:\n" << m_voltage << endl;

        //progress to the next time interval
        for (int k=0;k<component_list.size();k++){
            component_list[k]->change_time(true); 
        }
    }
}



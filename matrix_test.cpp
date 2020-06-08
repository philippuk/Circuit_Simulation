#include <iostream>
#include <vector>
#include <set>
#include <cassert>

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
double timestep=10e-6;

Node N000={"N000",0};
Node N001={"N001",0};
Node N002={"N002",0};
Node N003={"N003",0};

VoltageSource VS("V1",0,0,1,&N001,&N000);
CurrentSource CS("I1",0,0,2e-3,&N002,&N003); 
Resistor R1("R1",10e3,&N001,&N002);
Resistor R2("R2",10e3,&N003,&N000);
Resistor R3("R3",10e3,&N002,&N000);

vector<Node*> v_of_nodes={&N001,&N002,&N003};
vector<Component*> component_list={&VS,&CS,&R1,&R2,&R3};

//current statement
set<Node*> s_of_nodes={&N000,&N001,&N002,&N003};
//voltage statement
set<Component*> s_of_component;

//calculate the value for the current vector(current statement)
double v_current(Node* node){
    double sum=0;
    for(int j=0;j<component_list.size();j++){
        if((component_list[j]->name()[0]=='I'||component_list[j]->name()[0]=='L') && component_list[j]->node_positive()->name==node->name){
            sum += component_list[j]->current();
        }
        if((component_list[j]->name()[0]=='I'||component_list[j]->name()[0]=='L') && component_list[j]->node_negative()->name==node->name){
            sum -= component_list[j]->current();
        }
    }
    return sum;
}

//calculate the value for the conductance matrix(current statement)
double v_conductance(Node* node1, Node* node2){
    double sum=0;
    if(node1->name==node2->name){
        for(int j=0;j<component_list.size();j++){
            if(component_list[j]->name()[0]=='R' && (component_list[j]->node_positive()->name==node1->name||component_list[j]->node_negative()->name==node1->name)){
                sum += component_list[j]->conductance();
            }
        }
    }else{
        for(int j=0;j<component_list.size();j++){
            if(component_list[j]->name()[0]=='R' && component_list[j]->node_positive()->name==node1->name && component_list[j]->node_negative()->name==node2->name){
                sum += component_list[j]->conductance();
            }
            if (component_list[j]->name()[0]=='R' && component_list[j]->node_positive()->name==node2->name && component_list[j]->node_negative()->name==node1->name){
                sum += component_list[j]->conductance();
            }
        }
    }
    return sum;
}

//input the value for the conductance matrix(voltage statement)
vector<double> v_conductance_input(Component* VS){
    vector<double> res;
    for(int h=0;h<v_of_nodes.size();h++){
            res.push_back(0);
        }
    if(VS->node_positive()->name=="N000"){
        res[stoi(VS->node_negative()->name.substr(1,3))-1]=-1;
    }else if(VS->node_negative()->name=="N000"){
        res[stoi(VS->node_positive()->name.substr(1,3))-1]=1;
    }else{
        res[stoi(VS->node_positive()->name.substr(1,3))-1]=1;
        res[stoi(VS->node_negative()->name.substr(1,3))-1]=-1;
    }
    return res;
}

//input the value for the conductance matrix(current statement)
vector<double> v_conductance_input(Node* node){
    vector<double> res;
    if(node->name=="N000"){
        for(int h=0;h<v_of_nodes.size();h++){
            res.push_back(v_conductance(v_of_nodes[h],node));
        }
    }else{
        for(int h=0;h<v_of_nodes.size();h++){
            if (v_of_nodes[h]->name==node->name){
                res.push_back(v_conductance(v_of_nodes[h],node));
            }else{
                res.push_back(-v_conductance(v_of_nodes[h],node));
            }
        }
    }
    return res;
}
 
int main()
{
    //set timestep for component
    for (int i=0;i<component_list.size();i++){
        component_list[i]->set_timestep(timestep);
    }

    //output headers
    cout<<"time";
    for(int i=0;i<v_of_nodes.size();i++){
        cout<<",V("<<v_of_nodes[i]->name<<")";
    }
    for(int i=0;i<component_list.size();i++){
        cout<<",I("<<component_list[i]->name()<<")";
    }
    cout<<endl;

    //determine the value of s_of_nodes and s_of_component
    for(int j=0;j<component_list.size();j++){
        if(component_list[j]->name()[0]=='V'||component_list[j]->name()[0]=='C'){
            s_of_component.insert(component_list[j]);
            s_of_nodes.erase(component_list[j]->node_positive());
            s_of_nodes.erase(component_list[j]->node_negative());
        }
    }

    for (int i=0;timestep*i<=stoptime;i++){
        cout<<i*timestep;
        //initialize three matrix
        MatrixXd  m_conductance(v_of_nodes.size(), v_of_nodes.size());
        VectorXd m_current(v_of_nodes.size());
        VectorXd m_voltage(v_of_nodes.size());

        //value to input row by row
        int row=0;

        //input voltage statement
        set<Component*>::iterator it;
        for (it = s_of_component.begin(); it != s_of_component.end(); ++it) {
            assert(row<=v_of_nodes.size());
            m_current(row)=(*it)->voltage();
            vector<double>tmp=v_conductance_input(*it);
            for (int l=0;l<tmp.size();l++){
                m_conductance(row,l)=tmp[l];
            }
            row++;
        }

        set<Node*>::iterator n_it;
        //input current statement
        for (n_it = s_of_nodes.begin(); n_it != s_of_nodes.end(); ++n_it) {
            assert(row<=v_of_nodes.size());
            m_current(row)=v_current(*n_it);
            vector<double>tmp=v_conductance_input(*n_it);
            for (int l=0;l<tmp.size();l++){
                m_conductance(row,l)=tmp[l];
            }
            row++;
        }

        //calculation for the voltage matrix
        cerr << "Here is the conductance matrix:\n" << m_conductance << endl;
        cerr << "Here is the current vector:\n" << m_current << endl;
        m_voltage = m_conductance.colPivHouseholderQr().solve(m_current);
        cerr << "The voltage vector is:\n" << m_voltage << endl;

        //Input & Output Node Voltages
        for (int k=0;k<v_of_nodes.size();k++){
            cout<<","<<m_voltage(k);
            v_of_nodes[k]->voltage=m_voltage(k);
        }

        for (int k=0;k<component_list.size();k++){
            if(component_list[k]->name()[0]=='V'){
                cout<<","<<component_list[k]->source_current(component_list);
            }else{
                cout<<","<<component_list[k]->current();
            }
            
        }

        cout<<endl;

        //progress to the next time interval
        for (int k=0;k<component_list.size();k++){
            component_list[k]->change_time(); 
        }
    }
}



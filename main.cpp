#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <set>

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

//global variables
vector<Component*> component_list;
vector<Node*> node_list;
vector<string> words;
double timestep;
double stoptime;
string sentence;

//current statement
set<Node*> s_of_nodes;
//voltage statement
set<Component*> s_of_component;

//helper functions
vector<string> separate(string str) { 
   vector<string> tmp;
   string word = ""; 
   for (auto x : str){ 
        if (x == ' '){ 
           tmp.push_back(word); 
           word = ""; 
        }else{ 
           word = word + x; 
        } 
    }  
    tmp.push_back(word);
    return tmp; 
}

Node* nodefinder(string a){
    for(int i=0;i<node_list.size();i++){
        if(node_list[i]->name==a){
            return node_list[i];
        }
    }
    Node *n=new Node{a,0};
    node_list.push_back(n);
    return n;
}

double s_value(string s){
    int end_dig_pos;
    double value;
    double num;
    string multiplier;

    if(isalpha(s.back())){

        for(int i=0;i<s.size();i++){
            if (s[i]=='.'){
                continue;
            }else if(isalpha(s[i])){
                end_dig_pos=i;
                break;
            } 
        }   

        cerr<<s.substr(0,end_dig_pos)<<endl;
        num=stod(s.substr(0,end_dig_pos));
        multiplier=s.substr(end_dig_pos);
        
        if(multiplier=="k"){
            value = num*1000;
        }else if(multiplier=="Meg"){
            value = num*1000000;
        }else if(multiplier=="G"){
            value = num*1000000000;
        }else if(multiplier=="m"){
            value = num*pow(10,-3);
        }else if(multiplier=="u"){
            value = num*pow(10,-6);
        }else if(multiplier=="n"){
            value = num*pow(10,-9);
        }else if(multiplier=="p"){
            value = num*pow(10,-12);
        }else{
            cerr<<"Invalid multiplier";
            exit(1);
        }
        return value;
    }else{
        return stod(s);
    }
}

bool comparename(const Node* node1, const Node* node2){
    if(node1->name=="0"){
        return true;
    }else if (node2->name=="0"){
        return false;
    }else{
        int a=stoi(node1->name.substr(1));
        int b=stoi(node2->name.substr(1));
        return a<b;
    }
}

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
    for(int h=0;h<node_list.size();h++){
            res.push_back(0);
        }
    if(VS->node_positive()->name=="0"){
        res[stoi(VS->node_negative()->name.substr(1,3))-1]=-1;
    }else if(VS->node_negative()->name=="0"){
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
    if(node->name=="0"){
        for(int h=0;h<node_list.size();h++){
            res.push_back(v_conductance(node_list[h],node));
        }
    }else{
        for(int h=0;h<node_list.size();h++){
            if (node_list[h]->name==node->name){
                res.push_back(v_conductance(node_list[h],node));
            }else{
                res.push_back(-v_conductance(node_list[h],node));
            }
        }
    }
    return res;
}

int main(int argc, char *argv[]){

    cerr<<"filename:"<<argv[1]<<endl;

    ifstream infile(argv[1]);

    //input data
    while(getline(infile,sentence)){
        
        words=separate(sentence);
        
        for (size_t i = 0; i < words.size(); i++)
        {
            cerr<<words[i]<<" ";
        }
        
        cerr<<endl;
        
        if(sentence[0]=='*'){
            continue;
        }else if(sentence[0]=='R'||sentence[0]=='L'||sentence[0]=='C'){
            assert(words.size()==4);
            if(sentence[0]=='R'){
                component_list.push_back(new Resistor(words[0], s_value(words[3]), nodefinder(words[1]), nodefinder(words[2])));
            }else if(sentence[0]=='C'){
                component_list.push_back(new Capacitor(words[0], s_value(words[3]), nodefinder(words[1]), nodefinder(words[2])));
            }else{
                component_list.push_back(new Inductor(words[0], s_value(words[3]), nodefinder(words[1]), nodefinder(words[2])));
            }
        }else if(sentence[0]=='V'){
            assert(words.size()==4||words.size()==6);
            if(words.size()==4){
                component_list.push_back(new VoltageSource(words[0], 0, 0, s_value(words[3]), nodefinder(words[1]), nodefinder(words[2])));
            }else{
                double a=s_value(words[4]);
                double f=s_value(words[5].substr(0,words[5].size()-1));
                double o=s_value(words[3].substr(5));
                component_list.push_back(new VoltageSource(words[0], a, f, o, nodefinder(words[1]), nodefinder(words[2])));
            }
        }else if(sentence[0]=='I'){
            assert(words.size()==4||words.size()==6);
            if(words.size()==4){
                component_list.push_back(new CurrentSource(words[0], 0, 0, s_value(words[3]), nodefinder(words[1]), nodefinder(words[2])));
            }else{
                double a=s_value(words[4]);
                double f=s_value(words[5].substr(0,words[5].size()-1));
                double o=s_value(words[3].substr(5));
                component_list.push_back(new CurrentSource(words[0], a, f, o, nodefinder(words[1]), nodefinder(words[2])));
            }
        }else if(sentence[0]=='.'){
            if(words[0]==".end"){
                cerr<<"Input finished..."<<endl;
            }else if(words[0]==".tran"){
                assert(words.size()==5);

                stoptime=s_value(words[2].substr(0,words[2].size()-1));
                timestep=s_value(words[4].substr(0,words[4].size()-1));
            }else{
                cerr<<"Illegal instruction found..."<<endl;
                exit(1);
            }
        }else{
            cerr<<"Illegal instruction found..."<<endl;
            exit(1);
        }
    } 

    sort(node_list.begin(),node_list.end(),comparename);

    infile.close(); 

    for(int i=0;i<node_list.size();i++){
        s_of_nodes.insert(node_list[i]);
    }

    node_list.erase(node_list.begin());

    //set timestep for component
    for (int i=0;i<component_list.size();i++){
        component_list[i]->set_timestep(timestep);
    }

    //output headers
    cout<<"time";
    for(int i=0;i<node_list.size();i++){
        cout<<",V("<<node_list[i]->name<<")";
    }
    for(int i=0;i<component_list.size();i++){
        cout<<",I("<<component_list[i]->name()<<")";
    }
    cout<<endl;

    //determine the value of s_of_nodes and s_of_component
    for(int j=0;j<component_list.size();j++){
        if(component_list[j]->name()[0]=='V'){
            s_of_component.insert(component_list[j]);
            s_of_nodes.erase(component_list[j]->node_positive());
            s_of_nodes.erase(component_list[j]->node_negative());
        }else if(component_list[j]->name()[0]=='C'){
            s_of_component.insert(component_list[j]);
            s_of_nodes.erase(component_list[j]->node_positive());
            s_of_nodes.erase(component_list[j]->node_negative());
        }
    }

    //calculation process
    for (int i=0;timestep*i<=stoptime;i++){

        cout<<i*timestep;

        //initialize three matrix
        MatrixXd  m_conductance(node_list.size(), node_list.size());
        VectorXd m_current(node_list.size());
        VectorXd m_voltage(node_list.size());

        //value to input row by row
        int row=0;

        //input voltage statement
        set<Component*>::iterator it;
        for (it = s_of_component.begin(); it != s_of_component.end(); ++it) {
            assert(row<=node_list.size());
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
            assert(row<=node_list.size());
            if(s_of_component.empty() && (*n_it)->name=="0"){
                continue;
            }
            m_current(row)=v_current(*n_it);
            vector<double>tmp=v_conductance_input(*n_it);
            for (int l=0;l<tmp.size();l++){
                m_conductance(row,l)=tmp[l];
            }
            row++;
        }

        //calculation for the voltage matrix
        //cerr << "Here is the conductance matrix:\n" << m_conductance << endl;
        //cerr << "Here is the current vector:\n" << m_current << endl;
        m_voltage = m_conductance.colPivHouseholderQr().solve(m_current);
        //cerr << "The voltage vector is:\n" << m_voltage << endl;

        //Input & Output Node Voltages
        for (int k=0;k<node_list.size();k++){
            cout<<","<<m_voltage(k);
            node_list[k]->voltage=m_voltage(k);
        }

        for (int k=0;k<component_list.size();k++){
            if(component_list[k]->name()[0]=='V'||component_list[k]->name()[0]=='C'){
                cout<<","<<component_list[k]->current(component_list);
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
 
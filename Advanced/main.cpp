#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <set>

#include "Node.hpp"
#include "../Eigen/Dense"

#include "Components/CurrentSource.hpp"
#include "Components/VoltageSource.hpp"
#include "Components/Resistor.hpp"
#include "Components/Inductor.hpp"
#include "Components/Capacitor.hpp"
#include "Components/Component.hpp"
#include "Components/Diode.hpp"
#include "Components/Transistor.hpp"

using namespace std;
using namespace Eigen;

//global variables
vector<Component*> component_list_real;
vector<Component*> component_list_cal;
vector<Diode*> diode_list;
vector<Transistor*>tran_list;
vector<Node*> node_list;
vector<string> words;
double timestep;
double stoptime;
string sentence;
int diode_count=0;
int loop_count=0;

//current statement
set<Node*> s_of_nodes;
//voltage statement
set<Component*> s_of_component;

//helper functions
bool convergence(){
    bool res=true;
    for (int c=0;c<diode_list.size();c++){
        int no_of_values =diode_list[c]->guess_voltage.size();
        if(no_of_values<2){
            res= false;
        }else{
            res = res&& (abs(diode_list[c]->voltage()-diode_list[c]->guess_voltage[no_of_values-1])<=0.005) && (abs(diode_list[c]->voltage()-diode_list[c]->guess_voltage[no_of_values-2])<=0.005);
        }
    }
    return res;
}

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
    int m_pos;
    string no;
    double value;
    double num;
    string multiplier;

    if(isalpha(s.back())){

        for(int i=0;i<s.size();i++){
            if (s[i]=='.'){
                no.push_back(s[i]);
            }else if(isalpha(s[i])){
                m_pos=i;
                break;
            }else{
                no.push_back(s[i]);
            }
        }

        num=stod(no);
        multiplier=s.substr(m_pos);
        
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
    }else if(node2->name=="0"){
        return false;
    }else if(node1->name[0]=='P'){
        return false;
    }else if(node2->name[0]=='P'){
        return true;
    }else{
        int a=stoi(node1->name.substr(1));
        int b=stoi(node2->name.substr(1));
        return a<b;
    }
}

//calculate the value for the current vector(current statement)
double v_current(Node* node){
    double sum=0;
    for(int j=0;j<component_list_cal.size();j++){
        if((component_list_cal[j]->name()[0]=='I'||component_list_cal[j]->name()[0]=='L') && component_list_cal[j]->node_positive()->name==node->name){
            sum += component_list_cal[j]->current();
        }
        if((component_list_cal[j]->name()[0]=='I'||component_list_cal[j]->name()[0]=='L') && component_list_cal[j]->node_negative()->name==node->name){
            sum -= component_list_cal[j]->current();
        }
    }
    return sum;
}

//calculate the value for the conductance matrix(current statement)
double v_conductance(Node* node1, Node* node2){
    double sum=0;
    if(node1->name==node2->name){
        for(int j=0;j<component_list_cal.size();j++){
            if(component_list_cal[j]->name()[0]=='R' && (component_list_cal[j]->node_positive()->name==node1->name||component_list_cal[j]->node_negative()->name==node1->name)){
                sum += component_list_cal[j]->conductance();
            }
        }
    }else{
        for(int j=0;j<component_list_cal.size();j++){
            if(component_list_cal[j]->name()[0]=='R' && component_list_cal[j]->node_positive()->name==node1->name && component_list_cal[j]->node_negative()->name==node2->name){
                sum += component_list_cal[j]->conductance();
            }
            if (component_list_cal[j]->name()[0]=='R' && component_list_cal[j]->node_positive()->name==node2->name && component_list_cal[j]->node_negative()->name==node1->name){
                sum += component_list_cal[j]->conductance();
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
    if(VS->node_positive()->name[0]=='P'||VS->node_negative()->name[0]=='P'){
        if(VS->node_positive()->name=="0"){
            res[stoi(VS->node_negative()->name.substr(1))-diode_count+node_list.size()]=-1;
        }else if(VS->node_negative()->name=="0"){
            res[stoi(VS->node_positive()->name.substr(1))-diode_count+node_list.size()]=1;
        }else{
            if(VS->node_positive()->name[0]=='P'){
                res[stoi(VS->node_positive()->name.substr(1))-diode_count+node_list.size()]=1;
                res[stoi(VS->node_negative()->name.substr(1,3))-1]=-1;
            }else{
                res[stoi(VS->node_positive()->name.substr(1,3))-1]=1;
                res[stoi(VS->node_negative()->name.substr(1))-diode_count+node_list.size()]=-1;
            } 
        }
    }else{
        if(VS->node_positive()->name=="0"){
            res[stoi(VS->node_negative()->name.substr(1,3))-1]=-1;
        }else if(VS->node_negative()->name=="0"){
            res[stoi(VS->node_positive()->name.substr(1,3))-1]=1;
        }else{
            res[stoi(VS->node_positive()->name.substr(1,3))-1]=1;
            res[stoi(VS->node_negative()->name.substr(1,3))-1]=-1;
        }
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
                Resistor* r=new Resistor(words[0], s_value(words[3]), nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(r);
                component_list_real.push_back(r);
            }else if(sentence[0]=='C'){
                Capacitor* c=new Capacitor(words[0], s_value(words[3]), nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(c);
                component_list_real.push_back(c);
            }else{
                Inductor* l=new Inductor(words[0], s_value(words[3]), nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(l);
                component_list_real.push_back(l);
            }
        }else if(sentence[0]=='V'){
            assert(words.size()==4||words.size()==6);
            if(words.size()==4){
                VoltageSource *vs=new VoltageSource(words[0], 0, 0, s_value(words[3]), nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(vs);
                component_list_real.push_back(vs);
            }else{
                double a=s_value(words[4]);
                double f=s_value(words[5].substr(0,words[5].size()-1));
                double o=s_value(words[3].substr(5));
                VoltageSource *vs=new VoltageSource(words[0], a, f, o, nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(vs);
                component_list_real.push_back(vs);
            }
        }else if(sentence[0]=='I'){
            assert(words.size()==4||words.size()==6);
            if(words.size()==4){
                CurrentSource *cs=new CurrentSource(words[0], 0, 0, s_value(words[3]), nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(cs);
                component_list_real.push_back(cs);
            }else{
                double a=s_value(words[4]);
                double f=s_value(words[5].substr(0,words[5].size()-1));
                double o=s_value(words[3].substr(5));
                CurrentSource *cs=new CurrentSource(words[0], a, f, o, nodefinder(words[1]), nodefinder(words[2]));
                component_list_cal.push_back(cs);
                component_list_real.push_back(cs);
            }
        }else if (sentence[0]=='D'){
            assert(words.size()==4);
            string nod="P"+to_string(diode_count);
            Resistor *rd=new Resistor("R"+words[0], 0, nodefinder(words[1]), nodefinder(nod));
            VoltageSource *vs=new VoltageSource("V"+words[0],0,0,0,nodefinder(nod),nodefinder(words[2]));
            Diode *d =new Diode(words[0], nodefinder(words[1]), nodefinder(words[2]),rd,vs);
            component_list_real.push_back(d);
            diode_list.push_back(d);
            component_list_cal.push_back(rd);
            component_list_cal.push_back(vs);
            diode_count++;
        }else if(sentence[0]=='Q'){
            assert(words.size()==5);
            string nod="P"+to_string(diode_count);
            Resistor *rd;
            VoltageSource *vs;
            CurrentSource *cs;
            Diode *d;
            Transistor *q;
            if(words[4]=="NPN"){
                rd=new Resistor("R"+words[0], 0, nodefinder(words[2]), nodefinder(nod));
                vs=new VoltageSource("V"+words[0],0,0,0,nodefinder(nod),nodefinder(words[3]));
                cs=new CurrentSource("I"+words[0],0,0,0,nodefinder(words[1]),nodefinder(words[3]));
                d =new Diode("D"+words[0], nodefinder(words[2]), nodefinder(words[3]),rd,vs);
                q=new Transistor(words[0],d,cs);
            }else if(words[4]=="PNP"){
                rd=new Resistor("R"+words[0], 0, nodefinder(words[3]), nodefinder(nod));
                vs=new VoltageSource("V"+words[0],0,0,0,nodefinder(nod),nodefinder(words[2]));
                cs=new CurrentSource("I"+words[0],0,0,0,nodefinder(words[3]),nodefinder(words[1]));
                d =new Diode("D"+words[0], nodefinder(words[3]), nodefinder(words[2]),rd,vs);
                q=new Transistor(words[0],d,cs);
            }else{
                cerr<<"Invalid BJT type"<<endl;
                exit(1);
            }
            component_list_real.push_back(q);
            tran_list.push_back(q);
            diode_list.push_back(d);
            component_list_cal.push_back(rd);
            component_list_cal.push_back(vs);
            component_list_cal.push_back(cs);
            diode_count++;
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
    for (int i=0;i<component_list_cal.size();i++){
        component_list_cal[i]->set_timestep(timestep);
    }

    //output headers
    cout<<"time";
    for(int i=0;i<node_list.size();i++){
        if(node_list[i]->name[0]!='P'){
            cout<<",V("<<node_list[i]->name<<")";
        }
    }
    for(int i=0;i<component_list_real.size();i++){
        if(component_list_real[i]->name()[0]!='Q'){
            cout<<",I("+component_list_real[i]->name()+")";
        }else{
            cout<<",I("+component_list_real[i]->name()+"c"+")";
            cout<<",I("+component_list_real[i]->name()+"b"+")";
            cout<<",I("+component_list_real[i]->name()+"e"+")";
        }
    }
    cout<<endl;

    //determine the value of s_of_nodes and s_of_component
    for(int j=0;j<component_list_cal.size();j++){
        if(component_list_cal[j]->name()[0]=='V'){
            s_of_component.insert(component_list_cal[j]);
            s_of_nodes.erase(component_list_cal[j]->node_positive());
            s_of_nodes.erase(component_list_cal[j]->node_negative());
        }else if(component_list_cal[j]->name()[0]=='C'){
            s_of_component.insert(component_list_cal[j]);
            s_of_nodes.erase(component_list_cal[j]->node_positive());
            s_of_nodes.erase(component_list_cal[j]->node_negative());
        }
    }

    //calculation process
    for (int i=0;timestep*i<=1e-6;i++){

        //initialize three matrix
        MatrixXd m_conductance(node_list.size(), node_list.size());
        VectorXd m_current(node_list.size());
        VectorXd m_voltage(node_list.size());

        //value to input row by row
        int row=0;

        //change thevenin resistance and thevenin voltage
        for(int d=0;d<diode_list.size();d++){
            cerr<<"Current Guess Voltage:"<<diode_list[d]->guess_voltage.back()<<endl;
            diode_list[d]->th_resistance();
            diode_list[d]->th_voltage();
        }

        //change current dependent current source
        for(int d=0;d<tran_list.size();d++){
            tran_list[d]->current_source();
        }

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
        cerr << "Here is the conductance matrix:\n" << m_conductance << endl;
        cerr << "Here is the current vector:\n" << m_current << endl;
        m_voltage = m_conductance.colPivHouseholderQr().solve(m_current);
        cerr << "The voltage vector is:\n" << m_voltage << endl;

        for (int k=0;k<node_list.size();k++){
            node_list[k]->voltage=m_voltage(k);
        }

        if(loop_count>20){
            cerr<<"Loop over"+to_string(loop_count)+"times"<<endl;
            exit(1);
        }

        if(diode_count==0 ||(diode_count>0 && convergence())){
            
            cerr<<"Successful Guess..."<<endl;
            cout<<i*timestep;
            
            //Input & Output Node Voltages
            for (int k=0;k<node_list.size();k++){
                if(node_list[k]->name[0]!='P'){
                    cout<<","<<m_voltage(k);
                }
            }

            for (int k=0;k<component_list_real.size();k++){
                if(component_list_real[k]->name()[0]=='V'||component_list_real[k]->name()[0]=='C'){
                    cout<<","<<component_list_real[k]->current(component_list_cal);
                }else if (component_list_real[k]->name()[0]=='Q'){
                    vector<double> tmp=component_list_real[k]->bjt_current();
                    for (int m=0;m<tmp.size();m++){
                        cout<<","<<tmp[m];
                    }
                }else{
                    cout<<","<<component_list_real[k]->current();
                }
                
            }
            
            cout<<endl;

            //progress to the next time interval
            for (int k=0;k<component_list_cal.size();k++){
                component_list_cal[k]->change_time(); 
            }

            for (int c=0;c<diode_list.size();c++){
                diode_list[c]->guess_voltage.clear();
                diode_list[c]->guess_voltage.push_back(diode_list[c]->voltage());
            }
            loop_count=0;
        }else{ 
            i--;
            for (int c=0;c<diode_list.size();c++){
                cerr<<"Next Guess Voltage:"<<diode_list[c]->voltage()<<endl;
                diode_list[c]->guess_voltage.push_back(diode_list[c]->voltage());
            }  
            loop_count++;
        }
    }
}
 
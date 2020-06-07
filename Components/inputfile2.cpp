#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "Component.hpp"
#include "Resistor.hpp"
#include "Capacitor.hpp"
#include "Inductor.hpp"
#include "VoltageSource.hpp"
#include "CurrentSource.hpp"
#include "multiplier.hpp"

using namespace std;


Node* nodefinder(string a){
    vector<Node*> nodes;
    string a;
    for(int i=0;i<nodes.size();i++){
        if(nodes[i]->name==a){
            return nodes[i];
        }
    }
}



int main()
{
    string s;
    ifstream infile("Test1.1.txt");

    vector<Component*> component_list;
    vector<Node*> node_list;

    while(getline(infile,s)){

        size_t pos = s.find('N');
        for(int i=0;i<node_list.size();i++){
            if(node_list[i]->name!=s.substr(pos,4)){
                node_list.push_back(new Node{s.substr(pos,4), 0});
            }
        }

        size_t pos2 = pos+5;
        if(s[pos2]=='0'){
            for(int j=0;j<node_list.size();j++){
                if(node_list[j]->name!="0"){
                    node_list.push_back(new Node{"0", 0});
                }
            }
        }else if(s[pos2]=='N'){
            for(int k=0;k<node_list.size();k++){
                if(node_list[k]->name!=s.substr(pos2,4)){
                    node_list.push_back(new Node{s.substr(pos2,4), 0});
                }
            }
        }
    }




    while(getline(infile,s)){

        if(s[0]=='R'){

            size_t rpos = s.find('N');

            size_t rpos2 = rpos+5;

            if(s[rpos2]=='0'){
               
               double rnum = stod(s.substr(rpos2+2));
               
               double res = multiplier(rnum);
               
               component_list.push_back(new Resistor(s.substr(0,2), res, nodefinder(s.substr(rpos,4)), nodefinder(s.substr(rpos2,1))));



           }else if(s[rpos2]=='N'){
               
               double rnum = stod(s.substr(rpos2+5));

               double res = multiplier(rnum);

               component_list.push_back(new Resistor(s.substr(0,2), res, nodefinder(s.substr(rpos,4)), nodefinder(s.substr(rpos2,4))));

               
               

               };

           }  
            

        }
        
        infile.close();
    }


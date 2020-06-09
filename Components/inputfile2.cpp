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

            size_t pos = s.find('N');

            size_t pos2 = pos+5;

            if(s[pos2]=='0'){
               
               double num = stod(s.substr(pos2+2));
               
               double res = multiplier(num);
               
               component_list.push_back(new Resistor(s.substr(0,2), res, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));



           }else if(s[pos2]=='N'){
               
               double num = stod(s.substr(pos2+5));

               double res = multiplier(num);

               component_list.push_back(new Resistor(s.substr(0,2), res, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));

               
               

               };

           }else if(s[0]=='L'){

            size_t pos = s.find('N');

            size_t pos2 = pos+5;

            if(s[pos2]=='0'){
               
               double num = stod(s.substr(pos2+2));
               
               double ind = multiplier(num);
               
               component_list.push_back(new Inductor(s.substr(0,2), ind, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));



           }else if(s[pos2]=='N'){
               
               double num = stod(s.substr(pos2+5));

               double ind = multiplier(num);

               component_list.push_back(new Inductor(s.substr(0,2), ind, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));

               
               

               };
           }else if(s[0]=='C'){

            size_t pos = s.find('N');

            size_t pos2 = pos+5;

            if(s[pos2]=='0'){
               
               double num = stod(s.substr(pos2+2));
               
               double cap = multiplier(num);
               
               component_list.push_back(new Capacitor(s.substr(0,2), cap, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));



           }else if(s[pos2]=='N'){
               
               double num = stod(s.substr(pos2+5));

               double cap = multiplier(num);

               component_list.push_back(new Capacitor(s.substr(0,2), cap, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));

               
               

               };
           }else if(s[0]=='V'){

            size_t pos = s.find('N');

            size_t pos2 = pos+5;

            if(s[pos2]=='0'){

               if(s[pos2+2]=='S'){
                   size_t datapos = s.find('(');
                   string data = s.substr(datapos);
                
                       size_t blank = data.find(' ');
                       int digit = blank - datapos;
                       double a = multiplier(stod(data.substr(datapos+1,digit)));

                       string oandf = data.substr(blank+1);
                       size_t blank2 = oandf.find(' ');
                       int digit2 = blank2 - (blank+1);
                       double o = multiplier(stod(oandf.substr(blank+1,digit2)));

                       size_t endpos = oandf.find(')');
                       int digit3 = endpos - (blank2+1);
                       double f = multiplier(stod(oandf.substr(blank2+1,digit3)));

                       component_list.push_back(new VoltageSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));
                   
               }else{

                   double num = stod(s.substr(pos2+2));
                   double o = multiplier(num);
                   double a = 0;
                   double f = 0;
                   component_list.push_back(new VoltageSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));

               };

           }else if(s[pos2]=='N'){
               
               if(s[pos2+5]=='S'){
                   size_t datapos = s.find('(');
                   string data = s.substr(datapos);
                
                       size_t blank = data.find(' ');
                       int digit = blank - datapos;
                       double a = multiplier(stod(data.substr(datapos+1,digit)));

                       string oandf = data.substr(blank+1);
                       size_t blank2 = oandf.find(' ');
                       int digit2 = blank2 - (blank+1);
                       double o = multiplier(stod(oandf.substr(blank+1,digit2)));

                       size_t endpos = oandf.find(')');
                       int digit3 = endpos - (blank2+1);
                       double f = multiplier(stod(oandf.substr(blank2+1,digit3)));

                       component_list.push_back(new VoltageSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));
                   
               }else{

                   double num = stod(s.substr(pos2+5));
                   double o = multiplier(num);
                   double a = 0;
                   double f = 0;
                   component_list.push_back(new VoltageSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));

                 }

               };

           }else if(s[0]=='I'){

            size_t pos = s.find('N');

            size_t pos2 = pos+5;

            if(s[pos2]=='0'){

               if(s[pos2+2]=='S'){
                   size_t datapos = s.find('(');
                   string data = s.substr(datapos);
                
                       size_t blank = data.find(' ');
                       int digit = blank - datapos;
                       double a = multiplier(stod(data.substr(datapos+1,digit)));

                       string oandf = data.substr(blank+1);
                       size_t blank2 = oandf.find(' ');
                       int digit2 = blank2 - (blank+1);
                       double o = multiplier(stod(oandf.substr(blank+1,digit2)));

                       size_t endpos = oandf.find(')');
                       int digit3 = endpos - (blank2+1);
                       double f = multiplier(stod(oandf.substr(blank2+1,digit3)));

                       component_list.push_back(new CurrentSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));
                   
               }else{

                   double num = stod(s.substr(pos2+2));
                   double o = multiplier(num);
                   double a = 0;
                   double f = 0;
                   component_list.push_back(new CurrentSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,1))));

               };

           }else if(s[pos2]=='N'){
               
               if(s[pos2+5]=='S'){
                   size_t datapos = s.find('(');
                   string data = s.substr(datapos);
                
                       size_t blank = data.find(' ');
                       int digit = blank - datapos;
                       double a = multiplier(stod(data.substr(datapos+1,digit)));

                       string oandf = data.substr(blank+1);
                       size_t blank2 = oandf.find(' ');
                       int digit2 = blank2 - (blank+1);
                       double o = multiplier(stod(oandf.substr(blank+1,digit2)));

                       size_t endpos = oandf.find(')');
                       int digit3 = endpos - (blank2+1);
                       double f = multiplier(stod(oandf.substr(blank2+1,digit3)));

                       component_list.push_back(new CurrentSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));
                   
               }else{

                   double num = stod(s.substr(pos2+5));
                   double o = multiplier(num);
                   double a = 0;
                   double f = 0;
                   component_list.push_back(new CurrentSource(s.substr(0,2), a, o, f, nodefinder(s.substr(pos,4)), nodefinder(s.substr(pos2,4))));

                 }

               };
               
           }  
            

        }
        
        infile.close();
    }


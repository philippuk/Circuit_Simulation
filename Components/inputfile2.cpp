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


int main()
{
    string s;
    ifstream infile("Test1.1.txt");

    vector<Component*> component_list;
    vector<Node*> node_list;

    while(getline(infile,s)){

        if(s[0]=='R'){

            size_t rpos = s.find('N');
            
            Node *N = new Node{s.substr(rpos,4), 0};
            
            node_list.push_back(new Node{s.substr(rpos,4), 0});

            size_t rpos2 = rpos+5;

            if(s[rpos2]=='0'){

               Node *N2 = new Node{0, 0};
               
               double rnum = stod(s.substr(rpos2+2));
               
               double res = multiplier(rnum);
               
               component_list.push_back(new Resistor(s.substr(0,2), res, new Node{s.substr(rpos,4), 0}, new Node{0, 0}));

               node_list.push_back(new Node{0, 0});



           }else if(s[rpos2]=='N'){
               
               Node *N2 = new Node{s.substr(rpos2,4), 0};
               
               
               double rnum = stod(s.substr(rpos2+5));
               
               

               };

           }  
            

        }
        cout << component_list[0];
        infile.close();
    }


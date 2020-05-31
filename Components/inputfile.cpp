#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "Component.hpp"
//#include "Node.hpp"
//#include "Resistor.hpp"

using namespace std;

int main()
{

    string s;
    ifstream infile("Test1.1.txt");   
    vector<int> Rnode;
    vector<double> vr;
    vector<int> Rnode2;


    while (getline(infile, s)){
       if(s[0]=='R'){
           
           size_t pos = s.find('N');
           int nodepos = stoi(s.substr(pos+1,3));               
           Rnode.push_back(nodepos);
           
           
           int nodepos2;
           size_t pos2 = pos+5;
           
           if(s[pos2]=='0'){

               nodepos2 = 0;
               Rnode2.push_back(nodepos2);
               vr.push_back(stod(s.substr(pos2+2)));

           }else if(s[pos2]=='N'){
               
               nodepos2 = stoi(s.substr(pos2+1,3));
               Rnode2.push_back(nodepos2);
               vr.push_back(stod(s.substr(pos2+5)));

           }  
           
              
       }/*else if(s[0]=='V'){

       }else if(s[0]=='I'){

       }*/


    }
    
    infile.close();
}


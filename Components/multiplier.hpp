#ifndef multiplier_hpp
#define multiplier_hpp

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

 double multiplier(double num)
    {
        string s;
        double value;
        if(s[s.size()-1]='k'){

            value = num*1000;
            
        }else if(s[s.size()-1]='g'){

            value = num*1000000;

        }else if(s[s.size()-1]='G'){

            value = num*1000000000;

        }else if(s[s.size()-1]='m'){

            value = num*pow(10,-3);

        }else if(s[s.size()-1]='u'){

            value = num*pow(10,-6);

        }else if(s[s.size()-1]='n'){

            value = num*pow(10,-9);

        }else if(s[s.size()-1]='p'){

            value = num*pow(10,-12);

        }
        return value;
    }

    #endif
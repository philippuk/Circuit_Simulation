#ifndef Component_hpp
#define Component_hpp

#include <string>
#include <iostream>
#include "../Node.hpp"

using namespace std;

class Component
{
protected:
    double com_time=0;
    double com_timestep;
    string com_name;
    Node node_pos;
    Node node_neg;
public:
    void set_time(double time){
        com_time=time;
    }
    
    void set_timestep(double timestep){
        com_timestep=timestep;
    }
    
    void change_time(bool sign){
        if (sign){
            com_time += com_timestep;
        }else{
            com_time -= com_timestep;
        }
    }

    Node node_positive(){
        return node_pos;
    }

    Node node_negative(){
        return node_neg;
    }
    
    double time(){
        return com_time;
    }
    
    double timestep(){
        return com_timestep;
    }
    
    string name(){
        return com_name;
    }

    virtual double current()
    {
        cerr<<"Base case called"<<endl;
        return 0;
    };

    virtual double voltage()
    {
        cerr<<"Base case called"<<endl;
        return 0;
    };

    virtual double conductance()
    {
        cerr<<"Base case called"<<endl;
        return 0;
    };
};

#endif /* Component_hpp */

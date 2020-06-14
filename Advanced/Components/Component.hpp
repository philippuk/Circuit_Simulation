#ifndef Component_hpp
#define Component_hpp

#include <string>
#include <iostream>
#include <vector>
#include "../Node.hpp"

using namespace std;

class Component
{
protected:
    double com_time=0;
    double com_timestep;
    string com_name;
    Node* node_pos;
    Node* node_neg;
public:
    void set_timestep(double timestep){
        com_timestep=timestep;
    }

    Node* node_positive(){
        return node_pos;
    }

    Node* node_negative(){
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

    virtual void change_time()
    {
        com_time += com_timestep;
    }

    virtual double current()
    {
        cerr<<"Current Base case called"<<endl;
        return 0;
    };

    virtual vector<double> access()
    {
        cerr<<"dont called this"<<endl;
        return {};
    };

    virtual double voltage()
    {
        cerr<<"Voltage Base case called"<<endl;
        return 0;
    };

    virtual double conductance()
    {
        cerr<<"Conductance Base case called"<<endl;
        return 0;
    };

    virtual double current(const vector<Component *> &list)
    {
        cerr<<"Source Current Base case called"<<endl;
        return 0;
    }

    virtual vector<double> bjt_current()
    {
        cerr<<"BJT Current Base case called"<<endl;
        return {};
    }
};

#endif /* Component_hpp */

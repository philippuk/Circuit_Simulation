#include <iostream>
#include <vector>

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
 
int main()
{
    //input part
    double stoptime=10e-3;
    double timestep=1e-6;
    int no_node=3;
    vector<Component> component_list;
    
    Node N000={0,0};
    Node N001={1,0};
    Node N002={2,0};
    Node N003={3,0};
    
    VoltageSource VS("V1",0,0,1,N001,N000);
    Resistor R1("R1",10e3,N001,N002);
    CurrentSource CS("I1",0,0,2e-3,N002,N003);

    component_list.push_back(VS);
    component_list.push_back(R1);
    component_list.push_back(CS);
    
    Resistor R2("R2",10e3,N003,N000);
    component_list.push_back(R2);

    Resistor R3("R3",10e3,N002,N000);
    component_list.push_back(R3);

    for (int i=0;i<component_list.size();i++){
        component_list[i].set_timestep(timestep);
    }
    
    //calculation part
    for (int i=0;timestep*i<=stoptime;i++){
        Matrix<double, 3, 3> Conductance;
        Matrix<double, 3, 1> Current;
        Conductance << 1,0,0, -R1.conductance(),R1.conductance()+R3.conductance(),0,0,0,R2.conductance();
        Current << VS.voltage(), -CS.current(), CS.current();
        cout << "Here is the conductance matrix:\n" << Conductance << endl;
        cout << "Here is the current vector:\n" << Current << endl;
        Matrix<double,3,1> Voltage = Conductance.colPivHouseholderQr().solve(Current);
        cout << "The voltage vector is:\n" << Voltage << endl;
        component_list[i].change_time(true);
    }
}

#include <iostream>
#include <vector>

#include "CurrentSource.hpp"
#include "VoltageSource.hpp"
#include "Reisistor.hpp"
#include "Node.hpp"
#include "Inductor.hpp"
#include "Capacitor.hpp"
#include "Eigen/Dense"
 
using namespace std;
using namespace Eigen;
 
int main()
{
    //input part
    double stoptime=10e-3;
    double timestep=1e-6;
    int no_node=3;
    
    Node N000={0,0};
    Node N001={1,0};
    Node N002={2,0};
    Node N003={3,0};
    
    VoltageSource V1(0,0,1,N001,N000);
    Resistor R1(10e3,N001,N002);
    CurrentSource I1(0,0,2e-3,N002,N003);
    Resistor R2 (10e3,N003,N000);
    Resistor R3 (10e3,N002,N000);
    
    vector<Componenet> component_list.push_back(V1,R1,I1,R2,R3);
    
    for (int i=0;i<component_list.size();i++){
        component_list[i].set_timestep(timestep);
    }
    
    //calculation part
    for (int i=0;timestep*i<=stoptime;i++){
        Matrix<double, no_node, no_node>  Conductance;
        Matrix<double, no_node,1> Current;
        A << 1,0,0,  R1.conductance(),5,6,  7,8,10;
        b << V1.voltage(), 0, I1.current();
        cout << "Here is the matrix A:\n" << A << endl;
        cout << "Here is the vector b:\n" << b << endl;
        Vector3f x = A.colPivHouseholderQr().solve(b);
        cout << "The solution is:\n" << x << endl;
        component_list[i].set_timestep(timestep);
    }
}

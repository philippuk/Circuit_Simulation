#ifndef Transistor_hpp
#define Transistor_hpp

#include "Component.hpp"
#include "Component.hpp"
#include "Diode.hpp"
#include "CurrentSource.hpp"


#include <cmath>
#include <vector>

using namespace std;

class Transistor:public Component
{
protected:
    Diode *d;
    CurrentSource *Id;
public:
    Transistor(string name, Diode *diode, CurrentSource* C)
    {
        com_name=name;
        d=diode;
        Id=C;
    }
    
    vector<double> bjt_current(){
        double ib=d->current();
        double ic=Id->current();
        double ie=ib+ic;
        return {ic,ib,ie};
    }

    void current_source(){
        Id->Offset=d->current()*100;
    }

    vector<double> access(){
        return {};
    }

};



#endif /* Transistor_hpp */

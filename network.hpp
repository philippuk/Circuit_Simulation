#ifndef network_hpp
#define network_hpp

#include <iostream>
#include <cmath>
#include <string>

using namespace std;


double sine(double offset, double A, double f)//define a new sine function
{
    double t;
    return {A*sin(2*3.1415926535*f*t) + offset};
}


struct network                       //struct for R, L, C, V
{
    string type;
    string nodeA;
    string nodeB;
    double value;

};


network R(string nodeA, string nodeB, double v)
{
    return {"R", nodeA, nodeB, v};
};

network L(string nodeA, string nodeB, double v)
{
    return {"L", nodeA, nodeB, v};
};

network C(string nodeA, string nodeB, double v)
{
    return {"C", nodeA, nodeB, v};
};

network ACV(string nodeA, string nodeB, double d)
{
    double o, a, f;
    d = sine(o,a,f);
    return {"ACV", nodeA, nodeB, d};
};

network DCV(string nodeA, string nodeB, double v)
{
    return {"DCV", nodeA, nodeB, v};
};

network I(string nodeA, string nodeB, double v)
{
    return {"I", nodeA, nodeB, v};
};

#endif
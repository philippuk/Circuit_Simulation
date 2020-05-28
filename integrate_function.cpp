#include <iostream>
#include <functional> // For function objects
#include <cmath> // sin()

using namespace std;

// Definite integral
//  This function takes the integral of functions that returns double and only has one double parameter.
// precision = dx
// Smaller the precision means larger amount of loop iterations thus longer execution time.
double integrate(function<double(double)> Func, double Begin, double End, double precision = 0.001)
{
    double sum = 0.0;
    
    for ( double d = Begin ; d < End ; d+= precision )
    {
       sum += Func(d) * precision;
    }
    
    return sum;
}

double func(double x)
{
   return sin(x);
}

int main() {
    // integration example
    // we will use lambda functions in these examples but you should be able to use normal functions as well. In that case, giving the name of the function is enough, just like the function pointers in C/C++ ( i.e.: sin, cos... ).
    cout << "Integrating x^2 from 1 to 10: " << integrate( [](double x) {return x*x;}, 1, 10 ) << endl;
    // Normal function example
    cout << "Integrating sin(x) from 0 to pi: " << integrate( func, 0, 3.14159 ) << endl;
    return 0;
}

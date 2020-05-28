#include <iostream>
#include "Eigen/Dense"
 
using namespace std;
using namespace Eigen;
 
int main()
{
    int no_node;
   Matrix<double, no_node, no_node>  Conductance;
   Matrix<double, no_node,1> Current;
   A << 1,2,3,  4,5,6,  7,8,10;
   b << 3, 3, 4;
   cout << "Here is the matrix A:\n" << A << endl;
   cout << "Here is the vector b:\n" << b << endl;
   Vector3f x = A.colPivHouseholderQr().solve(b);
   cout << "The solution is:\n" << x << endl;
}

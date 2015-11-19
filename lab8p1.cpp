#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdexcept>

using namespace std;

struct MultiplyBy 
{ 
     double f; 
     MultiplyBy(double factor) : f(factor) {} 
     void operator()(double& val) const { val *= f; } 
}; 

int main(){
// компилировать с опцией -std=c++11
std::vector<double> vf = {1.,2.,3., 4., 5., 6.};
std::for_each( vf.begin(), vf.end(),MultiplyBy(3.1415926) );
std::ostream_iterator<double> itOut(std::cout,"\n");
std::copy(vf.begin(), vf.end(), itOut);
std::cout << "\n";

}


//--------------------------Includeok helye--------------------------//
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "vector.hpp"
#include "odeint.hpp"
#include <math.h>       /* exp */

using namespace cpl;
using namespace std;
//--------------------------Valtozok--------------------------//
double r;
double x0;
double x_aktualis;
int euler_vagy_rk4;
char* fileName;
double y;
double yaktualis;
//--------------------------Fuggveny deklaracio--------------------------//
Vector f(const Vector& x);
void Euler( cpl::Vector& x, double& tau, cpl::Vector derivs(const cpl::Vector&)){
      x += tau * derivs(x);
 }

//--------------------------Main--------------------------//
int main(int argc, char **argv)
{
//Futtatas: ./popdin.bin e/r x0 valami.data r



    if(*argv[1] == 'e') euler_vagy_rk4 = 1;
    if(*argv[1] == 'r') euler_vagy_rk4 = 2;
    x0 = atof(argv[2]);
    fileName = argv[3];
    r = atof(argv[4]);
   
    ofstream file(fileName);
   
    double dt = 0.05;
    double accuracy = 1e-6;
    double t = 0;
    double tMax = 10;
    Vector x(2);

    x[0] = t;
    x[1] = x0;
    file << t << '\t' << x0 << '\n';
    
    if(euler_vagy_rk4 == 1)
    {
        while(t < tMax)
        {
            Euler(x,dt,f);
            t += dt;
            x_aktualis = x[1];
            file << t << '\t' << x_aktualis << '\n';   
        }
    }
    if(euler_vagy_rk4 == 2)
    {
    while(t < tMax)
    {
        adaptiveRK4Step(x,dt,accuracy,f);
        t = x[0];
        x_aktualis = x[1];
        file << t << '\t' << x_aktualis << '\n';
        }
    }

    cout << "Adatok: "<< fileName << endl;
    file.close();
    return 0;
}
    //--------------------------Fuggveny definialas--------------------------//
    Vector f(const Vector& x)
    {
        double x_aktualis = x[1];
        Vector f(2);
        f[0] = 1;
        f[1] = r * x_aktualis * ( 1 - x_aktualis );
        return f;
    }


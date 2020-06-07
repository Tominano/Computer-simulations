#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include "vector.hpp"
#include "odeint.hpp"
using namespace cpl;

double r;
double n; //letszam
double k; //kapacitas

    void Euler( cpl::Vector& x, double& tau, cpl::Vector derivs(const cpl::Vector&)){
      x += tau * derivs(x);
    }
cpl::Vector f(const cpl::Vector& x) 
{
    double t = x[0];
    double xnk = x[1];
    Vector f(2);
    f[0] = 1;
    f[1] = r * xnk * ( 1 - xnk);
    return f;
}


int main(int argc, char* argv[]) 
{
    if(argc < 7)
    {
        cerr << "Usage: ./popdin <method (0 - Rungekutta, else - Euler)> <szuletes-halalozas> <letszam> <kapacitas> <dt> <accuracy>" << endl;
        return 0;
    }
    bool eu = true;
    if(atof(argv[1]) == 0) eu = false;

    r = atof(argv[2]);
    n = atof(argv[3]);
    k = atof(argv[4]);
    double xnk = n/k;
    double dt = atof(argv[5]);
    double accuracy = atof(argv[6]);
    double t = 0;
    double t_max = 10;

    stringstream fileName(""); fileName << "output/"<< eu << "-" << r << "-" << n << "-" << k << ".data";
    ofstream file(fileName.str().c_str());

    cpl::Vector x0(2);
    x0[0] = 0;  
    x0[1] = xnk; 

    //+++++++++++++++++++++++++++++++++++Euler+++++++++++++++++++++++++++++++++++
    cpl::Vector x = x0;
    if(eu == true)
    {
        cout << "\n Integrating with Euler" << endl;
        do 
        {
            for (int i = 0; i < 2; i++)
                file << x[i] << '\t';
            file << '\n';

            Euler(x, dt, f);

        } while (x[0] < t_max);
        file.close();
    }
    //+++++++++++++++++++++++++++++++++++Adaptive RK4+++++++++++++++++++++++++++++++++++
    else
    {
        stringstream stepsizefileName(""); stepsizefileName << "output/stepsizefile-" << r << "-" << n << "-" << k << ".data";
        ofstream stepsizeFile(stepsizefileName.str().c_str());
        x = x0;
        int steps = 0;
        double dt_max = 100, dt_min = 0;
        cout << "\n Integrating with adaptive step size" << endl;
        do 
        {
            for (int i = 0; i < 2; i++)
                file << x[i] << '\t';
            file << '\n';

            double t_save = x[0];
            adaptiveRK4Step(x, dt, accuracy, f);
            double step_size = x[0] - t_save;
            ++steps;
            stepsizeFile << x[1] << '\t' << step_size << '\n';


            if (step_size < dt_min) dt_min = step_size;
            if (step_size > dt_max) dt_max = step_size;
        } while (x[0] < t_max);
        cout << " number of adaptive steps = " << steps << endl;
        cout << " step size: min = " << dt_min << "  max = " << dt_max << endl;
        file.close();
        stepsizeFile.close();
    }
}


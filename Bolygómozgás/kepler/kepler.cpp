#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

#include "vector.hpp"
#include "odeint.hpp"
using namespace cpl;

const double pi = 4 * atan(1.0);
//const double GmPlusM = 4 * pi * pi; // Earth
const double GmPlusM = 39.46205; //Mercury
double alpha = 1.1e-6;

bool switch_t_with_y = false;    //  to interpolate to y = 0

//  Derivative vector for Newton's law of gravitation
// Enter aphelion distance in AU, and eccentricity: 1.0167 0.016710219
// Enter number of periods, step size, and adaptive accuracy: 100 0.001 1e-10
Vector f(const Vector& x) {
    double t = x[0], r_x = x[1], r_y = x[2], v_x = x[3], v_y = x[4];
    double rSquared = r_x*r_x + r_y*r_y;
    double rCubed = rSquared * sqrt(rSquared);
    Vector f(5);
    f[0] = 1;
    f[1] = v_x;
    f[2] = v_y;
    f[3] = - GmPlusM * r_x / rCubed* (1 + alpha/rSquared);
    f[4] = - GmPlusM * r_y / rCubed* (1 + alpha/rSquared);
    if (switch_t_with_y) {
        //  use y as independent variable
        for (int i = 0; i < 5; i++)
            f[i] /= v_y;
    }
    return f;
}

//  Change independent variable from t to y and step back to y = 0
void interpolate_crossing(Vector x, int& crossing) {
    ++crossing;
    switch_t_with_y = true;
    RK4Step(x, -x[2], f);
    cout << " crossing " << crossing << "\t t = " << x[0]
         << "\t x = " << x[1] << endl;
    switch_t_with_y = false;
}

int main() {
   //double r_ap = 1.0167; //Earth
    double r_ap = 0.46669835; //Mercury
   // double eccentricity = 0.016710219; //Earth 
    double eccentricity = 0.20563069; //Mercury
    double periods = 5000;
    double dt = 0.001;
    double accuracy = 1e-10;

    double a, T, v0;
    a = r_ap / (1 + eccentricity);
    T = pow(a, 1.5);
    v0 = sqrt(GmPlusM * (2 / r_ap - 1 / a));
    Vector x0(5);
    x0[0] = 0;  x0[1] = r_ap;  x0[2] = 0;  x0[3] = 0;  x0[4] = v0;

//+++++++++++++++++++++++++++++++++++Fixed RK4+++++++++++++++++++++++++++++++++++
    ofstream dataFile("fixed.data");
    Vector x = x0;
    int steps = 0, crossing = 0;
    cout << "\n Integrating with fixed step size" << endl;
    do {
        for (int i = 0; i < 3; i++)
            dataFile << x[i] << '\t';
        dataFile << '\n';

        double y = x[2];
        RK4Step(x, dt, f);
        ++steps;

        if (y * x[2] < 0)
            interpolate_crossing(x, crossing);
    } while (x[0] < periods * T);
    cout << " number of fixed size steps = " << steps << endl;
    cout << " data in file fixed.data" << endl;
    dataFile.close();
//+++++++++++++++++++++++++++++++++++Adaptive RK4+++++++++++++++++++++++++++++++++++
    dataFile.open("adaptive.data");
    ofstream stepsizeFile("earth_stepsize.data");
    x = x0;
    steps = crossing = 0;
    double dt_max = 0, dt_min = 100;
    cout << "\n Integrating with adaptive step size" << endl;
    do {
        for (int i = 0; i < 3; i++)
            dataFile << x[i] << '\t';
        dataFile << '\n';

        double t_save = x[0];
        double y = x[2];
        adaptiveRK4Step(x, dt, accuracy, f);
        double step_size = x[0] - t_save;
        ++steps;
        stepsizeFile << x[1] << '\t' << step_size << '\n';


        if (step_size < dt_min) dt_min = step_size;
        if (step_size > dt_max) dt_max = step_size;
        if (y * x[2] < 0)
            interpolate_crossing(x, crossing);
    } while (x[0] < periods * T);
    cout << " number of adaptive steps = " << steps << endl;
    cout << " step size: min = " << dt_min << "  max = " << dt_max << endl;
    cout << " data in file adaptive.data" << endl;
    dataFile.close();
    stepsizeFile.close();
}


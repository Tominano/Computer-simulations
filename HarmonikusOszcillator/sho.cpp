#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#include <bits/stdc++.h>

double omega;          // the natural frequency
double x, v;           // position and velocity at time t
int periods;           // number of periods to integrate
int stepsPerPeriod;    // number of time steps dt per period
string fileName;       // name of output file

void getInput();                 // for user to input parameters
void EulerCromer(double dt);
void Euler(double dt);     // takes an Euler-Cromer step
double energy();                 // computes the energy

void getInput ( ) {
    cout << "Enter omega: ";
    cin >> omega;
    cout << "Enter x(0) and v(0): ";
    cin >> x >> v;
    cout << "Enter number of periods: ";
    cin >> periods;
    cout << "Enter steps per period: ";
    cin >> stepsPerPeriod;
    cout << "Enter output file name: ";
    cin >> fileName;
}

void EulerCromer (double dt) {
    double a = - omega * omega * x;
    v += a * dt;
    x += v * dt;
}

void Euler (double dt) {
    double a = - omega * omega * x;
    x += v * dt;
    v += a * dt;
    
}

double energy ( ) {
    return 0.5 * (v * v + omega * omega * x * x);
}

int main ( ) {
    time_t start, end;
    time(&start); 

    getInput();
    ofstream file(fileName.c_str());
    if (!file) {
        cerr << "Cannot open " << fileName << "\nExiting ...\n";
        return 1;
    }
    const double pi = 4 * atan(1.0);
    double T = 2 * pi / omega;
    double dt = T / stepsPerPeriod;
    double t = 0;
    file << t << '\t' << x << '\t' << v << '\t' << 0 << '\n';
    for (int p = 1; p <= periods; p++) {
        for (int s = 0; s < stepsPerPeriod; s++) {
            Euler(dt);
            t += dt;
            file << t << '\t' << x << '\t' << v << '\t' << energy() << '\n';
        }
        cout << "Period = " << p << "\tt = " << t
             << "\tx = " << x << "\tv = " << v
             << "\tenergy = " << energy() << endl;
    }
    file.close();
    time(&end); 

    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed << time_taken << setprecision(100); 
    cout << " sec " << endl; 
}
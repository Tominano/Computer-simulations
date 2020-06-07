//--------------------------Includeok helye--------------------------//
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
//--------------------------Main--------------------------//
int main(int argc, char **argv)
{
//Futtatas: ./2popdin.bin r1 r2 k1 k2 alpha beta valami.data
    double r1 = atof(argv[1]);
    double r2 = atof(argv[2]);
    int k1 = atoi(argv[3]);
    int k2 = atoi(argv[4]);
    double alpha = atof(argv[5]);
    double beta = atof(argv[6]);
    char* fileName = argv[7];
    
    ofstream file(fileName);
    
    double dt = 0.05;
    double t = 0;
    double tMax = 100;

    file << t << '\t' << k1 << '\t' << k2 << '\n';
   
    double n1 = k1;
    double n2 = k2;
    while(t < tMax)
    {
        n1 += r1 * n1 * (1 - ((n1 + alpha * n2) / k1)) * dt;
        n2 += r2 * n2 * (1 - ((n2 + beta * n1) / k2)) * dt;
        t += dt;
        file << t << '\t' << n1 << '\t' << n2 << '\n';
    }

    cout << "Adatok: "<< fileName << endl;
    file.close();
    return 0;
}
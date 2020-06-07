#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
//Futtatas: ./3popdin.bin a b c d nyulakSzama rokakSzama valami.data S k melyikModszer
int main(int argc, char **argv)
{
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double c = atof(argv[3]);
    double d = atof(argv[4]);
    double n_nyul = atof(argv[5]);
    double n_roka = atof(argv[6]);
    char* fileName = argv[7];
    int S = atoi(argv[8]);
    int k = atoi(argv[9]);
    int modszer = atoi(argv[10]);
    ofstream file(fileName);
    double dt = 0.05;
    double t = 0;
    double tMax = 100;
    file << t << '\t' << n_nyul << '\t' << n_roka << '\n';
    if(modszer==1)
        while(t<tMax)
        {
            n_nyul += (a * n_nyul - b * n_roka * n_nyul ) * dt;
            n_roka += (c * n_nyul * n_roka - d * n_roka) * dt;
            t += dt;
            file << t << '\t' << n_nyul << '\t' << n_roka << '\n';
        }

    if(modszer==2)
        while(t<tMax)
    {
        n_nyul += (a*(1 - n_nyul / k) * n_nyul - b * n_roka * n_nyul ) * dt;
        n_roka += (c * n_nyul * n_roka - d * n_roka) * dt;
        t += dt;
        file << t << '\t' << n_nyul << '\t' << n_roka << '\n';
    }
    if(modszer==3)
    {
        while(t<tMax)
        {
            n_nyul += (a * n_nyul - b * ((n_roka * n_nyul) / (1 + n_nyul * S)) ) * dt;
            n_roka += (c * ((n_roka * n_nyul) / (1 + n_nyul * S)) - d * n_roka) * dt;
            t += dt;
            file << t << '\t' << n_nyul << '\t' << n_roka << '\n';
    }
    }
    cout << "Adatok: "<< fileName << endl;
    file.close();
    return 0;
}
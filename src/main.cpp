#include <iostream>
#include <fstream>
using namespace std;

// II - straight outta Anuța's curs*
// *almost
double **A;
int n;
int *predecesor;
const double MaxInf = 1.e20;
void Citire_Date(const char numeFisier[]) {
    int i,j;
    double c;
    ifstream fin(numeFisier);
    fin >> n;
    A = new double*[n];
    for(i=0; i<n; i++) {
        A[i] = new double[n];
        for(j=0;j<n;j++)
            if(i==j)
                A[i][j] = 0.0;
            else
                A[i][j] = MaxInf;
     }
    while(fin >> i >> j >> c)
        A[i][j] = c;
}
void afisare(int i) {
    if (predecesor[i] !=  -1)
        afisare(predecesor[i]);
    cout << i << " ";
}
int main() {
    Citire_Date("in.txt");
    predecesor = new int[n];
    int *selectat = new int[n];
    double *dmin = new double[n], min;
    int v, w = -1;
    for (int i = 0; i < n; i++) {
        selectat[i] = 0;
        dmin[i] = 0.0;
        predecesor[i] = -1;
    }
    cout << "Orasul de plecare S: ";
    cin >> v;
    selectat[v] = 1;
    for (int i = 0; i < n; i++) {
        dmin[i] = A[v][i];
        if (i != v)
            if (dmin[i] < MaxInf) predecesor[i] = v;
    }
    for (int i = 1; i <= n - 1; i++){
        min = MaxInf;
        for (int j = 0; j < n; j++)
            if (selectat[j] == 0)
                if (dmin[j] < min) {
                    min = dmin[j];
                    w = j;
                }
        if (w != -1) {
            selectat[w] = 1;
            for (int j = 0; j < n; j++)
                if (selectat[j] == 0)
                    if (dmin[j] > dmin[w] + A[w][j]) {
                        dmin[j] = dmin[w] + A[w][j];
                        predecesor[j] = w;
                    }
        }
    }
    int f;
    cout << "Orasul de sosire F: ";
    cin >> f;
    if (predecesor[f] != -1) {
        cout << "Costul drumului de la S (" << v << ") la F (" << f
             << ") este " << dmin[f] << endl;
        afisare(f);
        cout << endl;
    } else
        cout << "Nu exista drum de la S (" << v << ") la F (" << f << ")" << endl;
    cout << "Restul drumurilor: " << endl;
    for (int i = 0; i < n; i++)
        if (i != v) {
            if (predecesor[i] != -1) {
                cout << "Costul drumului de la " << v << " la " << i
                     << " este " << dmin[i] << endl;
                afisare(i);
                cout << endl;
            } else
                cout << "Nu exista drum de la " << v << " la " << i << endl;
        }
    return 0;
}

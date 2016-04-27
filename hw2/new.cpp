#include<iostream>
#include<fstream>
#include<math.h>

#define ERROR 10e-8

using namespace std;

double S, X, H, t, ss/* original 's' */, r;
int n, k;

double delta, rCap, u, d, p;

double **AMax;
double **AMin;
double ***Avg;
double ***Ans;

/* okay */
double ***make3(int a, int b, int c) {
  double ***A = new double**[a];
  for (int i = 0 ; i < a ; i++) {
    A[i] = new double*[b];
    for (int j = 0 ; j < b ; j++) {
      A[i][j] = new double[c];
    }
  }
  return A;
}

double **make2(int a, int b) {
  double **A = new double*[a];
  for (int i = 0 ; i < a ; i++) {
    A[i] = new double[b];
  }
  return A;
}

/* okay */
void calAMax() {
  double A, B;

  for(int j = 0; j <= n; j++) {
    for(int i = 0; i <= j; i++) {
      A = (1 - pow(u, j - i + 1)) / (1 - u);
      B = pow(u, j - i) * d * ((1 - pow(d, i)) / (1 - d));
      AMax[j][i] = (S * A + S * B) / (j + 1);
    }
  }
}

/* okay */
void calAMin() {
  double A, B;

  for(int j = 0; j <= n; j++) {
    for(int i = 0; i <= j; i++) {
      A = (1 - pow(d, i + 1)) / (1 - d);
      B = pow(d, i) * u * ((1 - pow(u, j - i)) / (1 - u));
      AMin[j][i] = (S * A + S * B) / (j + 1);
    }
  }
}

/* okay */
void calAvg() {
  for(int j = 0; j <= n; j++) {
    for(int i = 0; i <= j; i++) {
      for(int m = 0; m <= k; m++) {
        Avg[j][i][m] = ((k - m) / (double)k) * AMin[j][i] + (m / (double)k) * AMax[j][i];
      }
    }
  }
}

/* okay */
double solveAu(double cur_node, int j, int i) {
  return ((j + 1) * cur_node + S * pow(u, j + 1 - i) * pow(d, i)) / (double)(j + 2);
}
/* okay */
double solveAd(double cur_node, int j, int i) {
  return ((j + 1) * cur_node + S * pow(u, j - i) * pow(d, i + 1)) / (double)(j + 2);
}
/* okay */
int solveL(double A, int j, int i) {
  return (int)floor((A - AMin[j][i]) / ((AMax[j][i] - AMin[j][i]) / (double)k));
}
/* okay */
double solveXu(double Au, int j, int i, int l) {
  if(fabs(Avg[j + 1][i][l] - Avg[j + 1][i][l + 1]) < ERROR)
    return 0;
  else
    return (Au - Avg[j + 1][i][l + 1]) / (Avg[j + 1][i][l] - Avg[j + 1][i][l + 1]);
}
/* okay */
double solveXd(double Ad, int j, int i, int l) {
  if(fabs(Avg[j + 1][i + 1][l] - Avg[j + 1][i + 1][l + 1]) < ERROR)
    return 0;
  else
    return (Ad - Avg[j + 1][i + 1][l + 1]) / (Avg[j + 1][i + 1][l] - Avg[j + 1][i + 1][l + 1]);
}
/* okay */
double solveCu(double x, int j, int i, int l) {
  return x * Ans[j + 1][i][l] + (1 - x) * Ans[j + 1][i][l + 1];
}
/* okay */
double solveCd(double x, int j, int i, int l) {
  return x * Ans[j + 1][i + 1][l] + (1 - x) * Ans[j + 1][i + 1][l + 1];
}

double solve() {
  delta = t / n;
  rCap = r * delta;
  u = exp(ss * sqrt(delta));
  d = 1 / u;
  p = (exp(rCap) - d) / (u - d);

  AMax = make2(n + 1, n + 1);
  AMin = make2(n + 1, n + 1);
  calAMax(); // A_max(j, i)
  calAMin(); // A_min(j, i)

  Avg = make3(n + 1, n + 1, k + 1);
  Ans = make3(n + 1, n + 1, k + 1);
  calAvg(); // A_m(j, i);

  double Au, Ad, Xu, Xd, Cu, Cd, asnCall, amrCall;
  double Al, Al1;

  for(int j = n; j >= 0; j--) {
    for(int i = j; i >= 0; i--) {
      for(int m = 0; m <= k; m++) {
        Au = solveAu(Avg[j][i][m], j, i);
        Ad = solveAd(Avg[j][i][m], j, i);

        if(j == n) {
          Cu = fmax(Au - X, 0);
          Cd = fmax(Ad - X, 0);
        }
        else{
          Cu = -1;
          for(int l = 0; l < k; l++) {
            Al = Avg[j + 1][i][l];
            Al1 = Avg[j + 1][i][l + 1];

            if((Al < Au || fabs(Au - Al) < ERROR) && Au < Al1) {
              Xu = solveXu(Au, j, i, l);
              if(fabs(Xu - 1) < ERROR) {
                Xu = 1;
              }
              Cu = solveCu(Xu, j, i, l);
              break;
            }
          }

          if(Cu < 0) {
            Xu = solveXu(Au, j, i, k);
            if(fabs(Xu - 1) < ERROR) {
              Xu = 1;
            }
            Cu = solveCu(Xu, j, i, k);
          }

          if(Xu < 0 || Xu > 1) {
            cout << "@invalid" << Xu << ", " << Cu << endl;
            cout << "l: " << k << endl;
          }

          Cd = -1;
          for(int l = 0; l < k; l++) {
            Al = Avg[j + 1][i + 1][l];
            Al1 = Avg[j + 1][i + 1][l + 1];

            if((Al < Ad || fabs(Ad - Al) < ERROR) && Ad < Al1) {
              Xd = solveXd(Ad, j, i, l);
              if(fabs(Xd - 1) < ERROR) {
                Xd = 1;
              }
              Cd = solveCd(Xd, j, i, l);
              break;
            }
          }

          if(Cd < 0) {
            Xd = solveXd(Ad, j, i, k);
            if(fabs(Xd - 1) < ERROR) {
              Xd = 1;
            }
            Cd = solveCd(Xd, j, i, k);
          }

          if(Xd < 0 || Xd > 1) {
            cout << "!invalid" << Xd << ", " << Cd << endl;
            cout << "l: " << k << endl;
          }
        }

        amrCall = Avg[j][i][m] - X;
        asnCall = (p * Cu + (1 - p) * Cd) / exp(rCap);

        if(Avg[j][i][m] > H) {
          Ans[j][i][m] = 0;
        } else {
          Ans[j][i][m] = fmax(amrCall, asnCall);
        }
      }
    }
  }

  return Ans[0][0][0];
}

int main() {
  ifstream cin("input.txt");

  cin >> S >> X >> H >> t >> ss >> r >> n >>k;

  cout << solve() << endl;

  return 0;
}
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
#include<cmath>

#define MIN_ERR 10e-5

using namespace std;

int n; // the number of period
vector<float> S; // spot rates
vector<float> C; // cash flow
float W; // value defined in p.71
float mdfDr; // modified duration
float convx; // convexity

void input() {
  cout << "the number of period: ";
  cin >> n;

  float tS;
  for(int i = 0; i < n; i++) {
    cin >> tS;
    S.push_back(tS);
  }

  float tC;
  for(int i = 0; i < n; i++) {
    cin >> tC;
    C.push_back(tC);
  }

  cin >> W;
}

// calculate Present Value
float getPV() {
  float P = 0;
  for(int i = 0; i < n; i++) {
   P += C[i] / pow(1 + S[i], W + i);
  }

  //cout << "Present Value: " << P << endl;

  return P;
}

// calculate Macaulay Duration
float getMclDr() {
  float D = 0;
  for(int i = 0; i < n; i++) {
   D += (C[i] * (W + i)) / pow(1 + S[i], W + i);
  }

  float pv = getPV();
  D /= pv;

  //cout << "Macaulay Duration: " << D << endl;

  return D;
}

// approximation for Y2M using BISECTION
float getAppxY2m() {
  float pv = getPV();

  float start = 0;
  float end = 1;
  float y;

  while(1) {
    y = (start + end) / 2;

    float p = 0;
    for(int i = 0; i < n; i++) {
      p += C[i] / pow(1 + y, W + i);
    }

    if(abs(pv - p) < MIN_ERR) {
      break;
    }

    if(p > pv) {
      start = y;
    } else {
      end = y;
    }
  }

  return y;
}

// calculate Yield to Maturity
float getY2M() {
  float y = getAppxY2m();

  //cout << "Yield to Maturity: " << y << endl;

  return y;
}

float getConvexity() {
  float pv = getPV();
  float y = getAppxY2m();

  float tConvx = 0;

  for(int i = 0; i < n; i++) {
    tConvx += (C[i] * (W + i) * (W + i + 1)) / pow(1 + S[i], W + i);
  }
  tConvx /= (pv * pow(1 + y, 2));

  //cout << "Convexity: " << tConvx << endl;

  return tConvx;
}

void process() {
  // calculate Modified Duration
  float mclDr = getMclDr();
  float y = getY2M();

  mdfDr = mclDr / (1 + y);

  // calculate Convexity
  convx = getConvexity();
}

void output() {
  cout << mdfDr << " " << convx << endl;
}

int main() {
  cout.setf(ios::showpoint); // 끝의 0을 표시
  cout.precision(7);

  input();
  process();
  output();
}
#include <iostream>
#include <time.h>
#include <cmath>
#include <fstream>
#include <vector>
#include "matplotlibcpp.h"

#define _USE_MATH_DEFINES
#define TIMEPOINTS 256
#define NUM 14
#define FREQU 2000

void printValueX(float*);
//void wCalc(float* wconst);
float getDFT(int, float*);
void DFT(float*, float*);  //discrete Fourier transform
float getiDFT(int, float*);
void iDFT(float*, float*); //inverse discrete Fourier transform
void diff(float*, float*, float*);   //compere original signal with signsl after DFT and iDFT
float generatorSimple (int, float*, float*, float*);
void getSimpleSignal(float*, float*, float*, float*); //get pseudo random signal based on NUM=14 harmonic signals

using namespace std;
namespace plt = matplotlibcpp;

int main(){

  float valuesX [TIMEPOINTS] = {0};
  float spctr [TIMEPOINTS] = {0};
  float val [TIMEPOINTS] = {0};

  float amplitudes[NUM] = {0};
  float phathe[NUM] = {0};
  float freq[NUM] = {0};

  srand (time(NULL));
  //generator of parametersof NUM harmonics
  for (int i = 0; i < NUM; i++){
    amplitudes[i] = rand();
    phathe[i] = rand();
    freq[i] = rand() % 2000 + 1;
  }

  getSimpleSignal(&valuesX[0], &amplitudes[0], &phathe[0], &freq[0]);
  DFT(&valuesX[0], &spctr[0]);
  iDFT(&spctr[0], &val[0]);

  //float dif[TIMEPOINTS] = {0};
  //diff(&dif[0], &valuesX[0], &val[0]);

  //show spectr of generated pseudo random signal
  vector <double> u(TIMEPOINTS);
  for(int i = 0 ; i < TIMEPOINTS; i++)
    u[i] = spctr[i];

  plt::xkcd();
  plt::plot(u);
  plt::show();

  return 0;
}

void diff(float* result, float* a, float* b){
  for(int i = 0; i < TIMEPOINTS; i++)
    *(result + i) = fabs(*(a + i)) - fabs(*(b + i));
}

void DFT(float* valuesX, float* result){
  for(int i = 0; i < TIMEPOINTS; i++)
    *(result + i) = getDFT(i, valuesX);
}

float getDFT(int m, float* valuesX){
  float result = 0;
  float tmp = m * M_PI / TIMEPOINTS;
  for (int i = 0; i < TIMEPOINTS; i++)
    result += *(valuesX + i) * (cos(tmp * i) - sin (tmp * i));
  return result;
}

void iDFT(float* values, float* result){
  for(int i = 0; i < TIMEPOINTS; i++)
    *(result + i) = getDFT(i, values) / TIMEPOINTS;
}

float getiDFT(int m, float* values){
  float result = 0;
  float tmp = m * M_PI / TIMEPOINTS;
  for (int i = 0; i < TIMEPOINTS; i++)
    result += *(values + i) * (cos(tmp * i) + sin (tmp * i));
  return result;
}

void readTXT(float* valuesX){
  ifstream file("3.txt", ios::in);
  for (int i = 0; i < TIMEPOINTS; i++)
    file >> *(valuesX + i);
  file.close();
}

void printValueX (float* pValueX){
  for(int i = 0; i < TIMEPOINTS; i++)
    cout << *(pValueX + i) << endl;
}


float generatorSimple (int t, float* amplitudes, float* ph, float* fr){
  float result = 0;
  for(int i = 0; i < NUM; i++)
    result += *(amplitudes + i) * sin(*(fr + i) * t + *(ph + i) * M_PI);
  return result;
}

void getSimpleSignal(float* pValueX, float* amplitudes, float* ph, float* fr){
  for (int i = 0; i < TIMEPOINTS; i++)
    *(pValueX + i) = generatorSimple(i, amplitudes, ph, fr);
}

#include <iostream>
#include <time.h>
#include <cmath>
#include <fstream>
#include <vector>
//#include "matplotlibcpp.h"

#define _USE_MATH_DEFINES
#define TIMEPOINTS 256
#define NUM 14
#define FREQU 2000

void printValueX(float*);

float generatorSimple (int, float*, float*, float*);
void getSimpleSignal(float*, float*, float*, float*); //get pseudo random signal based on NUM=14 harmonic signals

void DFT (float *input, float **output, int size);
float abs0(float re, float im);
float phase(float re, float im);

using namespace std;
//namespace plt = matplotlibcpp;

int main(){
  float valuesX [TIMEPOINTS] = {0};

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

  // Memory Allocation
  float** X = new float*[2];
  X[0] = new float[TIMEPOINTS/2];
  X[1] = new float[TIMEPOINTS/2];

  // Calculation
  DFT(valuesX, X, TIMEPOINTS);

  for(int i = 0; i < TIMEPOINTS/2; i++){
      if(abs0(X[0][i], X[1][i])<0.001){
          X[0][i] = 0;
          X[1][i] = 0;
      }
      cout << i << ' ' << abs0(X[0][i], X[1][i]) << '\n';
  }

  // Memory Deallocation
  delete[] X[0];
  delete[] X[1];
  delete[] X;

  return 0;
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

void DFT (float *input, float **output, int size)
{
    // Memory Allocation
    float angle;
    float *reSum = new float[size];
    float *imSum = new float[size];

    // Frequency Cycle
    for (int k = 0; k < size/2; k++)
    {
        reSum[k] = 0;
        imSum[k] = 0;
        // Point Cycle
        for (int n = 0; n < size; n++)
        {
            angle     = -2*M_PI*k*n/size;
            reSum[k] += input[n] * cos(angle);
            imSum[k] += input[n] * sin(angle);
        }
        output[0][k] = reSum[k]*2/size;
        output[1][k] = imSum[k]*2/size;
    }

    // Memory Deallocation
    delete[] reSum;
    delete[] imSum;
}

//Complex Numbers
// Absolute value
float abs0(float re, float im)
{
    return pow(re*re + im*im, 0.5);
}
// Phase
float phase(float re, float im)
{
    return atan2(im, re)*180/M_PI;
}

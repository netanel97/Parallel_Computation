#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define CONSTRAINT 3
#define MASTER 0
#define THREDS_PER_BLOCK 256
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

typedef struct{
    int id;
    double x1;
    double x2;
    double a;
    double b;
}Point;


Point *readFromFile(int *N, int *K, double *D, int *tCount);

void calculateTValues(int tCount,int startIndex,int endIndex, double *tValues);

void inisitalizeProximitie(int *proximities,int tCount);

//void computeOnGPU(int *N,int *K,int D,double tValue,Point *allPoints,int *proximities);


//void calculateTValues(int tCount, double *tValues); /*Passed*/



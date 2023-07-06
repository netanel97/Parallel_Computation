#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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


Point *readFromFile(int* N,int* K,double* D,int* tCount);
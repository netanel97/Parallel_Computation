#include "myProto.h"


int main(int argc, char *argv[])
{
    clock_t startTime, endTime;
    Point *allPoints = NULL;
    int N, K, tCount;
    double D;
    double *tValues;

    printf("\n");
    allPoints = readFromFile(&N, &K,&D, &tCount);
    for(int i=0;i< N;i++)
    {
        printf("%d %1f %1f %1f %1f\n", allPoints[i].id, allPoints[i].x1, allPoints[i].x2, allPoints[i].a, allPoints[i].b);
    }
    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <omp.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include "myProto.h"

int main(int argc, char *argv[])
{
    int size, rank;
    int N;                   /*size of the data*/
    int K;                   /*Number of points that need to define Proximity Criteria*/
    int tCount;              /*Number of intervals + 1*/
    double D;                /*Distance between points*/
    double *tValues;         // actualTs
    Point *allPoints = NULL; /*Array of data that the MASTER read*/
    int *proximities = NULL; /*Array that store all the results*/
    // Point *pointChunk = NULL;
    // double *tValuesChunk = NULL;
    MPI_Status status;
    /*TODO: maybe need array of result for each proc and maybe not, maybe need to do itreation */
    /*for the above note, maybe i can create array with size N/size and for each index for start have -1 and after the proc
    finished, run on the array and count every ceil and check if its not -1 and send the array until -1*/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); /*Get the numbers of proc*/
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*Get for each proc the rank*/
    /*Creating MPI structs*/
    MPI_Datatype MPI_POINT;
    MPI_Type_contiguous(sizeof(Point), MPI_BYTE, &MPI_POINT); /* Creating Datatype that MPI will know*/
    MPI_Type_commit(&MPI_POINT);
    /*MASTER section that reading from file*/

    if (rank == MASTER)
    {
        allPoints = readFromFile(&N, &K, &D, &tCount); /*Reading from the file*/
        if (!allPoints)
        {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        // proximities = (int *)malloc((tCount + 1) * CONSTRAINT * sizeof(int)); /*For each point 3 possible values of points*/
        // inisitalizeProximitie(proximities, tCount);                           /*Init array proximite result by -1*/
    }
    // serval_Bcast() //TODO: create a function that do Bcast
    MPI_Bcast(&N, 1, MPI_INT, MASTER, MPI_COMM_WORLD); /*Broadcast N value*/

    MPI_Bcast(&K, 1, MPI_INT, MASTER, MPI_COMM_WORLD); /*Broadcast K value*/

    MPI_Bcast(&D, 1, MPI_INT, MASTER, MPI_COMM_WORLD); /*Broadcast D value*/

    MPI_Bcast(&tCount, 1, MPI_INT, MASTER, MPI_COMM_WORLD); /*Broadcast tCount value*/

    // tValues = (double *)malloc(sizeof(double) * (tCount + 1)); /*Each proc will have the tValue*/
    proximities = (int *)malloc((tCount + 1) * CONSTRAINT * sizeof(int)); /*For each point 3 possible values of points*/
    inisitalizeProximitie(proximities, tCount);                           /*Init array proximite result by -1*/

    if (rank != MASTER) /*Allocate mem for other proc*/
    {
        allPoints = (Point *)malloc(N * sizeof(Point)); /*Sending all the points to other proc*/
    }

    MPI_Bcast(allPoints, N, MPI_POINT, MASTER, MPI_COMM_WORLD); /*Broadcast allPoints(Array of struct) value*/

    // if (rank == MASTER) /*Calc the tValues*/
    // {
    //     calculateTValues(tCount, tValues);
    // }
    int chunkSize = (tCount + 1) / size;
    int remainingTvalues = (tCount + 1) % size;
    int startIndex = chunkSize * rank;
    int endIndex = startIndex + chunkSize + remainingTvalues;

    if (rank == MASTER)
    {
        chunkSize += remainingTvalues;
    }
    else
    {
        startIndex += remainingTvalues;
    }
    tValues = (double *)malloc(sizeof(double) * (chunkSize)); /*Each proc will have the tValue*/
    calculateTValues(tCount, startIndex, endIndex, tValues);
    MPI_Type_free(&MPI_POINT);
    free(allPoints);
    // free(pointChunk);
    MPI_Finalize();
    return 0;
}

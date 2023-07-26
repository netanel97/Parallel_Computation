#include "myProto.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
Point *readFromFile(int *N, int *K, double *D, int *tCount)
{
    FILE *file;
    Point *allPoints = NULL;
    /*Open file and read from the file*/
    file = fopen(INPUT_FILE, "r"); // open for read only
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open input file.\n");
        exit(1);
    }
    if (fscanf(file, "%d %d %lf %d\n", N, K, D, tCount) != 4)
    { /*Reading the first line*/
        fprintf(stderr, "Failed reading from file.\n");
        fclose(file);
        exit(1);
    }
    allPoints = (Point *)malloc((*N) * sizeof(Point)); /*Creating N spaces for points*/
    if (!allPoints)
    {
        fprintf(stderr, "Cannot Allocate memory.\n");
        fclose(file);
        exit(1);
    }
    /*Reading N lines of (X1,X2,a,b)*/
    for (int i = 0; i < *N; i++)
    {
        if (fscanf(file, "%d %lf %lf %lf %lf", &allPoints[i].id, &allPoints[i].x1, &allPoints[i].x2, &allPoints[i].a, &allPoints[i].b) != 5)
        {
            fprintf(stderr, "Failed reading from file.\n");
            fclose(file);
            free(allPoints);
            exit(1);
        }
    }
    fclose(file);
    return allPoints;
}

/*calculateTValues */
void calculateTValues(int tCount, double *tValues) /*Passed*/
{
    for (int i = 0; i <= tCount; ++i)
    {

        tValues[i] = (2.0 * i / tCount) - 1;
        printf("tValues[%d] = %f\n",i,tValues[i]);
    }
    printf("\nFinished calculating all t values \n");
}

void updateProximitePoints(int startingIndex, int *proximites, int pointId) /*i = 0,prox,point*/
{
    for (int i = 0; i < CONSTRAINT; i++)
    {
        int index = startingIndex * CONSTRAINT + i;
        if (proximites[index] == -1)
        {
            // printf("prox[%d] = %d\n",index,proximites[i]);
            proximites[index] = pointId; /*Put the point*/
            // printf("after prox[%d] = %d\n",index,proximites[index]);

            return;
        }
    }
}

double calcDistance(const Point p1, const Point p2, double t)
{

    double x1 = ((p1.x2 - p1.x1) / 2) * sin(t * M_PI / 2) + ((p1.x2 + p1.x1) / 2);
    double y1 = p1.a * x1 + p1.b;

    double x2 = ((p2.x2 - p2.x1) / 2) * sin(t * M_PI / 2) + (p2.x2 + p2.x1) / 2;
    double y2 = p2.a * x2 + p2.b;
  
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int calculateProximity(Point *allPoints, int N, double *tValues, int tCount, double distance, int *proximites, int K)
{
    int counter;
    for (int i = 0; i <= tCount; i++) /*running on tCounts*/
    {
        for (int j = 0; j < N; j++) /*Running on all the points*/
        {
            counter = 0;
            for (int k = 0; k < N; k++) /*check with other points but not the same point!*/
            {

                if (j != k && calcDistance(allPoints[j], allPoints[k], tValues[i]) < distance)
                {

                    counter++;
                    if (counter == K)
                    {
                        int pointId = allPoints[j].id;                 /*This point is proximity*/
                        updateProximitePoints(i, proximites, pointId); /*0,proximites,point that have 3 points*/
                        break;
                    }
                }
            }
        }
    }
}


void writeOutputFile(const char *filename, double *tValues, int tCount, int *proximity, Point *points, int N)
{
    FILE *file = fopen(filename, "w"); // Open the output file in write mode
    if (!file)
    {
        fprintf(stderr, "Failed to open output file.\n");
        exit(1);
    }

    int proximityFound = 0;
    for (int i = 0; i <= tCount; i++)
    {
        int counter = 0;
        int startIndex = CONSTRAINT * i;
        // printf("proximite[%d] = %d proximity[%d] = %d proximity[%d] = %d\n", startIndex, proximity[startIndex],
        //        startIndex + 1,
        //        proximity[startIndex + 1], startIndex + 2, proximity[startIndex + 2]);
        for (int j = startIndex; j < startIndex + CONSTRAINT; j++)
        {
            if (proximity[j] != -1)
            {
                // printf("prox[%d] = %d  tValue = %f\n",j,proximity[j],tValues[i]);
                counter++;
            }
        }
        // printf(" counter = %d\n",counter);
        if (counter == CONSTRAINT)
        {
            proximityFound = 1;

            fprintf(file, "Points ");
            for (int j = startIndex; j < CONSTRAINT + startIndex; j++)
            {
                fprintf(file, "pointID%d", proximity[j]); // Print the point IDs that satisfy the proximity criteria
                if (j < CONSTRAINT + startIndex - 1)
                    fprintf(file, ", ");
            }
            fprintf(file, " satisfy Proximity Criteria at t = %.2f\n", tValues[i]); // Print the t value
        }
    }
    if (!proximityFound)
    {
        fprintf(file, "There were no 3 points found for any t.\n"); // Print a message if no points satisfy the proximity criteria
    }

    fclose(file); // Close the file
}

int main(int argc, char *argv[])
{
    clock_t startTime, endTime;
    Point *allPoints = NULL;
    int N, K, tCount;
    double D;
    double *tValues; // actualTs
    int *proximites; // Array that store proximity criteria

    printf("\n");
    allPoints = readFromFile(&N, &K, &D, &tCount); /*Reading from the file*/

    tValues = (double *)malloc(sizeof(double) * (tCount + 1));
    if (!tValues)
    {
        fprintf(stderr, "Cannot Allocate memory.\n");
        exit(1);
    }
    calculateTValues(tCount, tValues); /*This function will build the Tcount Array*/

    proximites = (int *)malloc((tCount+1) * CONSTRAINT * sizeof(int)); /*Fore each point 3 possible values of points*/
    if (!proximites)
    {
        fprintf(stderr, "Failed to allocate proximites array.\n");
        exit(1);
    }
    for (int i = 0; i < (tCount+1) * CONSTRAINT; i++) /*Put -1 in the array*/
    {
        proximites[i] = -1;
        //printf("prox[%d] = %d\n",i,proximites[i]);
    }
    calculateProximity(allPoints, N, tValues, tCount, D, proximites, K); /*Finding if the points are proimity criteria*/
    // for (int i = 0; i < (tCount+1) * CONSTRAINT; i++) /*Put -1 in the array*/
    // {
    //     printf("i = %d and value = %d\n",i,proximites[i]);
    // }
    writeOutputFile(OUTPUT_FILE, tValues, tCount, proximites, allPoints, N);
    return 0;
}
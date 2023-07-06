#include "myProto.h"
#include <mpi.h>

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
    *tCount +=1; //For the last point
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

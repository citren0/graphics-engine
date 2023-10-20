
#include <iostream>
#include <string>
#include <cmath>

#include "../include/utils.hpp"


void initShape(struct shape * target)
{
    target->numVertices = 0;
    
    for (int i = 0; i < MAX_VERTICES_PER_SHAPE; i++)
    {
        for (int f = 0; f < MAX_VERTICES_PER_SHAPE; f++)
        {
            target->connectivity[i][f] = 0;
        }
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            target->vectors[i][f] = 0.0;
        }
    }
}

int addVertexToShape(struct shape * target, struct location point)
{
    if (target->numVertices >= MAX_VERTICES_PER_SHAPE)
    {
        return 1;
    }

    target->vectors[target->numVertices][0] = point.x;
    target->vectors[target->numVertices][1] = point.y;
    target->vectors[target->numVertices][2] = point.z;
    target->vectors[target->numVertices][3] = 1;

    target->numVertices++;

    return 0;
}

int addConnectionToShape(struct shape * target, int source, int destination)
{
    target->connectivity[source][destination] = 1;

    return 0;
}


void copyMatrix(double source[][4], double dest[][4], int n)
{
    for (int i = 0; i < 4; i++)
    {
        for (int f = 0; f < n; f++)
        {
            dest[i][f] = source[i][f];
        }
    }
}


// optimize
void matMatMult(double operation[4][4], double target[][4], double destination[][4], int n)
{
    double result[MAX_VERTICES_PER_SHAPE][4];

    for (int i = 0; i < n; i++)
    {
        for (int f = 0; f < 4; f++)
        {
            result[i][f] = 0;

            for (int j = 0; j < 4; j++)
            {
                result[i][f] += operation[j][f] * target[i][j];
            }
        }
    }

    copyMatrix(result, destination, n);
}


void printMat(double mat[][NUMBER_OF_HOMOGENEOUS_COORDS], int n)
{
    for (int i = 0; i < NUMBER_OF_HOMOGENEOUS_COORDS; i++)
    {
        printf("| ");
        for (int f = 0; f < n; f++)
        {
            printf("%f ", mat[f][i]);
        }
        printf("|\n");
    }
    printf("\n");
}
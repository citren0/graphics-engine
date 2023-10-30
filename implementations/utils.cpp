
#include <iostream>
#include <string>
#include <cmath>

#include "../include/utils.hpp"


void initShape(struct shape * target)
{
    target->numVertices = 0;
    
    for (int i = 0; i < MAX_VERTICES_PER_SHAPE; i++)
    {
        // Initialize each vertex to all zeroes.
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
    // Cannot add another vertex to a shape with the max number of vertices.
    if (target->numVertices >= MAX_VERTICES_PER_SHAPE)
    {
        return 1;
    }

    int numVertices = target->numVertices;

    target->vectors[numVertices][0] = point.x;
    target->vectors[numVertices][1] = point.y;
    target->vectors[numVertices][2] = point.z;
    target->vectors[numVertices][3] = 1;

    target->numVertices++;

    return 0;
}

int addConnectionToShape(struct shape * target, int source, int destination)
{
    target->connectivity[source][destination] = 1;

    return 0;
}


void copyMatrix(double source[][NUMBER_OF_HOMOGENEOUS_COORDS], double dest[][NUMBER_OF_HOMOGENEOUS_COORDS], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            dest[i][f] = source[i][f];
        }
    }
}


// optimize
void matMatMult(double operation[NUMBER_OF_HOMOGENEOUS_COORDS][NUMBER_OF_HOMOGENEOUS_COORDS], double target[][NUMBER_OF_HOMOGENEOUS_COORDS], double destination[][NUMBER_OF_HOMOGENEOUS_COORDS], int n)
{
    // TODO - In-place matrix multiplication.
    
    double result[MAX_VERTICES_PER_SHAPE][NUMBER_OF_HOMOGENEOUS_COORDS];

    for (int i = 0; i < n; i++)
    {
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            result[i][f] = 0;

            for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
            {
                result[i][f] += operation[j][f] * target[i][j];
            }
        }
    }

    copyMatrix(result, destination, n);
}


void printMat(double mat[][NUMBER_OF_HOMOGENEOUS_COORDS], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("\n");
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            printf("%f ", mat[i][f]);
        }
    }
}
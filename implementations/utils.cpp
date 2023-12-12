
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "../include/utils.hpp"


int initShape(struct shape * target)
{
    if (target == NULL)
    {
        return 1;
    }

    target->numVertices = 0;
    
    for (int i = 0; i < MAX_VERTICES_PER_SHAPE; i++)
    {
        // Initialize each vertex to all zeroes.
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            target->vertices[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0.0;
        }
    }

    for (int i = 0; i < NUMBER_OF_HOMOGENEOUS_COORDS; i++)
    {
        target->normal[i] = 0.0;
    }

    for (int i = 0; i < MAX_VERTICES_PER_SHAPE; i++)
    {
        target->connections[i] = -1;
    }

    return 0;
}

int connectShape(struct shape * target, int from, int to)
{
    if (target == NULL)
    {
        return 1;
    }

    if ((from > target->numVertices - 1 || from < 0) || (to > target->numVertices - 1 || to < 0))
    {
        return 1;
    }

    target->connections[from] = to;

    return 0;
}


int setShapeColor(struct shape * target, struct rgb color)
{
    if (target == NULL)
    {
        return 1;
    }

    target->color = color;

    return 0;
}


int addVertexToShape(struct shape * target, struct location point)
{
    if (target == NULL)
    {
        return 1;
    }

    int numVertices = target->numVertices;

    // Cannot add another vertex to a shape with the max number of vertices.
    if (numVertices >= MAX_VERTICES_PER_SHAPE)
    {
        return 1;
    }

    target->vertices[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 0] = point.x;
    target->vertices[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 1] = point.y;
    target->vertices[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 2] = point.z;
    target->vertices[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 3] = 1;

    target->numVertices++;

    return 0;
}


void copyMatrix1D(float * source, float * dest, int n)
{
    int indices = n * NUMBER_OF_HOMOGENEOUS_COORDS;

    int numThreads = 4;

    int indicesPerThread = indices / numThreads;

    int i = 0;

    #pragma omp parallel for
    for (i = 0; i < indices; i += indicesPerThread)
    {
        for (int f = 0; f < indicesPerThread; f++)
        {
            dest[i + f] = source[i + f];
        }
    }

    int leftOver = indices - (indicesPerThread * numThreads);
    for (int i = leftOver; i < indices; i++)
    {
        dest[i] = source[i];
    }
}


void crossProduct(float * vec1, float * vec2, float * result)
{
    result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}


float vectorMagnitude(float * vector, int vectorLength)
{
    float total = 0.0;

    for (int i = 0; i < vectorLength; i++)
    {
        total += pow(vector[i], 2);
    }

    return sqrt(total);
}


int scaleVector(float * vector, int vectorLength)
{
    if (vector == NULL)
    {
        return 1;
    }

    int mag = vectorMagnitude(vector, vectorLength);

    for (int i = 0; i < vectorLength; i++)
    {
        vector[i] /= mag;
    }

    return 0;
}


// https://stackoverflow.com/questions/22838071/robust-polygon-normal-calculation
int calculateNormal(struct shape * currShape)
{
    if (currShape == NULL)
    {
        return 1;
    }

    int numVertices = currShape->numVertices;

    // For this purpose we dont care about the homogeneous coordinates of the vectors we are examining.
    int vecLength = NUMBER_OF_HOMOGENEOUS_COORDS - 1;

    for (int i = 0; i < vecLength; i++)
    {
        currShape->normal[i] = 0.0;
    }

    for (int i = 0; i < numVertices - 1; i++)
    {
        float curr[vecLength];

        crossProduct(&currShape->vertices[i * NUMBER_OF_HOMOGENEOUS_COORDS], &currShape->vertices[(i + 1) * NUMBER_OF_HOMOGENEOUS_COORDS], curr);

        currShape->normal[0] += curr[0];
        currShape->normal[1] += curr[1];
        currShape->normal[2] += curr[2];
    }

    scaleVector(currShape->normal, 3);

    currShape->normal[3] = 1.0;

    return 0;

}


void printMat(float * mat, int numRows)
{
    for (int i = 0; i < numRows; i++)
    {
        printf("\n");
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            printf("%f ", mat[i * NUMBER_OF_HOMOGENEOUS_COORDS + f]);
        }
    }
}
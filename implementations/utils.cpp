
#include <chrono>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <omp.h>

#include "../include/utils.hpp"


void initShape(struct shape * target, double * vectors, int numShapes)
{
    target->numVertices = 0;
    target->vectors = vectors + (MAX_VERTICES_PER_SHAPE * NUMBER_OF_HOMOGENEOUS_COORDS * numShapes);
    
    for (int i = 0; i < MAX_VERTICES_PER_SHAPE; i++)
    {
        // Initialize each vertex to all zeroes.
        for (int f = 0; f < MAX_VERTICES_PER_SHAPE; f++)
        {
            target->connectivity[i][f] = 0;
        }

        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            target->vectors[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0.0;
        }
    }
}


double * initVertices()
{
    return (double *)malloc(NUMBER_OF_HOMOGENEOUS_COORDS * MAX_VERTICES_PER_SHAPE * MAX_SHAPES * sizeof(double));
}


// combinedVertices must have the correct number of rows, equal to the total number of vertices in targets.
// void combineVertices(std::vector<struct shape *> targets, double * combinedVertices)
// {
//     int numTargets = targets.size();
//     int row = 0;
//     //double * currIndex = combinedVertices;

//     for (int shape = 0; shape < numTargets; shape++)
//     {
//         int vertices = targets[shape]->numVertices;
//         struct shape * currShape = targets[shape];

//         for (int vertex = 0; vertex < vertices; vertex++)
//         {

//             combinedVertices[row * NUMBER_OF_HOMOGENEOUS_COORDS + 0] = currShape->vectors[vertex][0];
//             combinedVertices[row * NUMBER_OF_HOMOGENEOUS_COORDS + 1] = currShape->vectors[vertex][1];
//             combinedVertices[row * NUMBER_OF_HOMOGENEOUS_COORDS + 2] = currShape->vectors[vertex][2];
//             combinedVertices[row * NUMBER_OF_HOMOGENEOUS_COORDS + 3] = currShape->vectors[vertex][3];
            
//             // for (int coordinate = 0; coordinate < NUMBER_OF_HOMOGENEOUS_COORDS; coordinate++)
//             // {
//             //     *(currIndex++) = currShape->vectors[vertex][coordinate];
//             // }

//             row++;
//         }
//     }
// }


void applyTransform(double * vertices, double transform[NUMBER_OF_HOMOGENEOUS_COORDS][NUMBER_OF_HOMOGENEOUS_COORDS])
{
    auto start = std::chrono::high_resolution_clock::now();
        double * final = (double *)malloc(MAX_SHAPES * MAX_VERTICES_PER_SHAPE * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double));
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "malloc took " << duration.count()/1000 << " ms of processing." << std::endl;

    if (!final)
    {
        std::cout << "Malloc failed.\n";
        exit(1);
    }

    start = std::chrono::high_resolution_clock::now();
        matMatMult1D(transform, vertices, final, MAX_SHAPES * MAX_VERTICES_PER_SHAPE);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "matmatmult took " << duration.count()/1000 << " ms of processing." << std::endl;

    start = std::chrono::high_resolution_clock::now();
        copyMatrix1D(final, vertices, MAX_SHAPES * MAX_VERTICES_PER_SHAPE);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "copy matrix took " << duration.count()/1000 << " ms of processing." << std::endl;

    free(final);
}


// Under the C++ standard, vectors shall be contiguous in memory.
int countVertices(std::vector<struct shape *> targets)
{
    int totalVertices = 0;
    int numTargets = targets.size();

    for (int i = 0; i < numTargets; i++)
    {
        totalVertices += targets[i]->numVertices;
    }


    return totalVertices;
}


int addVertexToShape(struct shape * target, struct location point)
{
    int numVertices = target->numVertices;

    // Cannot add another vertex to a shape with the max number of vertices.
    if (numVertices >= MAX_VERTICES_PER_SHAPE)
    {
        return 1;
    }

    target->vectors[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 0] = point.x;
    target->vectors[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 1] = point.y;
    target->vectors[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 2] = point.z;
    target->vectors[numVertices * NUMBER_OF_HOMOGENEOUS_COORDS + 3] = 1;

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


void copyMatrix1D(double * source, double * dest, int n)
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


void matMatMult(double operation[NUMBER_OF_HOMOGENEOUS_COORDS][NUMBER_OF_HOMOGENEOUS_COORDS], double target[][NUMBER_OF_HOMOGENEOUS_COORDS], double destination[][NUMBER_OF_HOMOGENEOUS_COORDS], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            destination[i][f] = 0;

            for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
            {
                destination[i][f] += operation[j][f] * target[i][j];
            }
        }
    }
}

void matMatMult1D(double operation[NUMBER_OF_HOMOGENEOUS_COORDS][NUMBER_OF_HOMOGENEOUS_COORDS], double * target, double * destination, int n)
{
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
        {
            destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0;

            for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
            {
                destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] += operation[j][f] * target[i * NUMBER_OF_HOMOGENEOUS_COORDS + j];
            }
        }
    }
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
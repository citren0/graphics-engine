
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

#include "../include/display.hpp"
#include "../include/transforms.hpp"
#include "../include/gpu.hpp"



float projMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
        {S, 0, 0, 0,
         0, S, 0, 0,
         0, 0, -(far+near)/(far-near), -1,
         0, 0, (-2.0 * far * near)/(far-near), 0};



float * getProjMat()
{
    return projMat;
}


void initBuffer(char buf[SCREENHEIGHT * SCREENWIDTH])
{
    int arrSize = SCREENWIDTH * SCREENHEIGHT;
    for (int i = 0; i < arrSize; i++)
    {
        buf[i] = 0;
    }
}


void displayVertices(vector<struct shape> shapes, int * framebuf)
{    
    int maxVerticesToConsider = shapes.size() * MAX_VERTICES_PER_SHAPE;

    // Transform the vertices and scale homogeneous.
    gpuMatMatandScale(getProjMat(), shapes.size());

    // Draw vertices of the shapes.
    gpuDisplay(shapes.size());

    // Assign color values to framebuffer
    gpuPopulateFramebuffer(framebuf);

    printf("--------------\n");

}
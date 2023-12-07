
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "../include/transforms.hpp"
#include "../include/utils.hpp"
#include "../include/gpu.hpp"
#include "../include/constants.hpp"

// make the factor of movement changeable.
void moveShapesLeft(double * targets)
{
    // Matrices are stored with the vertical vectors contiguous in memory.
    double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] =
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             1, 0, 0, 1};

    gpuTransform(operation, MAXVERTICES);
}


void moveShapesRight(double * targets)
{
    double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                            -1, 0, 0, 1};

    gpuTransform(operation, MAXVERTICES);
}


void moveShapesUp(double * targets)
{
    double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 1, 0, 1};

    gpuTransform(operation, MAXVERTICES);

}


void moveShapesDown(double * targets)
{
    double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0,-1, 0, 1};

    gpuTransform(operation, MAXVERTICES);
}


void moveShapesIn(double * targets)
{
    double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0,-1, 1};

    gpuTransform(operation, MAXVERTICES);
}


void moveShapesOut(double * targets)
{
    double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 1, 1};


    gpuTransform(operation, MAXVERTICES);
}


// Will calculate the 3D center of two vertices.
void calculateCenterOfVertices(double vert1[4], double vert2[4], double result[3])
{
    result[0] = (vert1[0] + vert2[0]) / 2;
    result[1] = (vert1[1] + vert2[1]) / 2;
    result[2] = (vert1[2] + vert2[2]) / 2;
}


// Rotate the shape around (h,k)
void rotateShapesCCW(double * targets, struct location center, struct axis axisOfRotation)
{
    double thetaRads = PI/64.0;

    if (axisOfRotation.z)
    {
        double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), sin(thetaRads), 0, 0, 
                                 -sin(thetaRads), cos(thetaRads), 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1};
        
        double trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        double trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, MAXVERTICES);
        gpuTransform(rotMat, MAXVERTICES);
        gpuTransform(trans2, MAXVERTICES);
    }
    else if (axisOfRotation.x)
    {
        double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0, 
                                 0, cos(thetaRads), sin(thetaRads), 0,
                                 0, -sin(thetaRads), cos(thetaRads), 0,
                                 0, 0, 0, 1};

        double trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        double trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, MAXVERTICES);
        gpuTransform(rotMat, MAXVERTICES);
        gpuTransform(trans2, MAXVERTICES);
    }
    else if (axisOfRotation.y)
    {
        double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), 0, -sin(thetaRads), 0, 
                                 0, 1, 0, 0,
                                 sin(thetaRads), 0, cos(thetaRads), 0,
                                 0, 0, 0, 1};

        double trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        double trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, MAXVERTICES);
        gpuTransform(rotMat, MAXVERTICES);
        gpuTransform(trans2, MAXVERTICES);
    }
    else
    {
        return;
    }


}


void rotateShapesCW(double * targets, struct location center, struct axis axisOfRotation)
{
    double thetaRads = -PI/72.0;

    if (axisOfRotation.z)
    {
        double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), sin(thetaRads), 0, 0, 
                                 -sin(thetaRads), cos(thetaRads), 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1};
        
        double trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        double trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, MAXVERTICES);
        gpuTransform(rotMat, MAXVERTICES);
        gpuTransform(trans2, MAXVERTICES);
    }
    else if (axisOfRotation.x)
    {
        double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0, 
                                 0, cos(thetaRads), sin(thetaRads), 0,
                                 0, -sin(thetaRads), cos(thetaRads), 0,
                                 0, 0, 0, 1};

        double trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        double trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, MAXVERTICES);
        gpuTransform(rotMat, MAXVERTICES);
        gpuTransform(trans2, MAXVERTICES);
    }
    else if (axisOfRotation.y)
    {
        double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), 0, -sin(thetaRads), 0, 
                                 0, 1, 0, 0,
                                 sin(thetaRads), 0, cos(thetaRads), 0,
                                 0, 0, 0, 1};

        double trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        double trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, MAXVERTICES);
        gpuTransform(rotMat, MAXVERTICES);
        gpuTransform(trans2, MAXVERTICES);
    }
    else
    {
        return;
    }
}


void pivotCameraPitch(double * targets, double angle)
{
    double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0, 
                             0, cos(angle), sin(angle), 0,
                             0, -sin(angle), cos(angle), 0,
                             0, 0, 0, 1};

    gpuTransform(rotMat, MAXVERTICES);
}


void pivotCameraYaw(double * targets, double angle)
{
    double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {cos(angle), 0, -sin(angle), 0, 
                             0, 1, 0, 0,
                             sin(angle), 0, cos(angle), 0,
                             0, 0, 0, 1};

    gpuTransform(rotMat, MAXVERTICES);
}

void pivotCameraRoll(double * targets, double angle)
{
    double rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {cos(angle), -sin(angle), 0, 0, 
                             sin(angle),  cos(angle), 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1};

    gpuTransform(rotMat, MAXVERTICES);
}
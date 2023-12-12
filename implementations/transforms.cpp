
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "../include/transforms.hpp"
#include "../include/utils.hpp"
#include "../include/gpu.hpp"
#include "../include/shared.hpp"


// make the factor of movement changeable.
void moveShapesLeft(float amount, int numShapes)
{
    // Matrices are stored with the vertical vectors contiguous in memory.
    float operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] =
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             amount, 0, 0, 1};

    gpuTransform(operation, numShapes);
}


void moveShapesRight(float amount, int numShapes)
{
    float operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                            -amount, 0, 0, 1};

    gpuTransform(operation, numShapes);
}


void moveShapesUp(float amount, int numShapes)
{
    float operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, amount, 0, 1};

    gpuTransform(operation, numShapes);

}


void moveShapesDown(float amount, int numShapes)
{
    float operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0,-amount, 0, 1};

    gpuTransform(operation, numShapes);
}


void moveShapesIn(float amount, int numShapes)
{
    float operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0,-amount, 1};

    gpuTransform(operation, numShapes);
}


void moveShapesOut(float amount, int numShapes)
{
    float operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, amount, 1};


    gpuTransform(operation, numShapes);
}


// Will calculate the 3D center of two vertices.
void calculateCenterOfVertices(float vert1[4], float vert2[4], float result[3])
{
    result[0] = (vert1[0] + vert2[0]) / 2;
    result[1] = (vert1[1] + vert2[1]) / 2;
    result[2] = (vert1[2] + vert2[2]) / 2;
}


// Rotate the shape around (h,k)
void rotateShapesCCW(struct location center, struct axis axisOfRotation, float thetaRads, int numShapes)
{

    if (axisOfRotation.z)
    {
        float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), sin(thetaRads), 0, 0, 
                                 -sin(thetaRads), cos(thetaRads), 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1};
        
        float trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        float trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, numShapes);
        gpuTransform(rotMat, numShapes);
        gpuTransform(trans2, numShapes);
    }
    else if (axisOfRotation.x)
    {
        float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0, 
                                 0, cos(thetaRads), sin(thetaRads), 0,
                                 0, -sin(thetaRads), cos(thetaRads), 0,
                                 0, 0, 0, 1};

        float trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        float trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, numShapes);
        gpuTransform(rotMat, numShapes);
        gpuTransform(trans2, numShapes);
    }
    else if (axisOfRotation.y)
    {
        float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), 0, -sin(thetaRads), 0, 
                                 0, 1, 0, 0,
                                 sin(thetaRads), 0, cos(thetaRads), 0,
                                 0, 0, 0, 1};

        float trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        float trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, numShapes);
        gpuTransform(rotMat, numShapes);
        gpuTransform(trans2, numShapes);
    }
    else
    {
        return;
    }


}


void rotateShapesCW(struct location center, struct axis axisOfRotation, float thetaRads, int numShapes)
{

    if (axisOfRotation.z)
    {
        float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), sin(thetaRads), 0, 0, 
                                 -sin(thetaRads), cos(thetaRads), 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1};
        
        float trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        float trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, numShapes);
        gpuTransform(rotMat, numShapes);
        gpuTransform(trans2, numShapes);
    }
    else if (axisOfRotation.x)
    {
        float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0, 
                                 0, cos(thetaRads), sin(thetaRads), 0,
                                 0, -sin(thetaRads), cos(thetaRads), 0,
                                 0, 0, 0, 1};

        float trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        float trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, numShapes);
        gpuTransform(rotMat, numShapes);
        gpuTransform(trans2, numShapes);
    }
    else if (axisOfRotation.y)
    {
        float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {cos(thetaRads), 0, -sin(thetaRads), 0, 
                                 0, 1, 0, 0,
                                 sin(thetaRads), 0, cos(thetaRads), 0,
                                 0, 0, 0, 1};

        float trans1[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 -center.x, -center.y, -center.z, 1};

        float trans2[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                                {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 center.x, center.y, center.z, 1};


        gpuTransform(trans1, numShapes);
        gpuTransform(rotMat, numShapes);
        gpuTransform(trans2, numShapes);
    }
    else
    {
        return;
    }
}


void pivotCameraPitch(float angle, int numShapes)
{
    float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {1, 0, 0, 0, 
                             0, cos(angle), sin(angle), 0,
                             0, -sin(angle), cos(angle), 0,
                             0, 0, 0, 1};

    gpuTransform(rotMat, numShapes);
}


void pivotCameraYaw(float angle, int numShapes)
{
    float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {cos(angle), 0, -sin(angle), 0, 
                             0, 1, 0, 0,
                             sin(angle), 0, cos(angle), 0,
                             0, 0, 0, 1};

    gpuTransform(rotMat, numShapes);
}

void pivotCameraRoll(float angle, int numShapes)
{
    float rotMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                            {cos(angle), -sin(angle), 0, 0, 
                             sin(angle),  cos(angle), 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1};

    gpuTransform(rotMat, numShapes);
}
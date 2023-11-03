
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "../include/transforms.hpp"
#include "../include/utils.hpp"

using namespace std;


void scaleHomogenous(double source[][4], int n)
{
    for (int i = 0; i < n; i++)
    {
        source[i][0] = source[i][0] / source[i][3];
        source[i][1] = source[i][1] / source[i][3];
        source[i][2] = source[i][2] / source[i][3];
        source[i][3] = source[i][3] / source[i][3];
    }
}


void scaleHomogenous1D(double * source, int n)
{
    for (int i = 0; i < n; i++)
    {
        source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 0] = source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 0] / source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
        source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 1] = source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 1] / source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
        source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 2] = source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 2] / source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
        source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3] = source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3] / source[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
    }
}


// make the factor of movement changeable.
void moveShapesLeft(double * targets)
{
    // Matrices are stored with the vertical vectors contiguous in memory.
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 1, 0, 0, 1}};

    applyTransform(targets, operation);
}


void moveShapesRight(double * targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             {-1, 0, 0, 1}};

    applyTransform(targets, operation);
}


void moveShapesUp(double * targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 1, 0, 1}};

    applyTransform(targets, operation);

}


void moveShapesDown(double * targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0,-1, 0, 1}};

    applyTransform(targets, operation);
}


void moveShapesIn(double * targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 0, 1, 1}};

    applyTransform(targets, operation);
}


void moveShapesOut(double * targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 0,-1, 1}};


    applyTransform(targets, operation);
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
        double rotMat[4][4] = { {cos(thetaRads), sin(thetaRads), 0, 0}, 
                                {-sin(thetaRads), cos(thetaRads), 0, 0},
                                {0, 0, 1, 0},
                                {0, 0, 0, 1}};
        
        double trans1[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {-center.x, -center.y, -center.z, 1}};

        double trans2[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        applyTransform(targets, trans1);
        applyTransform(targets, rotMat);
        applyTransform(targets, trans2);
    }
    else if (axisOfRotation.x)
    {
        double rotMat[4][4] = { {1, 0, 0, 0}, 
                                {0, cos(thetaRads), sin(thetaRads), 0},
                                {0, -sin(thetaRads), cos(thetaRads), 0},
                                {0, 0, 0, 1}};

        double trans1[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {-center.x, -center.y, -center.z, 1}};

        double trans2[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        applyTransform(targets, trans1);
        applyTransform(targets, rotMat);
        applyTransform(targets, trans2);
    }
    else if (axisOfRotation.y)
    {
        double rotMat[4][4] = { {cos(thetaRads), 0, -sin(thetaRads), 0}, 
                                {0, 1, 0, 0},
                                {sin(thetaRads), 0, cos(thetaRads), 0},
                                {0, 0, 0, 1}};

        double trans1[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {-center.x, -center.y, -center.z, 1}};

        double trans2[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        applyTransform(targets, trans1);
        applyTransform(targets, rotMat);
        applyTransform(targets, trans2);
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
        double rotMat[4][4] = { {cos(thetaRads), sin(thetaRads), 0, 0}, 
                                {-sin(thetaRads), cos(thetaRads), 0, 0},
                                {0, 0, 1, 0},
                                {0, 0, 0, 1}};
        
        double trans1[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {-center.x, -center.y, -center.z, 1}};

        double trans2[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        applyTransform(targets, trans1);
        applyTransform(targets, rotMat);
        applyTransform(targets, trans2);
    }
    else if (axisOfRotation.x)
    {
        double rotMat[4][4] = { {1, 0, 0, 0}, 
                                {0, cos(thetaRads), sin(thetaRads), 0},
                                {0, -sin(thetaRads), cos(thetaRads), 0},
                                {0, 0, 0, 1}};

        double trans1[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {-center.x, -center.y, -center.z, 1}};

        double trans2[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        applyTransform(targets, trans1);
        applyTransform(targets, rotMat);
        applyTransform(targets, trans2);
    }
    else if (axisOfRotation.y)
    {
        double rotMat[4][4] = { {cos(thetaRads), 0, -sin(thetaRads), 0}, 
                                {0, 1, 0, 0},
                                {sin(thetaRads), 0, cos(thetaRads), 0},
                                {0, 0, 0, 1}};

        double trans1[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {-center.x, -center.y, -center.z, 1}};

        double trans2[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        applyTransform(targets, trans1);
        applyTransform(targets, rotMat);
        applyTransform(targets, trans2);
    }
    else
    {
        return;
    }
}


void pivotCameraPitch(double * targets, double angle)
{
    double rotMat[4][4] = { {1, 0, 0, 0}, 
                            {0, cos(angle), sin(angle), 0},
                            {0, -sin(angle), cos(angle), 0},
                            {0, 0, 0, 1}};

    applyTransform(targets, rotMat);
    
}


void pivotCameraYaw(double * targets, double angle)
{
    double rotMat[4][4] = { {cos(angle), 0, -sin(angle), 0}, 
                            {0, 1, 0, 0},
                            {sin(angle), 0, cos(angle), 0},
                            {0, 0, 0, 1}};

    applyTransform(targets, rotMat);
    
}
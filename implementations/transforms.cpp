
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


// TODO change so matrix is put in right place
// make the factor of movement changeable.
void moveShapesLeft(vector<struct shape *> targets)
{
    // Matrices are stored with the vertical vectors contiguous in memory.
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 1, 0, 0, 1}};

    int numTargets = targets.size();
    
    double buf[NUMBER_OF_HOMOGENEOUS_COORDS][MAX_VERTICES_PER_SHAPE];

    for (int i = 0; i < numTargets; i++)
    {
        // TODO - make num vertices not matter.
        matMatMult(operation, targets[i]->vectors, buf, targets[i]->numVertices);
        copyMatrix(buf, targets[i]->vectors, targets[i]->numVertices);
    }
}


void moveShapesRight(vector<struct shape *> targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             {-1, 0, 0, 1}};

    int numTargets = targets.size();

    double buf[NUMBER_OF_HOMOGENEOUS_COORDS][MAX_VERTICES_PER_SHAPE];

    for (int i = 0; i < numTargets; i++)
    {
        // TODO make num vertices not matter.
        matMatMult(operation, targets[i]->vectors, buf, targets[i]->numVertices);
        copyMatrix(buf, targets[i]->vectors, targets[i]->numVertices);
    }
}


void moveShapesUp(vector<struct shape *> targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 1, 0, 1}};

    int numTargets = targets.size();

    double buf[NUMBER_OF_HOMOGENEOUS_COORDS][MAX_VERTICES_PER_SHAPE];

    for (int i = 0; i < numTargets; i++)
    {
        // TODO make num vertices not matter.
        matMatMult(operation, targets[i]->vectors, buf, targets[i]->numVertices);
        copyMatrix(buf, targets[i]->vectors, targets[i]->numVertices);
    }
}


void moveShapesDown(vector<struct shape *> targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0,-1, 0, 1}};

    int numTargets = targets.size();

    double buf[NUMBER_OF_HOMOGENEOUS_COORDS][MAX_VERTICES_PER_SHAPE];

    for (int i = 0; i < numTargets; i++)
    {
        // TODO make num vertices not matter.
        matMatMult(operation, targets[i]->vectors, buf, targets[i]->numVertices);
        copyMatrix(buf, targets[i]->vectors, targets[i]->numVertices);
    }
}


void moveShapesIn(vector<struct shape *> targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 0, 1, 1}};

    int numTargets = targets.size();

    double buf[NUMBER_OF_HOMOGENEOUS_COORDS][MAX_VERTICES_PER_SHAPE];

    for (int i = 0; i < numTargets; i++)
    {
        // TODO make num vertices not matter.
        matMatMult(operation, targets[i]->vectors, buf, targets[i]->numVertices);
        copyMatrix(buf, targets[i]->vectors, targets[i]->numVertices);
    }
}


void moveShapesOut(vector<struct shape *> targets)
{
    double operation[4][4] = {{1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 0,-1, 1}};


    int numTargets = targets.size();

    double buf[NUMBER_OF_HOMOGENEOUS_COORDS][MAX_VERTICES_PER_SHAPE];
    
    for (int i = 0; i < numTargets; i++)
    {
        // TODO make num vertices not matter.
        matMatMult(operation, targets[i]->vectors, buf, targets[i]->numVertices);
        copyMatrix(buf, targets[i]->vectors, targets[i]->numVertices);
    }
}


// Will calculate the 3D center of two vertices.
void calculateCenterOfVertices(double vert1[4], double vert2[4], double result[3])
{
    result[0] = (vert1[0] + vert2[0]) / 2;
    result[1] = (vert1[1] + vert2[1]) / 2;
    result[2] = (vert1[2] + vert2[2]) / 2;
}


// Rotate the shape around (h,k)
void rotateShapesCCW(struct shape * target, struct location center, struct axis axisOfRotation)
{
    double thetaRads = PI/72.0;

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

        double trans3[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        // TODO make num vertices not matter.
        double buf[4][4];
        // Transform 1
        matMatMult(trans1, target->vectors, buf, target->numVertices);
        // Transform 2
        matMatMult(rotMat, buf, buf, target->numVertices);
        // Transform 3
        matMatMult(trans3, buf, target->vectors, target->numVertices);
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

        double trans3[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        // TODO make num vertices not matter.
        double buf[4][4];
        // Transform 1
        matMatMult(trans1, target->vectors, buf, target->numVertices);
        // Transform 2
        matMatMult(rotMat, buf, buf, target->numVertices);
        // Transform 3
        matMatMult(trans3, buf, target->vectors, target->numVertices);
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

        double trans3[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        // TODO make num vertices not matter.
        double buf[4][4];
        // Transform 1
        matMatMult(trans1, target->vectors, buf, target->numVertices);
        // Transform 2
        matMatMult(rotMat, buf, buf, target->numVertices);
        // Transform 3
        matMatMult(trans3, buf, target->vectors, target->numVertices);
    }
    else
    {
        return;
    }


}


void rotateShapesCW(struct shape * target, struct location center, struct axis axisOfRotation)
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

        double trans3[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        // TODO make num vertices not matter.
        double buf[4][4];
        // Transform 1
        matMatMult(trans1, target->vectors, buf, target->numVertices);
        // Transform 2
        matMatMult(rotMat, buf, buf, target->numVertices);
        // Transform 3
        matMatMult(trans3, buf, target->vectors, target->numVertices);
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

        double trans3[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        // TODO make num vertices not matter.
        double buf[4][4];
        // Transform 1
        matMatMult(trans1, target->vectors, buf, target->numVertices);
        // Transform 2
        matMatMult(rotMat, buf, buf, target->numVertices);
        // Transform 3
        matMatMult(trans3, buf, target->vectors, target->numVertices);
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

        double trans3[4][4] = { {1, 0, 0, 0},
                                {0, 1, 0, 0},
                                {0, 0, 1, 0},
                                {center.x, center.y, center.z, 1}};


        // TODO make num vertices not matter.
        double buf[4][4];
        // Transform 1
        matMatMult(trans1, target->vectors, buf, target->numVertices);
        // Transform 2
        matMatMult(rotMat, buf, buf, target->numVertices);
        // Transform 3
        matMatMult(trans3, buf, target->vectors, target->numVertices);
    }
    else
    {
        return;
    }
}


void pivotCameraPitch(vector<struct shape *> targets, double angle)
{
    double rotMat[4][4] = { {1, 0, 0, 0}, 
                            {0, cos(angle), sin(angle), 0},
                            {0, -sin(angle), cos(angle), 0},
                            {0, 0, 0, 1}};

    int numTargets = targets.size();
    for (int i = 0; i < numTargets; i++)
    {
        matMatMult(rotMat, targets[i]->vectors, targets[i]->vectors, targets[i]->numVertices);
    }
    
}


void pivotCameraYaw(vector<struct shape *> targets, double angle)
{
    double rotMat[4][4] = { {cos(angle), 0, -sin(angle), 0}, 
                            {0, 1, 0, 0},
                            {sin(angle), 0, cos(angle), 0},
                            {0, 0, 0, 1}};

    int numTargets = targets.size();
    for (int i = 0; i < numTargets; i++)
    {
        matMatMult(rotMat, targets[i]->vectors, targets[i]->vectors, targets[i]->numVertices);
    }
    
}
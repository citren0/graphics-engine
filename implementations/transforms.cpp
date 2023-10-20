
#include <iostream>
#include <string>
#include <cmath>

#include "../include/transforms.hpp"
#include "../include/utils.hpp"

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

void moveShapeLeft(struct shape * target)
{
    // Matrices are stored with the vertical vectors contiguous in memory.
    double operation[4][4] = {{ 1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             {-1, 0, 0, 1}};

    //matMatMult(operation, target->vectors, target->numVertices);
}

void moveShapeRight(struct shape * target)
{
    double operation[4][4] = {{ 1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 1, 0, 0, 1}};

    //matMatMult(operation, target->vectors, target->numVertices);
}

void moveShapeUp(struct shape * target)
{
    double operation[4][4] = {{ 1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 1, 0, 1}};

    //matMatMult(operation, target->vectors, target->numVertices);
}

void moveShapeDown(struct shape * target)
{
    double operation[4][4] = {{ 1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0,-1, 0, 1}};

    //matMatMult(operation, target->vectors, target->numVertices);
}

void moveShapeIn(struct shape * target)
{
    double operation[4][4] = {{ 1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 0, 1, 1}};

    //matMatMult(operation, target->vectors, target->numVertices);
}

void moveShapeOut(struct shape * target)
{
    double operation[4][4] = {{ 1, 0, 0, 0},
                             { 0, 1, 0, 0},
                             { 0, 0, 1, 0},
                             { 0, 0,-1, 1}};

    //matMatMult(operation, target->vectors, target->numVertices);
}



// Will calculate the 3D center of two vertices.
void calculateCenterOfVertices(double vert1[4], double vert2[4], double result[3])
{
    result[0] = (vert1[0] + vert2[0]) / 2;
    result[1] = (vert1[1] + vert2[1]) / 2;
    result[2] = (vert1[2] + vert2[2]) / 2;
}


// Rotate the shape around (h,k)
void rotateShapeCCW(struct shape * target, struct location center, struct axis axisOfRotation)
{
    double thetaRads = PI/36.0;

    if (axisOfRotation.z)
    {
        double rotMat[4][4] = {{cos(thetaRads), sin(thetaRads), 0, 0}, 
                            {-sin(thetaRads), cos(thetaRads), 0, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 1}};
        
        double trans1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-center.x, -center.y, -center.z, 1}};
        double trans3[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {center.x, center.y, center.z, 1}};

        //double trans1[4][4] = {{1, 0, 0, -center.x}, {0, 1, 0, -center.y}, {0, 0, 1, -center.z}, {0, 0, 0, 1}};
        //double trans3[4][4] = {{1, 0, 0, center.x}, {0, 1, 0, center.y}, {0, 0, 1, center.z}, {0, 0, 0, 1}};


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
        double rotMat[4][4] = {{1, 0, 0, 0}, 
                            {0, cos(thetaRads), sin(thetaRads), 0},
                            {0, -sin(thetaRads), cos(thetaRads), 0},
                            {0, 0, 0, 1}};
        double trans1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-center.x, -center.y, -center.z, 1}};
        double trans3[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {center.x, center.y, center.z, 1}};

        //double trans1[4][4] = {{1, 0, 0, -center.x}, {0, 1, 0, -center.y}, {0, 0, 1, -center.z}, {0, 0, 0, 1}};
        //double trans3[4][4] = {{1, 0, 0, center.x}, {0, 1, 0, center.y}, {0, 0, 1, center.z}, {0, 0, 0, 1}};


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
        double rotMat[4][4] = {{cos(thetaRads), 0, -sin(thetaRads), 0}, 
                            {0, 1, 0, 0},
                            {sin(thetaRads), 0, cos(thetaRads), 0},
                            {0, 0, 0, 1}};
        double trans1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-center.x, -center.y, -center.z, 1}};
        double trans3[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {center.x, center.y, center.z, 1}};

        //double trans1[4][4] = {{1, 0, 0, -center.x}, {0, 1, 0, -center.y}, {0, 0, 1, -center.z}, {0, 0, 0, 1}};
        //double trans3[4][4] = {{1, 0, 0, center.x}, {0, 1, 0, center.y}, {0, 0, 1, center.z}, {0, 0, 0, 1}};


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


void rotateShapeCW(struct shape * target, struct location center)
{
    double thetaRads = PI/36;

    double trans1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-center.x, -center.y, -center.z, 1}};
    double trans3[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {center.x, center.y, center.z, 1}};

    double rotMat[4][4] = {{cos(thetaRads), 0, -sin(thetaRads), 0}, 
                        {0, 1, 0, 0},
                        {sin(thetaRads), 0, cos(thetaRads), 0},
                        {0, 0, 0, 1}};

    

    // Transform 1
    //matMatMult(trans1, target->vectors, target->numVertices);
    // Transform 2
    //matMatMult(rotMat, target->vectors, target->numVertices);
    // Transform 3
    //matMatMult(trans3, target->vectors, target->numVertices);
}
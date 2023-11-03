#pragma once
#include "utils.hpp"
#include <vector>

using namespace std;

void scaleHomogenous(double source[][4], int n);

void scaleHomogenous1D(double * source, int n);

void moveShapesLeft(double * targets);

void moveShapesRight(double * targets);

void moveShapesUp(double * targets);

void moveShapesDown(double * targets);

void moveShapesIn(double * targets);

void moveShapesOut(double * targets);

void calculateCenterOfVertices(double vert1[4], double vert2[4], double result[3]);

void rotateShapesCCW(double * targets, struct location center, struct axis axisOfRotation);

void rotateShapesCW(double * targets, struct location center, struct axis axisOfRotation);

void pivotCameraPitch(double * targets, double angle);

void pivotCameraYaw(double * targets, double angle);
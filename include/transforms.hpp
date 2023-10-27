#pragma once
#include "utils.hpp"
#include <vector>

using namespace std;

void scaleHomogenous(double source[][4], int n);

void moveShapesLeft(vector<struct shape *> targets);

void moveShapesRight(vector<struct shape *> targets);

void moveShapesUp(vector<struct shape *> targets);

void moveShapesDown(vector<struct shape *> targets);

void moveShapesIn(vector<struct shape *> targets);

void moveShapesOut(vector<struct shape *> targets);

void calculateCenterOfVertices(double vert1[4], double vert2[4], double result[3]);

void rotateShapesCCW(struct shape * target, struct location center, struct axis axisOfRotation);

void rotateShapesCW(struct shape * target, struct location center, struct axis axisOfRotation);

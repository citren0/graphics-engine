#pragma once
#include "utils.hpp"
#include <vector>
#include "shared.hpp"

using namespace std;


#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "../include/transforms.hpp"
#include "../include/utils.hpp"
#include "../include/gpu.hpp"
#include "../include/shared.hpp"

// make the factor of movement changeable.
void moveShapesLeft(float amount, int numShapes);

void moveShapesRight(float amount, int numShapes);

void moveShapesUp(float amount, int numShapes);

void moveShapesDown(float amount, int numShapes);

void moveShapesIn(float amount, int numShapes);

void moveShapesOut(float amount, int numShapes);

void calculateCenterOfVertices(float vert1[4], float vert2[4], float result[3]);

void rotateShapesCCW(struct location center, struct axis axisOfRotation, float thetaRads, int numShapes);

void rotateShapesCW(struct location center, struct axis axisOfRotation, float thetaRads, int numShapes);

void pivotCameraPitch(float angle, int numShapes);

void pivotCameraYaw(float angle, int numShapes);

void pivotCameraRoll(float angle, int numShapes);
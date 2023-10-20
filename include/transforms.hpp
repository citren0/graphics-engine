#pragma once
#include "utils.hpp"

void scaleHomogenous(double source[][4], int n);

void moveShapeLeft(struct shape * target);

void moveShapeRight(struct shape * target);

void moveShapeUp(struct shape * target);

void moveShapeDown(struct shape * target);

void moveShapeIn(struct shape * target);

void moveShapeOut(struct shape * target);

void calculateCenterOfVertices(double vert1[4], double vert2[4], double result[3]);

void rotateShapeCCW(struct shape * target, struct location center, struct axis axisOfRotation);

void rotateShapeCW(struct shape * target, struct location center);
#pragma once
#include <vector>
#include "shared.hpp"


int addVertexToShape(struct shape * target, struct location point);

void copyMatrix1D(float * source, float * dest, int n);

void printMat(float * mat, int numRows);

int calculateNormal(struct shape * currShape);

int initShape(struct shape * target);

int setShapeColor(struct shape * target, struct rgb color);

int connectShape(struct shape * target, int from, int to);
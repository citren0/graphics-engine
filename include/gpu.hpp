#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <CL/cl.h>

#include "constants.hpp"


#define CHKERR(err, loc) if (err) { printf("error encountered at %d", loc); exit(1); }


// Function declarations
cl_device_id create_device();
cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename);

int gpuTransform(double * transform, unsigned int numRows);
int gpuMatMatandScale(double * projection, unsigned int numRows);
int gpuDisplay(unsigned int numRows);
int gpuPopulateFramebuffer(int * framebuffer);

int writeVerticesToGPU(double * vertices, int n);

int initMatMatMultGPU(unsigned int numRows);
void freeOpenCLKernel(void);
#pragma once

#include <CL/cl.h>

#include "constants.hpp"


#define CHKERR(err, loc) if (err) { printf("error encountered at %d", loc); exit(1); }


cl_device_id create_device();

cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename);

int gpuTransform(double * transform, unsigned int numRows);

int gpuMatMatandScale(double * projection, unsigned int numRows);

int gpuDisplay(unsigned int numRows);

int gpuPopulateFramebuffer(int * framebuffer);

int gpuInitializePixels(void);

int writeVerticesToGPU(double * vertices, int n);

int initMatMatMultGPU(unsigned int numRows);

void freeOpenCLKernel(void);
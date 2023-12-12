#pragma once

#include <CL/cl.h>
#include <vector>

#include "shared.hpp"


#define CHKERR(err, loc) if (err) { printf("error encountered at %d", loc); exit(1); }


cl_device_id create_device();

cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename);

int gpuTransform(float * transform, unsigned int numShapes);

int gpuMatMatandScale(float * projection, unsigned int numShapes);

int gpuDisplay(unsigned int numShapes);

int gpuPopulateFramebuffer(int * framebuffer);

int gpuInitializePixels(void);

int writeShapesToGPU(std::vector<struct shape> shapes);

int initMatMatMultGPU(void);

void freeOpenCLKernel(void);
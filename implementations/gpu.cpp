
#include <CL/cl.h>
#include <cstddef>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/gpu.hpp"
#include "../include/utils.hpp"
#include "../include/display.hpp"

#define PROGRAM_FILE "./implementations/clkernels/kernel.cl"

#define KERNEL_FUNC_TRANSFORM "transform"
#define KERNEL_FUNC_BOTH "matMatandScale"
#define KERNEL_FUNC_DISPLAYANDCONNECT "displayAndConnect"
#define KERNEL_FUNC_POPULATEFRAMEBUFFER "populateFramebuffer"
#define KERNEL_FUNC_COPYMATRIX "copyShapes" 
#define KERNEL_FUNC_INITIALIZEPIXELS "initializePixels"


cl_device_id device;
cl_context context;
cl_program program;
cl_kernel kernelTransform, kernelBoth, kernelDisplayandConnect, kernelPopulateFramebuffer, kernelCopyMatrix, kernelInitializePixels;
cl_command_queue queue;

size_t local_sizeTransform, global_sizeTransform;
size_t local_sizeBoth, global_sizeBoth;
size_t local_sizeDisplayandConnect, global_sizeDisplayandConnect;
size_t local_sizePopulateFramebuffer, global_sizePopulateFramebuffer;
size_t local_sizeCopyMatrix, global_sizeCopyMatrix;
size_t local_sizeInitializePixels, global_sizeInitializePixels;

cl_event event;

cl_mem dvertices, dprojection, dtransform, dprojected, dnumRows, dpixels, dframebuf, ddestination;


void freeOpenCLKernel(void)
{
   clReleaseKernel(kernelTransform);
   clReleaseKernel(kernelDisplayandConnect);
   clReleaseKernel(kernelBoth);
   clReleaseKernel(kernelPopulateFramebuffer);
   clReleaseKernel(kernelCopyMatrix);
   clReleaseKernel(kernelInitializePixels);

   clReleaseMemObject(dvertices);
   clReleaseMemObject(dprojected);
   clReleaseMemObject(dnumRows);
   clReleaseMemObject(dprojection);
   clReleaseMemObject(dpixels);
   clReleaseMemObject(dframebuf);
   clReleaseMemObject(ddestination);

   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
}



// SET NUMROWS CORRECTLY TO THE NUMBER OF SHAPES????


int initMatMatMultGPU(void)
{
   cl_int err;
   long long int sizeFramebuffer = SCREENWIDTH * SCREENHEIGHT * sizeof(float);
   long long int sizePixels = SCREENHEIGHT * SCREENWIDTH * sizeof(char);
   long long int sizeShapes = MAX_SHAPES * sizeof(struct shape);
   unsigned int numHomogCoords = NUMBER_OF_HOMOGENEOUS_COORDS;

   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   program = build_program(context, device, PROGRAM_FILE);
   queue = clCreateCommandQueue(context, device, 0, &err);


   // Allocate GPU memory
   dprojection = clCreateBuffer(context, CL_MEM_READ_WRITE, (NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(float)), NULL, NULL);
   dtransform = clCreateBuffer(context, CL_MEM_READ_WRITE, (NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(float)), NULL, NULL);
   dvertices = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeShapes, NULL, NULL); // HOLDS SHAPES
   dprojected = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeShapes, NULL, NULL); // HOLDS SHAPES
   ddestination = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeShapes, NULL, NULL); // HOLDS SHAPES
   dpixels = clCreateBuffer(context, CL_MEM_READ_WRITE, sizePixels, NULL, NULL);
   dframebuf = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeFramebuffer, NULL, NULL);
   dnumRows = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(unsigned int), NULL, NULL);
  

   // Create Each kernel.
   kernelTransform = clCreateKernel(program, KERNEL_FUNC_TRANSFORM, &err);
   kernelBoth = clCreateKernel(program, KERNEL_FUNC_BOTH, &err);
   kernelDisplayandConnect = clCreateKernel(program, KERNEL_FUNC_DISPLAYANDCONNECT, &err);
   kernelPopulateFramebuffer = clCreateKernel(program, KERNEL_FUNC_POPULATEFRAMEBUFFER, &err);
   kernelCopyMatrix = clCreateKernel(program, KERNEL_FUNC_COPYMATRIX, &err);
   kernelInitializePixels = clCreateKernel(program, KERNEL_FUNC_INITIALIZEPIXELS, &err);


   // Get local size.
   err |= clGetKernelWorkGroupInfo(kernelTransform, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeTransform), &local_sizeTransform, NULL);
   err |= clGetKernelWorkGroupInfo(kernelBoth, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeBoth), &local_sizeBoth, NULL);
   err |= clGetKernelWorkGroupInfo(kernelDisplayandConnect, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeDisplayandConnect), &local_sizeDisplayandConnect, NULL);
   err |= clGetKernelWorkGroupInfo(kernelPopulateFramebuffer, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizePopulateFramebuffer), &local_sizePopulateFramebuffer, NULL);
   err |= clGetKernelWorkGroupInfo(kernelCopyMatrix, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeCopyMatrix), &local_sizeCopyMatrix, NULL);
   err |= clGetKernelWorkGroupInfo(kernelInitializePixels, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeInitializePixels), &local_sizeInitializePixels, NULL);


   // Determine local and global size for each kernel.
   global_sizeTransform = ceil((float) MAX_SHAPES * MAX_VERTICES_PER_SHAPE / local_sizeTransform) * local_sizeTransform;
   global_sizeBoth = ceil((float) MAX_SHAPES * MAX_VERTICES_PER_SHAPE / local_sizeBoth) * local_sizeBoth;
   global_sizeDisplayandConnect = ceil((float) MAX_SHAPES * MAX_VERTICES_PER_SHAPE / local_sizeDisplayandConnect) * local_sizeDisplayandConnect;
   global_sizePopulateFramebuffer = ceil((float) SCREENHEIGHT / local_sizeDisplayandConnect) * local_sizeDisplayandConnect;
   global_sizeCopyMatrix = ceil((float) MAX_SHAPES / local_sizeCopyMatrix) * local_sizeCopyMatrix;
   global_sizeInitializePixels = ceil((float) SCREENHEIGHT / local_sizeInitializePixels) * local_sizeInitializePixels;


   // Kernel arguments.
   err |= clSetKernelArg(kernelTransform, 0, sizeof(cl_mem), &dtransform);
   err |= clSetKernelArg(kernelTransform, 1, sizeof(cl_mem), &dvertices);
   err |= clSetKernelArg(kernelTransform, 2, sizeof(cl_mem), &ddestination);
   err |= clSetKernelArg(kernelTransform, 3, sizeof(cl_mem), &dnumRows);

   err |= clSetKernelArg(kernelBoth, 0, sizeof(cl_mem), &dprojection);
   err |= clSetKernelArg(kernelBoth, 1, sizeof(cl_mem), &dvertices);
   err |= clSetKernelArg(kernelBoth, 2, sizeof(cl_mem), &dprojected);
   err |= clSetKernelArg(kernelBoth, 3, sizeof(cl_mem), &dnumRows);

   err |= clSetKernelArg(kernelDisplayandConnect, 0, sizeof(cl_mem), &dprojected);
   err |= clSetKernelArg(kernelDisplayandConnect, 1, sizeof(cl_mem), &dpixels);
   err |= clSetKernelArg(kernelDisplayandConnect, 2, sizeof(cl_mem), &dnumRows);

   err |= clSetKernelArg(kernelPopulateFramebuffer, 0, sizeof(cl_mem), &dframebuf);
   err |= clSetKernelArg(kernelPopulateFramebuffer, 1, sizeof(cl_mem), &dpixels);

   err |= clSetKernelArg(kernelInitializePixels, 0, sizeof(cl_mem), &dpixels);
   err |= clSetKernelArg(kernelInitializePixels, 1, sizeof(cl_mem), &dnumRows);

   // Check for errors.
   if (err)
   {
      printf("Error in initialization.\n");
      return 1;
   }
   
   return 0;
}


int gpuInitializePixels(void)
{
   clock_t begin, end;
   float time_spent;
   
   cl_int err = 0;

   long numRows = SCREENHEIGHT;

   begin = clock();

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numRows, 0, NULL, NULL);

   err |= clEnqueueNDRangeKernel(queue, kernelInitializePixels, 1, NULL, &global_sizeInitializePixels, &local_sizeInitializePixels, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);
   
   CHKERR(err, 90)

   end = clock();
   time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing initializePixels: %f\n\n\n", time_spent);

   return 0;
}


int writeShapesToGPU(std::vector<struct shape> shapes)
{
   cl_int err;

   int bytes = shapes.size() * sizeof(struct shape);

   err |= clEnqueueWriteBuffer(queue, dvertices, CL_TRUE, 0, bytes, &shapes[0], 0, NULL, NULL);

   CHKERR(err, 1);

   return 0;
}


int gpuCopyDestToOutput(unsigned int numShapes)
{
   clock_t begin, end;
   float time_spent;
   
   cl_int err = 0;

   long numRows = MAX_SHAPES * MAX_VERTICES_PER_SHAPE;

   begin = clock();

   err |= clSetKernelArg(kernelCopyMatrix, 0, sizeof(cl_mem), &ddestination);
   err |= clSetKernelArg(kernelCopyMatrix, 1, sizeof(cl_mem), &dvertices);
   err |= clSetKernelArg(kernelCopyMatrix, 2, sizeof(cl_mem), &dnumRows);

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numShapes, 0, NULL, NULL);

   err |= clEnqueueNDRangeKernel(queue, kernelCopyMatrix, 1, NULL, &global_sizeCopyMatrix, &local_sizeCopyMatrix, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);
   
   CHKERR(err, 90)

   end = clock();
   time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing copydesttooutput: %f\n\n\n", time_spent);

   return 0;
}


int gpuTransform(float * transform, unsigned int numShapes)
{
   clock_t begin, end;
   float time_spent;
   cl_int err = 0;

   unsigned int sizeOfTransform = NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(float);
   unsigned int numVertices = numShapes * MAX_VERTICES_PER_SHAPE;

   begin = clock();

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numVertices, 0, NULL, NULL);

   err |= clEnqueueWriteBuffer(queue, dtransform, CL_TRUE, 0, sizeOfTransform, transform, 0, NULL, NULL);

   err |= clEnqueueNDRangeKernel(queue, kernelTransform, 1, NULL, &global_sizeTransform, &local_sizeTransform, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 236)

   end = clock();
   time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing transform: %f\n\n\n", time_spent);

   gpuCopyDestToOutput(numShapes);

   return 0;
}


int gpuMatMatandScale(float * projection, unsigned int numShapes)
{
   clock_t begin, end;
   float time_spent;
   cl_int err = 0;

   unsigned int numVertices = numShapes * MAX_VERTICES_PER_SHAPE;

   begin = clock();

   err |= clEnqueueWriteBuffer(queue, dprojection, CL_TRUE, 0, (NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(float)), projection, 0, NULL, NULL);
   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numVertices, 0, NULL, NULL);
   
   CHKERR(err, 4)

   err |= clEnqueueNDRangeKernel(queue, kernelBoth, 1, NULL, &global_sizeBoth, &local_sizeBoth, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 5)

   end = clock();
   time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing matmatandscale: %f\n\n\n", time_spent);

   return 0;
}


int gpuDisplay(unsigned int numShapes)
{
   clock_t begin, end;
   float time_spent;

   unsigned int pixelsSize = SCREENWIDTH * SCREENHEIGHT * sizeof(char);
   
   cl_int err = 0;

   unsigned int numVertices = numShapes * MAX_VERTICES_PER_SHAPE;

   gpuInitializePixels();

   begin = clock();

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numVertices, 0, NULL, NULL);
   
   err |= clEnqueueNDRangeKernel(queue, kernelDisplayandConnect, 1, NULL, &global_sizeDisplayandConnect, &local_sizeDisplayandConnect, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 6)

   end = clock();
   time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing display: %f\n\n\n", time_spent);

   return 0;
}


int gpuPopulateFramebuffer(int * framebuffer)
{
   clock_t begin, end;
   float time_spent;
   
   cl_int err = 0;

   unsigned int framebufferSize = SCREENHEIGHT * SCREENWIDTH * sizeof(int);

   begin = clock();
   
   err |= clEnqueueNDRangeKernel(queue, kernelPopulateFramebuffer, 1, NULL, &global_sizePopulateFramebuffer, &local_sizePopulateFramebuffer, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   err |= clEnqueueReadBuffer(queue, dframebuf, CL_TRUE, 0, framebufferSize, framebuffer, 0, NULL, NULL);

   CHKERR(err, 7)

   end = clock();
   time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing populateframebuffer: %f\n\n\n", time_spent);

   return 0;
}
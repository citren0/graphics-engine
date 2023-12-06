
#include <CL/cl.h>
#include <cstddef>
#include <time.h>
#include <unistd.h>
#include "../include/gpu.hpp"
#include "../include/utils.hpp"
#include "../include/display.hpp"

#define PROGRAM_FILE "./implementations/clkernels/kernel.cl"

#define KERNEL_FUNC_TRANSFORM "transform"
#define KERNEL_FUNC_BOTH "matMatandScale"
#define KERNEL_FUNC_DISPLAYANDCONNECT "displayAndConnect"
#define KERNEL_FUNC_POPULATEFRAMEBUFFER "populateFramebuffer"
#define KERNEL_FUNC_COPYMATRIX "copyMatrix" 

cl_device_id device;
cl_context context;
cl_program program;
cl_kernel kernelTransform, kernelBoth, kernelDisplayandConnect, kernelPopulateFramebuffer, kernelCopyMatrix;
cl_command_queue queue;

size_t local_sizeTransform, global_sizeTransform;
size_t local_sizeBoth, global_sizeBoth;
size_t local_sizeDisplayandConnect, global_sizeDisplayandConnect;
size_t local_sizePopulateFramebuffer, global_sizePopulateFramebuffer;
size_t local_sizeCopyMatrix, global_sizeCopyMatrix;

cl_event event;

cl_mem dinput, dprojection, dtransform, doutput, dnumRows, dpixels, dframebuf, ddestination;


void freeOpenCLKernel(void)
{
   clReleaseKernel(kernelTransform);
   clReleaseKernel(kernelDisplayandConnect);
   clReleaseKernel(kernelBoth);
   clReleaseKernel(kernelPopulateFramebuffer);
   clReleaseKernel(kernelCopyMatrix);

   clReleaseMemObject(dinput);
   clReleaseMemObject(doutput);
   clReleaseMemObject(dnumRows);
   clReleaseMemObject(dprojection);
   clReleaseMemObject(dpixels);
   clReleaseMemObject(dframebuf);
   clReleaseMemObject(ddestination);

   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
}


int initMatMatMultGPU(unsigned int numRows)
{
   cl_int err;
   long long int numElements = numRows * NUMBER_OF_HOMOGENEOUS_COORDS;
   long long int sizeFramebuffer = SCREENWIDTH * SCREENHEIGHT * sizeof(double);
   long long int sizePixels = SCREENHEIGHT * SCREENWIDTH * sizeof(char);
   size_t bytes = numElements * sizeof(double);
   unsigned int numHomogCoords = NUMBER_OF_HOMOGENEOUS_COORDS;

   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   program = build_program(context, device, PROGRAM_FILE);
   queue = clCreateCommandQueue(context, device, 0, &err);


   // Allocate GPU memory
   dprojection = clCreateBuffer(context, CL_MEM_READ_WRITE, (NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double)), NULL, NULL);
   dtransform = clCreateBuffer(context, CL_MEM_READ_WRITE, (NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double)), NULL, NULL);

   dinput = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, NULL);
   doutput = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, NULL);
   ddestination = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, NULL);

   dpixels = clCreateBuffer(context, CL_MEM_READ_WRITE, sizePixels, NULL, NULL);
   dframebuf = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeFramebuffer, NULL, NULL);

   dnumRows = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, NULL);


   // Create Each kernel.
   kernelTransform = clCreateKernel(program, KERNEL_FUNC_TRANSFORM, &err);
   kernelBoth = clCreateKernel(program, KERNEL_FUNC_BOTH, &err);
   kernelDisplayandConnect = clCreateKernel(program, KERNEL_FUNC_DISPLAYANDCONNECT, &err);
   kernelPopulateFramebuffer = clCreateKernel(program, KERNEL_FUNC_POPULATEFRAMEBUFFER, &err);
   kernelCopyMatrix = clCreateKernel(program, KERNEL_FUNC_COPYMATRIX, &err);


   // Get local size.
   err |= clGetKernelWorkGroupInfo(kernelTransform, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeTransform), &local_sizeTransform, NULL);
   err |= clGetKernelWorkGroupInfo(kernelBoth, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeBoth), &local_sizeBoth, NULL);
   err |= clGetKernelWorkGroupInfo(kernelDisplayandConnect, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeDisplayandConnect), &local_sizeDisplayandConnect, NULL);
   err |= clGetKernelWorkGroupInfo(kernelPopulateFramebuffer, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizePopulateFramebuffer), &local_sizePopulateFramebuffer, NULL);
   err |= clGetKernelWorkGroupInfo(kernelCopyMatrix, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_sizeCopyMatrix), &local_sizeCopyMatrix, NULL);


   // Determine local and global size for each kernel.
   global_sizeTransform = ceil((float) numElements / local_sizeTransform) * local_sizeTransform;
   global_sizeBoth = ceil((float) numElements / local_sizeBoth) * local_sizeBoth;
   global_sizeDisplayandConnect = ceil((float) numElements / local_sizeDisplayandConnect) * local_sizeDisplayandConnect;
   global_sizePopulateFramebuffer = ceil((float) SCREENHEIGHT / local_sizeDisplayandConnect) * local_sizeDisplayandConnect;
   global_sizeCopyMatrix = ceil((float) numElements / local_sizeCopyMatrix) * local_sizeCopyMatrix;


   // Kernel arguments.
   err |= clSetKernelArg(kernelTransform, 0, sizeof(cl_mem), &dtransform);
   err |= clSetKernelArg(kernelTransform, 1, sizeof(cl_mem), &doutput);
   err |= clSetKernelArg(kernelTransform, 2, sizeof(cl_mem), &ddestination);
   err |= clSetKernelArg(kernelTransform, 3, sizeof(cl_mem), &dnumRows);

   err |= clSetKernelArg(kernelBoth, 0, sizeof(cl_mem), &dprojection);
   err |= clSetKernelArg(kernelBoth, 1, sizeof(cl_mem), &dinput);
   err |= clSetKernelArg(kernelBoth, 2, sizeof(cl_mem), &doutput);
   err |= clSetKernelArg(kernelBoth, 3, sizeof(cl_mem), &dnumRows);

   err |= clSetKernelArg(kernelDisplayandConnect, 0, sizeof(cl_mem), &doutput);
   err |= clSetKernelArg(kernelDisplayandConnect, 1, sizeof(cl_mem), &dpixels);
   err |= clSetKernelArg(kernelDisplayandConnect, 2, sizeof(cl_mem), &dnumRows);

   err |= clSetKernelArg(kernelPopulateFramebuffer, 0, sizeof(cl_mem), &dframebuf);
   err |= clSetKernelArg(kernelPopulateFramebuffer, 1, sizeof(cl_mem), &dpixels);

   // Check for errors.
   if (err)
   {
      printf("Error in initialization.\n");
      return 1;
   }
   
   return 0;
}


int writeVerticesToGPU(double * vertices, int n)
{
   cl_int err;

   int bytes = n * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double);

   err |= clEnqueueWriteBuffer(queue, doutput, CL_TRUE, 0, bytes, vertices, 0, NULL, NULL);

   CHKERR(err, 1);

   return 0;
}


int gpuCopyDestToOutput()
{
   clock_t begin, end;
   double time_spent;
   
   cl_int err = 0;

   long numRows = MAX_SHAPES * MAX_VERTICES_PER_SHAPE;

   begin = clock();

   err |= clSetKernelArg(kernelCopyMatrix, 0, sizeof(cl_mem), &ddestination);
   err |= clSetKernelArg(kernelCopyMatrix, 1, sizeof(cl_mem), &doutput);
   err |= clSetKernelArg(kernelCopyMatrix, 2, sizeof(cl_mem), &dnumRows);

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numRows, 0, NULL, NULL);

   CHKERR(err, 90)

   err |= clEnqueueNDRangeKernel(queue, kernelCopyMatrix, 1, NULL, &global_sizeCopyMatrix, &local_sizeCopyMatrix, 0, NULL, &event); 

   CHKERR(err, 91)

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);
   
   CHKERR(err, 92)

   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing copydesttooutput: %f\n\n\n", time_spent);

   return 0;
}


int gpuCopyOutputToInput()
{
   clock_t begin, end;
   double time_spent;
   
   cl_int err = 0;

   int numRows = MAX_SHAPES * MAX_VERTICES_PER_SHAPE;

   begin = clock();

   err |= clSetKernelArg(kernelCopyMatrix, 0, sizeof(cl_mem), &doutput);
   err |= clSetKernelArg(kernelCopyMatrix, 1, sizeof(cl_mem), &dinput);
   err |= clSetKernelArg(kernelCopyMatrix, 2, sizeof(cl_mem), &dnumRows);

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numRows, 0, NULL, NULL);

   CHKERR(err, 90)

   err |= clEnqueueNDRangeKernel(queue, kernelCopyMatrix, 1, NULL, &global_sizeCopyMatrix, &local_sizeCopyMatrix, 0, NULL, &event); 

   CHKERR(err, 91)

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 92)

   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing copyoutputtoinput: %f\n\n\n", time_spent);

   return 0;
}


int gpuTransform(double * transform, unsigned int numRows)
{
   clock_t begin, end;
   double time_spent;
   
   unsigned int sizeOfTransform = NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double);

   cl_int err = 0;

   begin = clock();

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numRows, 0, NULL, NULL);
   CHKERR(err, 233)
   err |= clEnqueueWriteBuffer(queue, dtransform, CL_TRUE, 0, sizeOfTransform, transform, 0, NULL, NULL);
   CHKERR(err, 234)
   err |= clEnqueueNDRangeKernel(queue, kernelTransform, 1, NULL, &global_sizeTransform, &local_sizeTransform, 0, NULL, &event); 
   CHKERR(err,235)
   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 236)

   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing transform: %f\n\n\n", time_spent);

   gpuCopyDestToOutput();

   return 0;
}


int gpuMatMatandScale(double * projection, unsigned int numRows)
{
   clock_t begin, end;
   double time_spent;
   

   long long int numElements = numRows * NUMBER_OF_HOMOGENEOUS_COORDS;
   size_t bytes = numElements * sizeof(double);
   cl_int err = 0;

   begin = clock();

   gpuCopyOutputToInput();

   double arrr[NUMBER_OF_HOMOGENEOUS_COORDS * 1];
   err |= clEnqueueReadBuffer(queue, dinput, CL_TRUE, 0, NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double), arrr, 0, NULL, NULL);
   printMat(arrr, 1);


   err |= clEnqueueWriteBuffer(queue, dprojection, CL_TRUE, 0, (NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS * sizeof(double)), projection, 0, NULL, NULL);
   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numRows, 0, NULL, NULL);
   
   err |= clEnqueueNDRangeKernel(queue, kernelBoth, 1, NULL, &global_sizeBoth, &local_sizeBoth, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 5)

   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing matmatandscale: %f\n\n\n", time_spent);

   return 0;
}


int gpuDisplay(unsigned int numRows)
{
   clock_t begin, end;
   double time_spent;

   unsigned int pixelsSize = SCREENWIDTH * SCREENHEIGHT * sizeof(char);
   
   cl_int err = 0;

   begin = clock();

   err |= clEnqueueWriteBuffer(queue, dnumRows, CL_TRUE, 0, sizeof(unsigned int), &numRows, 0, NULL, NULL);
   
   err |= clEnqueueNDRangeKernel(queue, kernelDisplayandConnect, 1, NULL, &global_sizeDisplayandConnect, &local_sizeDisplayandConnect, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   CHKERR(err, 6)

   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing display: %f\n\n\n", time_spent);

   return 0;
}


int gpuPopulateFramebuffer(int * framebuffer)
{
   clock_t begin, end;
   double time_spent;
   
   cl_int err = 0;

   unsigned int framebufferSize = SCREENHEIGHT * SCREENWIDTH * sizeof(int);

   begin = clock();
   
   err |= clEnqueueNDRangeKernel(queue, kernelPopulateFramebuffer, 1, NULL, &global_sizePopulateFramebuffer, &local_sizePopulateFramebuffer, 0, NULL, &event); 

   err |= clWaitForEvents(1, &event);
   err |= clFinish(queue);

   err |= clEnqueueReadBuffer(queue, dframebuf, CL_TRUE, 0, framebufferSize, framebuffer, 0, NULL, NULL);

   CHKERR(err, 7)

   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("time spent initializing and executing populateframebuffer: %f\n\n\n", time_spent);

   return 0;
}
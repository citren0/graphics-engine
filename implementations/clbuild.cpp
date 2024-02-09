
#include <iostream>

#include "../include/gpu.hpp"


// OpenCL Building based off of rsnemmen's examples.
// https://github.com/rsnemmen/OpenCL-examples

cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   err = clGetPlatformIDs(1, &platform, NULL);
   if (err < 0)
   {
      perror("Couldn't identify a platform");
      exit(1);
   } 

   // Grab device id of the GPU.
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   
   if (err == CL_DEVICE_NOT_FOUND)
   {
      // GPU cannot be found.
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }

   if (err < 0)
   {
      perror("Couldn't access any devices");
      exit(1);   
   }

   return dev;
}


cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {

   cl_program program;
   FILE *program_handle;
   char *program_buffer, *program_log;
   size_t program_size, log_size;
   int err;

   // OpenCL kernels are contained in a text file which will be compiled during runtime.
   program_handle = fopen(filename, "r");

   if (program_handle == NULL)
   {
      perror("Couldn't find the program file");
      exit(1);
   }

   fseek(program_handle, 0, SEEK_END);
   program_size = ftell(program_handle);
   rewind(program_handle);

   program_buffer = (char*)malloc(program_size + 1);
   program_buffer[program_size] = '\0';

   fread(program_buffer, sizeof(char), program_size, program_handle);
   fclose(program_handle);

   program = clCreateProgramWithSource(ctx, 1, (const char**)&program_buffer, &program_size, &err);
   if (err < 0)
   {
      perror("Couldn't create the program");
      exit(1);
   }
   free(program_buffer);

   err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
   if (err < 0)
   {
      // Build error, find log and print it.
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

      program_log = (char*) malloc(log_size + 1);
      program_log[log_size] = '\0';

      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, NULL);
      printf("%s\n", program_log);

      free(program_log);
      exit(1);
   }

   return program;
}



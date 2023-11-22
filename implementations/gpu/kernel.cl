/*
   Code that contains kernels to run on accelerator in parallel. A kernel 
   represents the basic unit of executable code. Each kernel will be 
   executed on one work item ("pixel") of your parallel task:

   1 work item = 1 "pixel" in your image 
*/

/*
   Since the work group size is used to tune performance and will 
   not necessarily be a devisor of the total number of threads needed 
   it is common to be 
   forced to launch more threads than are needed and ignore the extras. 
   After we check that we are inside of the problem domain we can access 
   and manipulate the device memory.
*/

/*
   void matMatMult1D(double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS], double * target, double * destination, int n)
   {
      #pragma omp parallel for
      for (int i = 0; i < n; i++)
      {

         // We will try to make n work groups and see the performance.

         for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
         {
               destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0;

               for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
               {
                  destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] += operation[j * NUMBER_OF_HOMOGENEOUS_COORDS + f] * target[i * NUMBER_OF_HOMOGENEOUS_COORDS + j];
               }
         }
      }
   }
*/

__kernel void oneDMatMatMult(__global double * operation, __global double * target, __global double * destination, int n, int numberHomogCoordinates)
{
	int i = get_global_id(0);


	if ((i >= 0) && (i < n)) {
		   for (int f = 0; f < numberHomogCoordinates; f++)
         {
            destination[i * numberHomogCoordinates + f] = 0;

            for (int j = 0; j < numberHomogCoordinates; j++)
            {
               destination[i * numberHomogCoordinates + f] += operation[j * numberHomogCoordinates + f] * target[i * numberHomogCoordinates + j];
            }
         }
	}
}

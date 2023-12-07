
#include "./include/constants.hpp"

#define dist(x1,y1,x2,y2) sqrt(pow(x2-x1, 2) + pow(y2-y1, 2))
#define scaleX(x) (int)(((double)(x + 1) / 2) * (SCREENWIDTH))
#define scaleY(y) (int)(((double)(y + 1) / 2) * (SCREENHEIGHT))



__kernel void transform(__global double * operation, __global double * target, __global double * destination, __global unsigned int * n)
{
	int i = get_global_id(0);

   unsigned int numRows = *n;

	if ((i >= 0) && (i < numRows))
   {
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


__kernel void copyMatrix(__global double * source, __global double * dest, __global unsigned int * n)
{
	int i = get_global_id(0);

   unsigned int numRows = *n;


	if ((i >= 0) && (i < numRows))
   {
		   for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
         {
            dest[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] = source[i * NUMBER_OF_HOMOGENEOUS_COORDS + f];
         }         
	}
}


__kernel void matMatandScale(__global double * operation, __global double * target, __global double * destination, __global unsigned int * n)
{
	int i = get_global_id(0);

   int numRows = *n;

	if ((i >= 0) && (i < numRows))
   {
      for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
      {
         destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0;

         for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
         {
            destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + f] += operation[j * NUMBER_OF_HOMOGENEOUS_COORDS + f] * target[i * NUMBER_OF_HOMOGENEOUS_COORDS + j];
         }
      }

      destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 0] = destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 0] / destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
      destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 1] = destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 1] / destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
      destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 2] = destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 2] / destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
      destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3] = destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3] / destination[i * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
	}
}


__kernel void initializePixels(__global char * pixels, __global unsigned int * n)
{
   int i = get_global_id(0);

   int numRows = *n;

	if ((i >= 0) && (i < numRows))
   {
      for (int f = 0; f < SCREENWIDTH; f++)
      {
         pixels[i * SCREENWIDTH + f] = 0;
      }
	}
}


__kernel void displayAndConnect(__global double * homogCurr, __global char * pixels, __global unsigned int * n)
{
	int i = get_global_id(0);

   int numRows = *n;

	if ((i >= 0) && (i < numRows))
   {
      int row = i * NUMBER_OF_HOMOGENEOUS_COORDS;

      // Unscaled coordinates are on the interval [-1,1]
      int x = scaleX(homogCurr[row + 0]);
      int y = scaleY(homogCurr[row + 1]);

      // Leave a small margin on each side of the display.
      if (x > 0 && x < SCREENWIDTH && y > 0 && y < SCREENHEIGHT)
      {
         pixels[y * SCREENWIDTH + x] = 1;
      }
	}
}



__kernel void populateFramebuffer(__global int * framebuf, __global char * pixels)
{
	int i = get_global_id(0);

	if ((i >= 0) && (i < SCREENHEIGHT))
   {
      for (int col = 0; col < SCREENWIDTH; col++)
      {
            int arrayPosition = i * SCREENWIDTH + col;

            if (pixels[arrayPosition] == 1)
            {
               // White
               framebuf[arrayPosition] = 0xFFFFFFFF;
            }
            else if (pixels[arrayPosition] == 2)
            {
               // Blue
               framebuf[arrayPosition] = 0xFF0000FF;
            }
            else
            {
               // Black
               framebuf[arrayPosition] = 0x00000000;
            }
      }
	}
}

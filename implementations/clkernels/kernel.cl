
#include "./include/shared.hpp"

#define dist(x1,y1,x2,y2) sqrt(pow(x2-x1, 2) + pow(y2-y1, 2))
#define scaleX(x) (int)(((float)(x + 1) / 2) * (SCREENWIDTH))
#define scaleY(y) (int)(((float)(y + 1) / 2) * (SCREENHEIGHT))

#define maxDistance (int)ceil(sqrt(pow((float)SCREENWIDTH, (float)2.0) + pow((float)SCREENHEIGHT, (float)2.0)))



__kernel void transform(__global float * operation, __global struct shape * target, __global struct shape * destination, __constant unsigned int * n)
{
	int i = get_global_id(0);

   int numRows = *n;

   int shapeIdx = i / MAX_VERTICES_PER_SHAPE;
   int localRow = i % MAX_VERTICES_PER_SHAPE;

   if ((i >= 0) && (i < numRows))
   {
      for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
      {
         (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0;

         for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
         {
            (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + f] += operation[j * NUMBER_OF_HOMOGENEOUS_COORDS + f] * (target + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + j];
         }
      }
   }

}


__kernel void copyShapes(__global struct shape * source, __global struct shape * dest, __constant unsigned int * n)
{
	int i = get_global_id(0);

   unsigned int numRows = *n;

	if ((i >= 0) && (i < numRows))
   {
		*(dest + i) = *(source + i);
	}

}


__kernel void matMatandScale(__global float * operation, __global struct shape * target, __global struct shape * destination, __constant unsigned int * n)
{
	int i = get_global_id(0);

   int numRows = *n;

   int shapeIdx = i / MAX_VERTICES_PER_SHAPE;
   int localRow = i % MAX_VERTICES_PER_SHAPE;

   if ((i >= 0) && (i < numRows))
   {
      for (int f = 0; f < NUMBER_OF_HOMOGENEOUS_COORDS; f++)
      {
         (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + f] = 0;

         for (int j = 0; j < NUMBER_OF_HOMOGENEOUS_COORDS; j++)
         {
            (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + f] += operation[j * NUMBER_OF_HOMOGENEOUS_COORDS + f] * (target + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + j];
         }
      }

      (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 0] = (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 0] / (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
      (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 1] = (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 1] / (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
      (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 2] = (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 2] / (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
      (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 3] = (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 3] / (destination + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 3];
   
   }

}


__kernel void initializePixels(__global char * pixels, __constant unsigned int * n)
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


__kernel void displayAndConnect(__global struct shape * homogCurr, __global char * pixels, __constant unsigned int * n)
{
	int i = get_global_id(0);

   int numRows = *n;

   int shapeIdx = i / MAX_VERTICES_PER_SHAPE;
   int localRow = i % MAX_VERTICES_PER_SHAPE;

   if ((i >= 0) && (i < numRows))
   {
      // Unscaled coordinates are on the interval [-1,1]
      int x = scaleX((homogCurr + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 0]);
      int y = scaleY((homogCurr + shapeIdx)->vertices[localRow * NUMBER_OF_HOMOGENEOUS_COORDS + 1]);

      // Leave a small margin on each side of the display.
      if (x > 0 && x < SCREENWIDTH && y > 0 && y < SCREENHEIGHT)
      {
         pixels[y * SCREENWIDTH + x] = 1;
      }

      // Each vertex will have at most one connection.
      int toVertex = (homogCurr + shapeIdx)->connections[localRow];
      // connections will be -1 if uninitialized.
      // if (toVertex != -1)
      // {
      //    int tox = scaleX((homogCurr + shapeIdx)->vertices[toVertex * NUMBER_OF_HOMOGENEOUS_COORDS + 0]);
      //    int toy = scaleY((homogCurr + shapeIdx)->vertices[toVertex * NUMBER_OF_HOMOGENEOUS_COORDS + 1]);

      //    int deltaX = tox - x;
      //    int deltaY = toy - y;

      //    int dist = (int)ceil(sqrt((float)(deltaX * deltaX) + (deltaY * deltaY)));

      //    if ((deltaX != 0 || deltaY != 0) && (dist < maxDistance))
      //    {

      //       float currX = x;
      //       float currY = y;

      //       float incrementX = (float)deltaX / dist;
      //       float incrementY = (float)deltaY / dist;

      //       for (int step = 0; step < dist; step++)
      //       {
      //          currX += incrementX;
      //          currY += incrementY;

      //          if (currX > 0 && currX < SCREENWIDTH && currY > 0 && currY < SCREENHEIGHT)
      //          {
      //             pixels[(int)currY * SCREENWIDTH + (int)currX] = 2;
      //          }
      //       }
      //    }
      // }
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
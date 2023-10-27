
#include <iostream>
#include <string>
#include <cmath>

#include "../include/display.hpp"
#include "../include/transforms.hpp"

// Field of View in degrees
#define fov 45.0

// Coefficient for projection matrix
#define S 1.0/(tan(fov*0.5*PI/180.0))

// Far and near clipping planes
#define far 100.0
#define near 0.1


double projMat[4][4] = {{S, 0, 0, 0},{0, S, 0, 0},{0, 0, -(far+near)/(far-near), -1},{0, 0, (-2.0 * far * near)/(far-near), 0}};



void initBuffer(char buf[SCREENHEIGHT][SCREENWIDTH])
{
    for (int i = 0; i < SCREENHEIGHT; i++)
    {
        for (int f = 0; f < SCREENWIDTH; f++)
        {
            buf[i][f] = 0;
        }
    }
}


void displayVertices(std::vector<struct shape *> shapes, int * framebuf)
{
    int numShapes = shapes.size();

    char pixels[SCREENHEIGHT][SCREENWIDTH];

    initBuffer(pixels);

    // Must calculate pixel location of each vertex and the connections between.
    for (int index = 0; index < numShapes; index++)
    {
        // TODO - make number of vertices not matter.
        

        struct shape * currShape = shapes[index];
        int n = currShape->numVertices;

        double homogCurr[n][NUMBER_OF_HOMOGENEOUS_COORDS];


        matMatMult(projMat, currShape->vectors, homogCurr, n);
        scaleHomogenous(homogCurr, n);

        
        for (int i = 0; i < n; i++)
        {
            double unscaledX = homogCurr[i][0];
            double unscaledY = homogCurr[i][1];

            // Leave a small margin on each side of the display.
            if (unscaledX > 0.95 || unscaledX < -0.95 || unscaledY > 0.95 || unscaledY < -0.95)
            {
                continue;
            }
            
            // Unscaled coordinates are on the interval [-1,1]
            int x = scaleX(unscaledX);
            int y = scaleY(unscaledY);

            pixels[y][x] = 1;
        }

        // Draw connection lines from vertex 'from' to vertex 'to'
        for (int from = 0; from < n; from++)
        {
            for (int to = 0; to < n; to++)
            {
                if (currShape->connectivity[from][to] == 1)
                {
                    // points 'from' and 'to' are connected.

                    double pixelDistance = dist(homogCurr[from][0]*(SCREENWIDTH), homogCurr[from][1]*(SCREENHEIGHT), homogCurr[to][0]*(SCREENWIDTH), homogCurr[to][1]*SCREENHEIGHT);

                    double deltaX = homogCurr[to][0] - homogCurr[from][0];
                    double deltaY = homogCurr[to][1] - homogCurr[from][1];
                    
                    double unscaledX = homogCurr[from][0];
                    double unscaledY = homogCurr[from][1];

                    for (int from = 0; from < pixelDistance; from++)
                    {
                        unscaledX += deltaX/pixelDistance;

                        unscaledY += deltaY/pixelDistance;

                        if (unscaledX < 0.95 && unscaledX > -0.95 && unscaledY < 0.95 && unscaledY > -0.95)
                        {
                            int x = scaleX(unscaledX);
                            int y = scaleY(unscaledY);

                            if (pixels[y][x] != 1)
                            {
                                pixels[y][x] = 2;
                            }
                        }

                        
                    }
                    
                    
                }
            }
        }

    }

    // Set color values in framebuffer according to pixel values.
    for (int row = 0; row < SCREENHEIGHT; row++)
    {
        for (int col = 0; col < SCREENWIDTH; col++)
        {
            int arrayPosition = row * SCREENWIDTH + col;

            if (pixels[row][col] == 1)
            {
                framebuf[arrayPosition] = 0xFFFFFFFF;
            }
            else if (pixels[row][col] == 2)
            {
                framebuf[arrayPosition] = 0xFF0000FF;
            }
            else
            {
                framebuf[arrayPosition] = 0x00000000;
            }
        }
    }

}
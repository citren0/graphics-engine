
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

#include "../include/display.hpp"
#include "../include/transforms.hpp"


double projMat[4][4] = {{S, 0, 0, 0},
                        {0, S, 0, 0},
                        {0, 0, -(far+near)/(far-near), -1},
                        {0, 0, (-2.0 * far * near)/(far-near), 0}};


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

    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop;
    int totalConnections = 0;

    // Must calculate pixel location of each vertex and the connections between.
    for (int index = 0; index < numShapes; index++)
    {
        // TODO - make number of vertices not matter.
        
        struct shape * currShape = shapes[index];

        int n = currShape->numVertices;

        double homogCurr[n][NUMBER_OF_HOMOGENEOUS_COORDS];

        bool isBehind = false;

        for (int i = 0; i < n; i++)
        {
            if (currShape->vectors[i][2] < 0)
            {
                isBehind = true;
                break;
            }
        }

        if (isBehind)
        {
            continue;
        }

        matMatMult(projMat, currShape->vectors, homogCurr, n);

        scaleHomogenous(homogCurr, n);

        // Draw vertices of the shapes.
        for (int i = 0; i < n; i++)
        {

            // Unscaled coordinates are on the interval [-1,1]
            int x = scaleX(homogCurr[i][0]);
            int y = scaleY(homogCurr[i][1]);

            // Leave a small margin on each side of the display.
            if (x > 0 && x < SCREENWIDTH && y > 0 && y < SCREENHEIGHT)
            {
                pixels[y][x] = 1;
            }

        }


        for (int from = 0; from < n; from++)
        {
            for (int to = 0; to < n; to++)
            {
                if (currShape->connectivity[from][to] == 1)
                {
                    int fromx = scaleX(homogCurr[from][0]);
                    int fromy = scaleY(homogCurr[from][1]);

                    int tox = scaleX(homogCurr[to][0]);
                    int toy = scaleY(homogCurr[to][1]);

                    int dist = dist(fromx, fromy, tox, toy);

                    // Investigate.
                    if (dist > dist(0, 0, SCREENWIDTH, SCREENHEIGHT))
                    {
                        continue;
                    }

                    double xIncrement = (dist != 0) ? (double)(tox - fromx) / dist : 0.0;
                    double yIncrement = (dist != 0) ? (double)(toy - fromy) / dist : 0.0;

                    double currx = fromx;
                    double curry = fromy;

                    // Problematic.
                    for (int march = 0; march < dist; march++)
                    {
                        //printf("dist %d\n currx %f\n curry %f\n\n", dist, currx, curry);
                        if (currx > 0 && currx < SCREENWIDTH && curry > 0 && curry < SCREENHEIGHT)
                        {
                            if (pixels[(int)curry][(int)currx] != 1)
                            {
                                pixels[(int)curry][(int)currx] = 2;
                            }
                        }

                        currx += xIncrement;
                        curry += yIncrement;
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
                // White
                framebuf[arrayPosition] = 0xFFFFFFFF;
            }
            else if (pixels[row][col] == 2)
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


    std::cout << "Connecting took " << totalConnections << " us" << std::endl;
    std::cout << std::endl << std::endl << std::endl;

}
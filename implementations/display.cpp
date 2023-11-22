
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

#include "../include/display.hpp"
#include "../include/transforms.hpp"


double projMat[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS] = 
                        {S, 0, 0, 0,
                         0, S, 0, 0,
                         0, 0, -(far+near)/(far-near), -1,
                         0, 0, (-2.0 * far * near)/(far-near), 0};


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


void displayVertices(vector<struct shape *> shapes, double * vertices, int * framebuf)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop;

    char pixels[SCREENHEIGHT][SCREENWIDTH];

    initBuffer(pixels);
    
    int maxVertices = MAX_SHAPES * MAX_VERTICES_PER_SHAPE;
        
    double * homogCurr = initVertices();

    matMatMult1D(projMat, vertices, homogCurr, maxVertices);

    scaleHomogenous1D(homogCurr, maxVertices);

    // Draw vertices of the shapes.
    #pragma omp parallel for
    for (int i = 0; i < maxVertices; i++)
    {

        int row = i * NUMBER_OF_HOMOGENEOUS_COORDS;

        // Unscaled coordinates are on the interval [-1,1]
        int x = scaleX(homogCurr[row + 0]);
        int y = scaleY(homogCurr[row + 1]);

        // Leave a small margin on each side of the display.
        if (x > 0 && x < SCREENWIDTH && y > 0 && y < SCREENHEIGHT)
        {
            pixels[y][x] = 1;
        }

    }


    int numShapes = shapes.size();
    struct shape * currShape;

    int maxConnDistance = dist(0, 0, SCREENWIDTH, SCREENHEIGHT);

    start = std::chrono::high_resolution_clock::now();
        #pragma omp parallel for
        for (int shape = 0; shape < numShapes; shape++)
        {
            currShape = shapes[shape];
            int numVertices = currShape->numVertices;
            double * homogCurrPos = homogCurr + (shape * MAX_VERTICES_PER_SHAPE * NUMBER_OF_HOMOGENEOUS_COORDS);

            for (int from = 0; from < numVertices; from++)
            {
                for (int to = 0; to < numVertices; to++)
                {
                    if (currShape->connectivity[from][to] == 1)
                    {
                        int fromx = scaleX(homogCurrPos[from * NUMBER_OF_HOMOGENEOUS_COORDS + 0]);
                        int fromy = scaleY(homogCurrPos[from * NUMBER_OF_HOMOGENEOUS_COORDS + 1]);

                        int tox = scaleX(homogCurrPos[to * NUMBER_OF_HOMOGENEOUS_COORDS + 0]);
                        int toy = scaleY(homogCurrPos[to * NUMBER_OF_HOMOGENEOUS_COORDS + 1]);

                        int dist = dist(fromx, fromy, tox, toy);

                        // Investigate.
                        if (dist > maxConnDistance)
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
    stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Connecting took a total of " << duration.count()/1000 << " ms of processing." << std::endl;

    
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

}
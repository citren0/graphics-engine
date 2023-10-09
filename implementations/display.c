
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "../include/display.h"
#include "../include/transforms.h"

#define fov 45.0

#define S 1.0/(tan(fov*0.5*PI/180.0))

#define far 100.0
#define near 0.1

#define left -1.0
#define right 1.0
#define top 1.0
#define bottom -1.0


float projMat[4][4] = {{S, 0, 0, 0},{0, S, 0, 0},{0, 0, -(far+near)/(far-near), -1},{0, 0, (-2.0 * far * near)/(far-near), 0}};



void initBuf(char buf[SCREENHEIGHT][SCREENWIDTH])
{
    for (int i = 0; i < SCREENHEIGHT; i++)
    {
        for (int f = 0; f < SCREENWIDTH; f++)
        {
            buf[i][f] = 0;
        }
    }
}


void displayVertices(struct shape * shapes[], int numShapes)
{
    char buf[SCREENHEIGHT][SCREENWIDTH];

    initBuf(buf);

    for (int index = 0; index < numShapes; index++)
    {
        // TODO  make number of vertices not matter.
        float homogCurr[4][4];

        struct shape * currShape = shapes[index];

        matMatMult(projMat, currShape->vectors, homogCurr, 4);
        scaleHomogenous(homogCurr, 4);



        int n = currShape->numVertices;
        for (int i = 0; i < n; i++)
        {
            float unscaledX = homogCurr[i][0];
            float unscaledY = homogCurr[i][1];

            if (unscaledX > 0.95 || unscaledX < -0.95 || unscaledY > 0.95 || unscaledY < -0.95)
            {
                continue;
            }
            int x = (int)((unscaledX*(SCREENWIDTH/2)) + (SCREENWIDTH/2));
            int y = (int)((unscaledY*(SCREENHEIGHT/2)) + (SCREENHEIGHT/2));
            buf[y][x] = 1;
        }

        // Draw lines
        for (int i = 0; i < n; i++)
        {
            for (int f = 0; f < n; f++)
            {
                if (currShape->connectivity[i][f] == 1)
                {
                    // points i and f are connected.

                    float pixelDistance = dist(homogCurr[i][0]*(SCREENWIDTH), homogCurr[i][1]*(SCREENHEIGHT), homogCurr[f][0]*(SCREENWIDTH), homogCurr[f][1]*SCREENHEIGHT);
                    float deltaX = homogCurr[f][0] - homogCurr[i][0];
                    float deltaY = homogCurr[f][1] - homogCurr[i][1];
                    
                    // truncate as thats what we do to all coordinates for now.

                    float unscaledX = homogCurr[i][0];
                    float unscaledY = homogCurr[i][1];

                    for (int i = 0; i < pixelDistance; i++)
                    {
                        unscaledX += deltaX/pixelDistance;

                        unscaledY += deltaY/pixelDistance;

                        if (unscaledX < 0.95 && unscaledX > -0.95 && unscaledY < 0.95 && unscaledY > -0.95)
                        {
                            int x = (int)((unscaledX*(SCREENWIDTH/2)) + (SCREENWIDTH/2));
                            int y = (int)((unscaledY*(SCREENHEIGHT/2)) + (SCREENHEIGHT/2));

                            if (buf[y][x] != 1)
                            {
                                buf[y][x] = 2;
                            }
                        }

                        
                    }
                    
                    
                }
            }
        }

    }

    printf(" ");
    for (int i = 0; i < SCREENWIDTH; i++)
    {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < SCREENHEIGHT; i++)
    {
        printf("|");
        for (int f = 0; f < SCREENWIDTH; f++)
        {
            if (buf[i][f] == 1)
            {
                printf("#");
            }
            else if (buf[i][f] == 2)
            {
                printf("·");
            }
            else
            {
                printf(" ");
            }

        }
        printf("|\n");
    }

    printf(" ");
    for (int i = 0; i < SCREENWIDTH; i++)
    {
        printf("-");
    }
    printf("\n");
}
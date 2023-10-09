
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "./include/display.h"
#include "./include/utils.h"
#include "./include/transforms.h"

#define MAXSHAPES 10


int main(void)
{
    struct shape * shapes[MAXSHAPES];
    int numShapes = 0;

    struct location center = {2.5, 2.5, 17.5};
    struct axis a1 = {0, 1, 0};


    struct shape square1;
    initShape(&square1);

    struct shape square2;
    initShape(&square2);


    addVertexToShape(&square1, (struct location){0, 0, 20});
    addVertexToShape(&square1, (struct location){5, 0, 20});
    addVertexToShape(&square1, (struct location){0, 5, 20});
    addVertexToShape(&square1, (struct location){5, 5, 20});

    addConnectionToShape(&square1, 0, 1);
    addConnectionToShape(&square1, 1, 3);
    addConnectionToShape(&square1, 2, 0);
    addConnectionToShape(&square1, 3, 2);


    addVertexToShape(&square2, (struct location){0, 0, 15});
    addVertexToShape(&square2, (struct location){5, 0, 15});
    addVertexToShape(&square2, (struct location){0, 5, 15});
    addVertexToShape(&square2, (struct location){5, 5, 15});

    addConnectionToShape(&square2, 0, 1);
    addConnectionToShape(&square2, 1, 3);
    addConnectionToShape(&square2, 2, 0);
    addConnectionToShape(&square2, 3, 2);




    shapes[numShapes] = &square1;
    numShapes++;

    shapes[numShapes] = &square2;
    numShapes++;


    float buf[4][4];


    for (;;)
    {
        
        char input;
        //scanf(" %c", &input);
        system("clear");

        input = 'e';


        switch (input)
        {
            case 'a':
                moveShapeLeft(&square1);
                break;
            case 'd':
                moveShapeRight(&square1);
                break;
            case 'w':
                moveShapeUp(&square1);
                break;
            case 's':
                moveShapeDown(&square1);
                break;
            case 'r':
                moveShapeOut(&square1);
                break;
            case 'f':
                moveShapeIn(&square1);
                break;
            case 'q':
                rotateShapeCW(&square1, center);
                break;
            case 'e':
                rotateShapeCCW(&square1, center, a1);
                rotateShapeCCW(&square2, center, a1);
                break;
        }


        printf("\n");
        

        displayVertices(shapes, numShapes);


        usleep(100000);
    }

    return 0;
}









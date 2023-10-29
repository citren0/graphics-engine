
#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <vector>
#include <chrono>

#include <X11/Xutil.h>

#include "./include/display.hpp"
#include "./include/utils.hpp"
#include "./include/transforms.hpp"
#include "./include/window.hpp"


// 33 fps lock.
#define FRAMETIME 30000


int main(void)
{

    XEvent event;
    int * framebuf;
    X11Window window;

    window.init_x();

    framebuf = window.getFrameBuffer();

    std::vector<struct shape *> shapes;


    struct shape * square;
    int gridSize = 1000;
    for (int i = 0; i < gridSize; i++)
    {
        for (int f = 0; f < gridSize; f++)
        {
            square = (struct shape *)malloc(sizeof(struct shape));

            if (square == NULL)
            {
                printf("Malloc failed.\n");
                exit(1);
            }

            initShape(square);
            addVertexToShape(square, (struct location){(double)0 + 5*f, 0, (double)15 + 5*i});
            addVertexToShape(square, (struct location){(double)5 + 5*f, 0, (double)20 + 5*i});
            addVertexToShape(square, (struct location){(double)0 + 5*f, 0, (double)20 + 5*i});
            addVertexToShape(square, (struct location){(double)5 + 5*f, 0, (double)15 + 5*i});

            addConnectionToShape(square, 0, 2);
            addConnectionToShape(square, 2, 1);
            addConnectionToShape(square, 1, 3);
            addConnectionToShape(square, 3, 0);

            shapes.push_back(square);
        }
    }


    for (;;)
    {

        XNextEvent(window.getDisplay(), &event);

        int ks;

        event.type = KeyPress;

        std::chrono::time_point<std::chrono::high_resolution_clock> start;

        switch (event.type)
        {
            case KeyPress:

                start = std::chrono::high_resolution_clock::now();

                ks = XLookupKeysym(&(event.xkey), 0);

                switch (ks)
                {
                    case XK_a:                        
                        moveShapesLeft(shapes);
                        break;
                    case XK_d:
                        moveShapesRight(shapes);
                        break;
                    case XK_w:
                        moveShapesUp(shapes);
                        break;
                    case XK_s:
                        moveShapesDown(shapes);
                        break;
                    case XK_r:
                        moveShapesOut(shapes);
                        break;
                    case XK_f:
                        moveShapesIn(shapes);
                        break;
                    case XK_q:
                        // rotateShapesCW(&square1, center, a1);
                        // rotateShapesCW(&square2, center, a1);
                        // rotateShapesCW(&square3, center, a1);
                        // rotateShapesCW(&square4, center, a1);
                        break;
                    case XK_e:
                        // rotateShapesCCW(&square1, center, a1);
                        // rotateShapesCCW(&square2, center, a1);
                        // rotateShapesCCW(&square3, center, a1);
                        // rotateShapesCCW(&square4, center, a1);
                        break;
                    case XK_i:
                        pivotCameraPitch(shapes, PI/64);
                        break;
                    case XK_k:
                        pivotCameraPitch(shapes, -PI/64);
                        break;
                    case XK_j:
                        pivotCameraYaw(shapes, PI/64);
                        break;
                    case XK_l:
                        pivotCameraYaw(shapes, -PI/64);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        printf("\n");

        displayVertices(shapes, framebuf);

        window.putImage();


        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Frame took " << duration.count()/1000 << " ms of processing." << std::endl;

        usleep(duration.count() < FRAMETIME ? FRAMETIME-duration.count() : 0);

    }

    return 0;
}









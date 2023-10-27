
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


#define MAXSHAPES 10

#define FRAMETIME 30000



int main(void)
{

    XEvent event;
    int * framebuf;

    X11Window window;

    window.init_x();

    framebuf = window.getFrameBuffer();


    std::vector<struct shape *> shapes;

    struct location center = {2.5, 2.5, 17.5};
    struct axis a1 = {0, 1, 0};


    struct shape square1;
    initShape(&square1);

    struct shape square2;
    initShape(&square2);

    struct shape square3;
    initShape(&square3);

    struct shape square4;
    initShape(&square4);


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


    addVertexToShape(&square3, (struct location){0, 0, 15});
    addVertexToShape(&square3, (struct location){5, 0, 20});
    addVertexToShape(&square3, (struct location){0, 5, 20});
    addVertexToShape(&square3, (struct location){5, 5, 15});

    addConnectionToShape(&square3, 0, 1);
    addConnectionToShape(&square3, 1, 3);
    addConnectionToShape(&square3, 2, 0);
    addConnectionToShape(&square3, 3, 2);


    addVertexToShape(&square4, (struct location){0, 0, 20});
    addVertexToShape(&square4, (struct location){5, 0, 15});
    addVertexToShape(&square4, (struct location){0, 5, 15});
    addVertexToShape(&square4, (struct location){5, 5, 20});

    addConnectionToShape(&square4, 0, 1);
    addConnectionToShape(&square4, 1, 3);
    addConnectionToShape(&square4, 2, 0);
    addConnectionToShape(&square4, 3, 2);

    shapes.push_back(&square1);
    shapes.push_back(&square2);
    shapes.push_back(&square3);
    shapes.push_back(&square4);


    for (;;)
    {

        XNextEvent(window.getDisplay(), &event);
        int ks;
        event.type = KeyPress;
        switch (event.type)
        {
            case KeyPress:
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
                        rotateShapesCW(&square1, center, a1);
                        rotateShapesCW(&square2, center, a1);
                        rotateShapesCW(&square3, center, a1);
                        rotateShapesCW(&square4, center, a1);
                        break;
                    case XK_e:
                        rotateShapesCCW(&square1, center, a1);
                        rotateShapesCCW(&square2, center, a1);
                        rotateShapesCCW(&square3, center, a1);
                        rotateShapesCCW(&square4, center, a1);
                        
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        printf("\n");


        // Create maximum framerate by timing and sleeping.
        auto start = std::chrono::high_resolution_clock::now();

        displayVertices(shapes, framebuf);

        window.putImage();


        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << "Slept for " << (double)duration.count() << " ms" << std::endl;

        usleep(duration.count() < FRAMETIME ? FRAMETIME-duration.count() : 0);

    }

    return 0;
}









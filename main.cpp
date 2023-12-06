
#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <vector>
#include <chrono>

#include <fstream>
#include <sstream>

#include <X11/Xutil.h>

#include "./include/display.hpp"
#include "./include/utils.hpp"
#include "./include/transforms.hpp"
#include "./include/window.hpp"
#include "./include/gpu.hpp"

// 33 fps lock.
#define FRAMETIME 30000

struct location center = {0, 0, 0};
struct axis a1 = {0, 1, 0};


int main(void)
{

    XEvent event;
    int * framebuf;
    X11Window window;

    window.init_x();

    initMatMatMultGPU(MAX_VERTICES_PER_SHAPE * MAX_SHAPES);

    framebuf = window.getFrameBuffer();

    std::vector<struct shape *> shapes;

    double * vertices = initVertices();

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

            initShape(square, vertices, shapes.size());

            addVertexToShape(square, (struct location){(double)0 + 5*f, 0, (double)15 + 5*i});
            addVertexToShape(square, (struct location){(double)5 + 5*f, 0, (double)20 + 5*i});
            addVertexToShape(square, (struct location){(double)0 + 5*f, 0, (double)20 + 5*i});
            addVertexToShape(square, (struct location){(double)5 + 5*f, 0, (double)15 + 5*i});

            shapes.push_back(square);
        }
    }

    writeVerticesToGPU(vertices, MAX_SHAPES * MAX_VERTICES_PER_SHAPE);

    // struct shape * square;
    // std::fstream f("object.obj" , std::ios::in);
    // std::string str;
    // square = (struct shape *)malloc(sizeof(struct shape));

    // if (square == NULL)
    // {
    //     exit(1);
    // }

    // initShape(square, vertices, shapes.size());

    // while (getline(f, str))
    // {
    //     if (str[0] == 'v')
    //     {
            
    //         std::stringstream strstream;
    //         strstream << str;
    //         char v;
    //         double a, b, c;
    //         strstream >> v >> a >> b >> c;

    //         std::cout << str << "       a " << a << " b " << b << " c " << c << std::endl;

    //         addVertexToShape(square, (struct location){a, b, c});        
    //     }
    // }
    // shapes.push_back(square);


    bool exit = false;
    bool render = true;
    int ks;

    while (!exit)
    {

        render = true;

        XNextEvent(window.getDisplay(), &event);

        switch (event.type)
        {
            case KeyPress:

                ks = XLookupKeysym(&(event.xkey), 0);

                switch (ks)
                {
                    case XK_a:                        
                        moveShapesLeft(vertices);
                        break;
                    case XK_d:
                        moveShapesRight(vertices);
                        break;
                    case XK_w:
                        moveShapesUp(vertices);
                        break;
                    case XK_s:
                        moveShapesDown(vertices);
                        break;
                    case XK_r:
                        moveShapesOut(vertices);
                        break;
                    case XK_f:
                        moveShapesIn(vertices);
                        break;
                    case XK_q:
                        rotateShapesCW(vertices, center, a1);
                        break;
                    case XK_e:
                        rotateShapesCCW(vertices, center, a1);
                        break;
                    case XK_i:
                        pivotCameraPitch(vertices, PI/64);
                        break;
                    case XK_k:
                        pivotCameraPitch(vertices, -PI/64);
                        break;
                    case XK_j:
                        pivotCameraYaw(vertices, -PI/64);
                        break;
                    case XK_l:
                        pivotCameraYaw(vertices, PI/64);
                        break;
                    case XK_Escape:
                        render = false;
                        exit = true;
                        break;
                    default:
                        render = false;
                        break;
                }
                break;
            default:
                break;
        }

        if (render)
        {
            displayVertices(shapes, vertices, framebuf);
            window.putImage();
        }

    }


    freeOpenCLKernel();

    return 0;
}









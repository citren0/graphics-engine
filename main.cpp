
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

    // struct shape * square;
    // int gridSize = 1000;
    // for (int i = 0; i < gridSize; i++)
    // {
    //     for (int f = 0; f < gridSize; f++)
    //     {
    //         square = (struct shape *)malloc(sizeof(struct shape));

    //         if (square == NULL)
    //         {
    //             printf("Malloc failed.\n");
    //             exit(1);
    //         }

    //         initShape(square, vertices, shapes.size());

    //         addVertexToShape(square, (struct location){(double)0 + 5*f, 0, (double)15 + 5*i});
    //         addVertexToShape(square, (struct location){(double)5 + 5*f, 0, (double)20 + 5*i});
    //         addVertexToShape(square, (struct location){(double)0 + 5*f, 0, (double)20 + 5*i});
    //         addVertexToShape(square, (struct location){(double)5 + 5*f, 0, (double)15 + 5*i});

    //         shapes.push_back(square);
    //     }
    // }

    struct shape * square;
    

    int gridCats = 10;
    for (int i = 0; i < gridCats; i++)
    {
        for (int j = 0; j < gridCats; j++)
        {
            std::fstream f("object.obj" , std::ios::in);
            std::string str;

            square = (struct shape *)malloc(sizeof(struct shape));

            if (square == NULL)
            {
                exit(1);
            }

            initShape(square, vertices, shapes.size());

            while (getline(f, str))
            {
                if (str[0] == 'v')
                {
                    
                    std::stringstream strstream;
                    strstream << str;
                    char v;
                    double a, b, c;
                    strstream >> v >> a >> b >> c;

                    addVertexToShape(square, (struct location){a + 40*i, b + 60*j, c});        
                }
            }
            shapes.push_back(square);
        }
    }
    

    writeVerticesToGPU(vertices, MAX_SHAPES * MAX_VERTICES_PER_SHAPE);





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
                        moveShapesRight(vertices);
                        break;
                    case XK_d:
                        moveShapesLeft(vertices);
                        break;
                    case XK_w:
                        moveShapesIn(vertices);
                        break;
                    case XK_s:
                        moveShapesOut(vertices);
                        break;
                    case XK_space:
                        moveShapesDown(vertices);
                        break;
                    case XK_Shift_L:
                        moveShapesUp(vertices);
                        break;
                    case XK_q:
                        pivotCameraRoll(vertices, -PI/32);
                        break;
                    case XK_e:
                        pivotCameraRoll(vertices, PI/32);
                        break;
                    case XK_i:
                        pivotCameraPitch(vertices, PI/32);
                        break;
                    case XK_k:
                        pivotCameraPitch(vertices, -PI/32);
                        break;
                    case XK_j:
                        pivotCameraYaw(vertices, -PI/32);
                        break;
                    case XK_l:
                        pivotCameraYaw(vertices, PI/32);
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









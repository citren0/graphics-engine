
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
#include "include/shared.hpp"

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

    initMatMatMultGPU();

    framebuf = window.getFrameBuffer();

    std::vector<struct shape> shapes;

    
    // int gridSize = 1000;
    // for (int i = 0; i < gridSize; i++)
    // {
    //     for (int f = 0; f < gridSize; f++)
    //     {
    //         struct shape square;

    //         CHK(initShape(&square))

    //         CHK(addVertexToShape(&square, (struct location){(float)0 + 5*f, 0, (float)0 + 5*i}))
    //         CHK(addVertexToShape(&square, (struct location){(float)5 + 5*f, 0, (float)5 + 5*i}))
    //         CHK(addVertexToShape(&square, (struct location){(float)0 + 5*f, 0, (float)5 + 5*i}))
    //         CHK(addVertexToShape(&square, (struct location){(float)5 + 5*f, 0, (float)0 + 5*i}))

    //         CHK(calculateNormal(&square))

    //         CHK(setShapeColor(&square, (struct rgb){255, 0, 0}))

    //         CHK(connectShape(&square, 0, 2))
    //         CHK(connectShape(&square, 2, 1))
    //         CHK(connectShape(&square, 1, 3))
    //         CHK(connectShape(&square, 3, 0))

    //         shapes.push_back(square);
    //     }
    // }

    
    int gridCats = 10;

    for (int row = 0; row < gridCats; row++)
    {
        for (int col = 0; col < gridCats; col++)
        {
            std::fstream f("object.obj" , std::ios::in);
            std::string str;
            
            while (f.peek() != EOF)
            {
                struct shape first;

                CHK(initShape(&first))

                while (getline(f, str) && first.numVertices < MAX_VERTICES_PER_SHAPE)
                {
                    std::stringstream strstream;
                    strstream << str;
                    string v;
                    float a, b, c;
                    strstream >> v >> a >> b >> c;

                    if (v == "v")
                    {
                        if (addVertexToShape(&first, (struct location){a + 40*row, b + 60*col, c}))
                        {
                            break;
                        }
                    }
                }

                shapes.push_back(first);
            }
        }
    }

    writeShapesToGPU(shapes);





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
                        moveShapesRight(1, shapes.size());
                        break;
                    case XK_d:
                        moveShapesLeft(1, shapes.size());
                        break;
                    case XK_w:
                        moveShapesIn(1, shapes.size());
                        break;
                    case XK_s:
                        moveShapesOut(1, shapes.size());
                        break;
                    case XK_space:
                        moveShapesDown(1, shapes.size());
                        break;
                    case XK_Shift_L:
                        moveShapesUp(1, shapes.size());
                        break;
                    case XK_q:
                        pivotCameraRoll(-PI/32, shapes.size());
                        break;
                    case XK_e:
                        pivotCameraRoll(PI/32, shapes.size());
                        break;
                    case XK_i:
                        pivotCameraPitch(PI/32, shapes.size());
                        break;
                    case XK_k:
                        pivotCameraPitch(-PI/32, shapes.size());
                        break;
                    case XK_j:
                        pivotCameraYaw(-PI/32, shapes.size());
                        break;
                    case XK_l:
                        pivotCameraYaw(PI/32, shapes.size());
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
            auto start = std::chrono::high_resolution_clock::now();
                displayVertices(shapes, framebuf);
                window.putImage();
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            float milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            std::cout << "display and putimage took " << milliseconds/1000 << " seconds.\n";
        }

    }


    freeOpenCLKernel();

    return 0;
}









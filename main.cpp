
#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <vector>
#include <chrono>

// X11 libraries
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "./include/display.hpp"
#include "./include/utils.hpp"
#include "./include/transforms.hpp"

#define MAXSHAPES 10

int * framebuf;
XSetWindowAttributes attrs;
Window parent;
Visual *visual;
Window win;
XImage *ximage;
XEvent event;
Display *dpy;
XVisualInfo vinfo;
int depth;
XVisualInfo *visual_list;
XVisualInfo visual_template;
int nxvisuals;
GC NormalGC;


void init_x() 
{

    int i;

    dpy = XOpenDisplay(NULL);

    nxvisuals = 0;
    visual_template.screen = DefaultScreen(dpy);
    visual_list = XGetVisualInfo (dpy, VisualScreenMask, &visual_template, &nxvisuals);

    //Change to this line
    //if (!XMatchVisualInfo(dpy, XDefaultScreen(dpy), 32, TrueColor, &vinfo))
    if (!XMatchVisualInfo(dpy, XDefaultScreen(dpy), 24, TrueColor, &vinfo))
    {
        fprintf(stderr, "no such visual\n");
        return;
    }

    parent = XDefaultRootWindow(dpy);

    XSync(dpy, True);

    printf("creating RGBA child\n");

    visual = vinfo.visual;
    depth = vinfo.depth;

    attrs.colormap = XCreateColormap(dpy, XDefaultRootWindow(dpy), visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;



    framebuf = (int *) malloc((SCREENWIDTH*SCREENHEIGHT)*4);

    for (i = 0; i < (SCREENWIDTH*SCREENHEIGHT); i++)
    {
        framebuf[i] = 0xFFFFFFFF;
    }

    win = XCreateWindow(dpy, parent, 100, 100, SCREENWIDTH, SCREENHEIGHT, 0, depth, InputOutput,
                        visual, CWBackPixel | CWColormap | CWBorderPixel, &attrs);

    //Change to this line
    //ximage = XCreateImage(dpy, vinfo.visual, depth, XYPixmap, 0, (char *)framebuf, SCREENWIDTH, SCREENHEIGHT, 8, SCREENWIDTH*4);
    ximage = XCreateImage(dpy, vinfo.visual, depth, ZPixmap, 0, (char *)framebuf, SCREENWIDTH, SCREENHEIGHT, 8, SCREENWIDTH*4);

    if (ximage == 0)
    {
        printf("ximage is null!\n");
    }

    XSync(dpy, True);

    XSelectInput(dpy, win, ExposureMask | KeyPressMask);

    XGCValues gcv;
    unsigned long gcm;

    //gcm = GCForeground | GCBackground | GCGraphicsExposures;
    //gcv.foreground = BlackPixel(dpy, parent);
    //gcv.background = WhitePixel(dpy, parent);
    gcm = GCGraphicsExposures;
    gcv.graphics_exposures = 0;
    NormalGC = XCreateGC(dpy, parent, gcm, &gcv);

    XMapWindow(dpy, win);

    // while(!XNextEvent(dpy, &event))
    // {
    //     switch(event.type)
    //     {
    //         case Expose:
    //             printf("I have been exposed!\n");
                
    //             break;
    //     }
    // }

    printf("No error\n");


}



int main(void)
{

    init_x();

    std::vector<struct shape *> shapes;

    //struct shape * shapes[MAXSHAPES];
    //int numShapes = 0;

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




    // shapes[numShapes] = &square1;
    // numShapes++;

    shapes.push_back(&square1);
    shapes.push_back(&square2);

    // shapes[numShapes] = &square2;
    // numShapes++;


    double buf[4][4];


    for (;;)
    {
        // Lock framerate to ~30 by timing and sleeping.
        auto start = std::chrono::high_resolution_clock::now();

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
        

        displayVertices(shapes, framebuf);

        // This function call is particularly slow.
        // TODO switch to shm
        XPutImage(dpy, win, NormalGC, ximage, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT);


        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << duration.count() << std::endl;

        usleep(duration.count() < 33000 ? 33000-duration.count() : 0);

    }

    return 0;
}









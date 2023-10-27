
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

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#include "./include/display.hpp"
#include "./include/utils.hpp"
#include "./include/transforms.hpp"

#include <curses.h>

#define MAXSHAPES 10

#define FRAMETIME 30000

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
XShmSegmentInfo shminfo;
XGCValues gcv;
unsigned long gcm;


static int HandleXError( Display *dpy, XErrorEvent *event )
{
    printf("errorrorororor");
    exit(1);
}

void init_x() 
{

    int i;

    dpy = XOpenDisplay(NULL);

    nxvisuals = 0;
    visual_template.screen = DefaultScreen(dpy);
    visual_list = XGetVisualInfo(dpy, VisualScreenMask, &visual_template, &nxvisuals);


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

    win = XCreateWindow(dpy, parent, 100, 100, SCREENWIDTH, SCREENHEIGHT, 0, depth, InputOutput,
                        visual, CWBackPixel | CWColormap | CWBorderPixel, &attrs);



    ximage = XShmCreateImage(dpy, vinfo.visual, depth, ZPixmap, NULL, &shminfo, SCREENWIDTH, SCREENHEIGHT); 

    shminfo.shmid = shmget(IPC_PRIVATE, ximage->bytes_per_line * ximage->height, IPC_CREAT|0777);
    shminfo.shmaddr = ximage->data = (char *)shmat(shminfo.shmid, 0, 0); 

    if (shminfo.shmaddr <= (char *) 0)
    {
        printf("error");
        exit(1);
    }


    shminfo.readOnly = False;

    int ErrorFlag = 0;
    XSetErrorHandler( HandleXError );

    

    
    XShmAttach(dpy, &shminfo);

    framebuf = (int *)shminfo.shmaddr;

    if (ximage == 0)
    {
        printf("ximage is null!\n");
        exit(1);
    }

    XSync(dpy, False);

    XSelectInput(dpy, win, ExposureMask | KeyPressMask);


       /* An error may still occur upon the first XShmPutImage.  So it's a */
   /* good idea to test it here.  However, we need a window to put the */
   /* image into, etc.... */
   GC gc = XCreateGC( dpy, win, 0, NULL );
   XShmPutImage( dpy, win, gc, ximage, 0, 0, 0, 0, 1, 1 /*one pixel*/, False );
   XSync( dpy, False );
   XFreeGC( dpy, gc );
   XSetErrorHandler( NULL );
   if (ErrorFlag) {
      XFlush( dpy );
      ErrorFlag = 0;
      XDestroyImage( ximage );
      shmdt( shminfo.shmaddr );
      shmctl( shminfo.shmid, IPC_RMID, 0 );
      exit(1);
   }




    gcm = GCGraphicsExposures;
    gcv.graphics_exposures = 0;
    NormalGC = XCreateGC(dpy, parent, gcm, &gcv);

    XMapWindow(dpy, win);
    XInternAtom(dpy, "WM_DELETE_WINDOW", False); 

    printf("No error\n");


}



int main(void)
{

    XEvent event;

    init_x();


    std::vector<struct shape *> shapes;

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

    shapes.push_back(&square1);
    shapes.push_back(&square2);


    for (;;)
    {
        
        // char input;
        // input = getch();
        // system("clear");

        //XNextEvent(dpy, &event);
        int ks;
        event.type = KeyPress;
        switch (event.type)
        {
            case KeyPress:
                ks = XK_e;//XLookupKeysym(&(event.xkey), 0);

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
                        break;
                    case XK_e:
                        rotateShapesCCW(&square1, center, a1);
                        rotateShapesCCW(&square2, center, a1);
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

        displayVertices(shapes, (int *)ximage->data);

        // XPutImage(dpy, win, NormalGC, ximage, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT);
        XShmPutImage(dpy, win, NormalGC, ximage, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT, false);
        XFlush(dpy);


        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Solution achieved " << (double)duration.count() / FRAMETIME << " efficiency." << std::endl;
        std::cout << "Slept for " << (double)duration.count() << " ms" << std::endl;

        usleep(duration.count() < FRAMETIME ? FRAMETIME-duration.count() : 0);

    }

    return 0;
}










#include <cstdlib>
#include <cstdio>

// X11 libraries
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#include "./../include/window.hpp"
#include "./../include/display.hpp"


static int HandleXError(Display *dpy, XErrorEvent *event)
{
    UNUSED(dpy);
    UNUSED(event);

    printf("X error.\n");

    exit(1);
}


void X11Window::init_x() 
{
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

    XSetErrorHandler(HandleXError);
    
    XShmAttach(dpy, &shminfo);

    if (ximage == 0)
    {
        printf("ximage is null!\n");
        exit(1);
    }

    XSync(dpy, False);

    XSelectInput(dpy, win, ExposureMask | KeyPressMask);

    gcm = GCGraphicsExposures;
    gcv.graphics_exposures = 0;
    NormalGC = XCreateGC(dpy, parent, gcm, &gcv);

    XMapWindow(dpy, win);
    XInternAtom(dpy, "WM_DELETE_WINDOW", False); 

    printf("No error\n");


}


int * X11Window::getFrameBuffer(void)
{
    return (int *)shminfo.shmaddr;
}


void X11Window::putImage(void)
{
    XShmPutImage(dpy, win, NormalGC, ximage, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT, false);
    XFlush(dpy);
}

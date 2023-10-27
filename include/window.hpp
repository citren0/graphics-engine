
#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>


class X11Window
{
    private:
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

    public:
        void init_x();
        int * getFrameBuffer(void);
        void putImage(void);
        Display * getDisplay(void);
};


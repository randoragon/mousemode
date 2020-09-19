#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <signal.h>

#define FREQUENCY 60

Display *dpy;
int running = 1;
int root;
char keys_return[32] = {0};
int mousespdl = 0;
int mousespdd = 0;
int mousespdu = 0;
int mousespdr = 0;
int mousepress1 = 0;
int mousepress2 = 0;
int mousepress3 = 0;
int mousescrollrate = 0;

int iskeydown(int keysym)
{
    KeyCode c = XKeysymToKeycode(dpy, keysym);
    return (keys_return[ c >> 3 ] & (1 << (c & 7)))? 1 : 0;
}

int handleKeys()
{
    /* References:
     * https://stackoverflow.com/a/49840783
     * https://pastebin.com/sk7FZ6AP
     */
    XQueryKeymap(dpy, keys_return);
    if (iskeydown(XK_q))
        return 0;
    if (iskeydown(XK_Super_L) || iskeydown(XK_Super_R))
        return 1;
    int isfast, isslow, isleft, isdown, isup, isright, ism1, ism2, ism3, ism4, ism5;
    isfast    = iskeydown(XK_f);
    isslow    = iskeydown(XK_c);
    isleft    = (iskeydown(XK_h) || iskeydown(XK_Left))?  1 : 0;
    isdown    = (iskeydown(XK_j) || iskeydown(XK_Down))?  1 : 0;
    isup      = (iskeydown(XK_k) || iskeydown(XK_Up))?    1 : 0;
    isright   = (iskeydown(XK_l) || iskeydown(XK_Right))? 1 : 0;
    ism1      = iskeydown(XK_a);
    ism2      = iskeydown(XK_s);
    ism3      = iskeydown(XK_d);
    ism4      = iskeydown(XK_y);
    ism5      = iskeydown(XK_e);
    mousespdl = isleft  * (isslow? 1 : 8) * (isfast? 2 : 1);
    mousespdd = isdown  * (isslow? 1 : 8) * (isfast? 2 : 1);
    mousespdu = isup    * (isslow? 1 : 8) * (isfast? 2 : 1);
    mousespdr = isright * (isslow? 1 : 8) * (isfast? 2 : 1);
    XWarpPointer(dpy, None, None, 0, 0, 0, 0, mousespdr - mousespdl, mousespdd - mousespdu);

    if (ism1 && !mousepress1) {
        XTestFakeButtonEvent(dpy, 1, True, CurrentTime);
        mousepress1 = 1;
    } else if (!ism1 && mousepress1) {
        XTestFakeButtonEvent(dpy, 1, False, CurrentTime);
        mousepress1 = 0;
    }
    if (ism2 && !mousepress2) {
        XTestFakeButtonEvent(dpy, 2, True, CurrentTime);
        mousepress2 = 1;
    } else if (!ism2 && mousepress1) {
        XTestFakeButtonEvent(dpy, 2, False, CurrentTime);
        mousepress2 = 0;
    }
    if (ism3 && !mousepress1) {
        XTestFakeButtonEvent(dpy, 3, True, CurrentTime);
        mousepress3 = 1;
    } else if (!ism3 && mousepress1) {
        XTestFakeButtonEvent(dpy, 3, False, CurrentTime);
        mousepress3 = 0;
    }
    if (!isslow) {
        if (ism4 && !ism5 && mousescrollrate-- <= 0) {
            XTestFakeButtonEvent(dpy, 4, True, CurrentTime);
            XTestFakeButtonEvent(dpy, 4, False, CurrentTime);
            mousescrollrate = (isfast? 1 : 2);
        } else if (!ism4 && ism5 && mousescrollrate-- <= 0) {
            XTestFakeButtonEvent(dpy, 5, True, CurrentTime);
            XTestFakeButtonEvent(dpy, 5, False, CurrentTime);
            mousescrollrate = (isfast? 1 : 2);
        } else if (!ism4 && !ism5) {
            mousescrollrate = 0;
        }
    } else {
        XTestFakeButtonEvent(dpy, 4, ism4? True : False, CurrentTime);
        XTestFakeButtonEvent(dpy, 5, ism5? True : False, CurrentTime);
    }
    XFlush(dpy);

    return 1;
}

void grabkey(int keysym)
{
    KeyCode code;
    if ((code = XKeysymToKeycode(dpy, keysym))) {
        XGrabKey(dpy, code, 0                             , root, True, GrabModeAsync, GrabModeAsync);
        XGrabKey(dpy, code, ShiftMask                     , root, True, GrabModeAsync, GrabModeAsync);
        XGrabKey(dpy, code, ControlMask                   , root, True, GrabModeAsync, GrabModeAsync);
        XGrabKey(dpy, code, Mod1Mask                      , root, True, GrabModeAsync, GrabModeAsync);
        XGrabKey(dpy, code, ShiftMask|ControlMask         , root, True, GrabModeAsync, GrabModeAsync);
    }
}

void termhandler(int signum)
{
    running = 0;
}

int main()
{
    signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);

    if (!(dpy = XOpenDisplay(NULL))) {
        fprintf(stderr, "mousemode: failed to open display");
        return 2;
    }

    root = RootWindow(dpy, DefaultScreen(dpy));
    {
        for (int i = XK_a; i <= XK_z; i++) {
            grabkey(i);
        }
        grabkey(XK_Left);
        grabkey(XK_Down);
        grabkey(XK_Up);
        grabkey(XK_Right);
    }

    while (running) {
        running &= handleKeys();
        usleep(1000000 / FREQUENCY);
    }

    XUngrabKey(dpy, AnyKey, AnyModifier, root);
    for (int i = 1; i <= 5; i++)
        XTestFakeButtonEvent(dpy, i, False, CurrentTime);
    XCloseDisplay(dpy);
    return 0;
}

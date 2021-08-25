#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <cstring>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;

  char color_vals[16][16]={"white", "black", "#DB3558", "#BDD9BF", "#878787", "#FFC857", "#7E7799", "#FFE4AB", "#FFF8DC", "firebrick", "Lavender", "#3A444D", "#D8DDE2", "mistyrose"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 16; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
    Pixmap newPix = XCreatePixmap(d,w,200,
          200,DefaultDepth(d,DefaultScreen(d)));
    GC newGC = XCreateGC(d, newPix, 0,(XGCValues *)0);
  XDrawString(d, w, newGC, x, y, msg.c_str(), msg.length());
}

void Xwindow::drawLine(int beginx, int beginy, int endx, int endy) {
    XDrawLine(d, w, gc, beginx, beginy, endx, endy);
}

void Xwindow::drawText(int x, int y, string text, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XFontStruct * font = XLoadQueryFont(d, "7x14");
    XTextItem ti[1];
    char carr[20];
    strcpy(carr, text.c_str());
    ti[0].chars = carr;
    ti[0].nchars = text.length();
    ti[0].delta = 0;
    ti[0].font = font->fid;
    XDrawText(d, w, gc, x, y, ti, 1);
    XUnloadFont(d, font->fid);
    XSetForeground(d, gc, colours[Black]);
    XFlush(d);
}

#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[16];

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.

    // Green - Aquamarine - I
    // Orange - Coral - L
    // Blue - CornFlower Blue - z
    // Pink - Misty Rose - o
    // Purple - Medium Violet Red - s
    // Yellow - Khaki - T
    // Red - Crimson - J
    // Brown - Fire Brick - Star
    
  enum {White=0, Black, Red, Green, Blue, Pink, Purple, Yellow, Orange, Brown, BackGround, Board, Text, Selected}; // Available colours.

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a string
  void drawString(int x, int y, std::string msg);
    
  // Draws a line
  void drawLine(int beginx, int beginy, int endx, int endy);

  void drawText(int x, int y, std::string text, int colour);

};

#endif

#ifndef graphicsdisplay_hpp
#define graphicsdisplay_hpp

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include "observer.h"
#include "window.h"

class GraphicsDisplay : public Observer {
    Xwindow xw;
    int row = 18;
    int col = 11;
    bool blinded = false;
    
    void init();
    void drawGrid(int player);
    void drawBoard(int player);
    void drawLevelnScore(int player);
    void drawNextBlock(int player);
    void printLevel(int player, int level);
    void printScore(int player, int score);
    void printByType(size_t x, size_t y, int width, int height, CellType type);
    void printBlockByType(CellType type, int currentPlayer, int which);
    
  public:
    GraphicsDisplay();
    ~GraphicsDisplay();
    void clear(int player);
    
    void notify(Grid& theGrid) override;
    void printGrid(Grid& theGrid) override;
    void blind() override;
    void unblind() override;
};

#endif /* graphicsdisplay_hpp */


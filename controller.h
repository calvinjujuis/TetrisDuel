#ifndef controller_hpp
#define controller_hpp

#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "grid.h"
#include "observer.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

class Controller {
    int result;     // result is 1 when only player 1 has lost;
                    // is 2 when only player 2 has lost
                    // 0 when no one has lost; 3 when both have lost
    int highestScore;
    std::shared_ptr<Grid> g1;
    std::shared_ptr<Grid> g2;
    std::shared_ptr<TextDisplay> td;
    std::shared_ptr<GraphicsDisplay> gd;
    
    std::shared_ptr<Grid> determinePlayer(int p);
    std::vector<CellType> convert(std::vector<std::string> playerBlocks);
    
    
    public:
    Controller(std::vector<std::string> player1blocks,
               std::vector<std::string> player2blocks,
               bool isTextOnly);
    void init(bool isTextOnly);
    int getResult();             // returns result
    void restart();              // clears the grids but keeps highestScore
    int getHighestScore();
    
    void moveLeft(int multiplier, int p);         //
    void moveRight(int multiplier, int p);        //
    void moveDown(int multiplier, int p);         //
    void clockwise(int multiplier, int p);        //
    void counterClockwise(int multiplier, int p); //
    int drop(int multiplier, int p);
    std::string hint(int multiplier, int p);
    void levelUp(int multiplier, int p);
    void levelDown(int multiplier, int p);
    void noRandom(int p);
    void restoreRandom(int p);      // sets random in grid to be true
    void changeCurrentBlock(int p, CellType type);// changes current undropped block to type
    
    void makeBlind(int p);
    void makeHeavy(int p);
    void makeJustDrop(int p);
    
    // bonus:
    bool selected(int p, int x, int y);
    void storeBlock(int p);
    
    friend std::ostream &operator<<(std::ostream &out, const Controller &ctrl);
    
};

#endif /* controller_hpp */

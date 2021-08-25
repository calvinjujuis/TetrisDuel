#ifndef grid_hpp
#define grid_hpp

#include <vector>
#include <memory>
#include <cstdlib>
#include "cell.h"
#include "nextblock.h"
#include "block.h"
#include "observer.h"

enum class CellType;

class Cell;

class Observer;

struct Point;

class Grid {
    const int row = 18;
    const int col = 11;
    const int player;
    int level = 0;
    int level4count = 0; // count how many turns are left until next star drop
    int level5count = 0;
    int score = 0;
    bool lost = false;
    bool random = true; // randomness regarding level 3 and 4
    int curNoRandomBlock = 0; // stores the currrent podition at level0Blocks
    
    std::shared_ptr<Block> curBlock;
    std::shared_ptr<Block> nextBlock;
    std::vector<CellType> Level0Blocks;
    std::vector<std::shared_ptr<Observer>> observers;
    std::vector<std::vector<Cell>> theGrid;
    
    // bonus:
    std::shared_ptr<Block> blockHolder;
    
    std::vector<Point> erased;

    void checkState();       // check if the player has lost
    bool checkFull(int row); // returns true if a row is full
    void dropOneRow(int row);
    int clearRow(); // return true of at least one row is cleared;
                     // (if at level4) if true, set level4count to 0;
                     //                if false, increment level4count
                     //                          and call dropStarBlock
    
    // Block
    void erase();   // turn off all cells in curBlock
    void update();  // update all cells in curBlock
    void heavyAction(); // move down one if block is heavy
    std::shared_ptr<Block> makeBlockFromFile(); // make Block from level0blocks
    
    // NextBlock
    std::shared_ptr<Block> getNextBlock();

    void dropStar();
    
    /*
     Level5
     */
    bool level5RandomPlace(int x, int y, CellType type);
    void level5RandomBlock();
    
public:
    Grid(int player);
    void init(); // initializes theGrid, first curBlock and nextBlock
    
    void clear();
    int getPlayer();
    int getRow();
    int getCol();
    int getLevel();
    int getScore();
    bool isLost();
    std::vector<std::vector<Cell>> &getGrid();
    std::shared_ptr<Block> getCurBlock();
    std::shared_ptr<Block> getnextBlock();
    std::vector<Point> getErased();
    
    void setLevel0(std::vector<CellType> blocks);
    void attachObserver(std::shared_ptr<Observer> obs);
    void notifyObservers();
    void observerPrintGrid();
    
    void moveLeft(int multiplier);           //
    void moveRight(int multiplier);          //
    void moveDown(int multiplier);               //
    void clockwise(int multiplier);          //
    void counterClockwise(int multiplier);   //
    int drop(int multiplier);                // returns the maximum # of rows                                             // cleared in one drop
    std::string hint();
    void levelUp(int multiplier);
    void levelDown(int multiplier);
    void noRandom();
    void restoreRandom();      // set random to be true
    void changeCurrentBlock(CellType type);// change current undropped block to type
    // add score when a block is completely cleared
    void blockCleared(int level);
    
    // special actions
    void blind();
    void makeBlind(); // blind observers
    void undoBlind(); // unblind observers
    void heavy();
    void justDrop();
    
    /*
     special Actions:
     */
    bool selected(int x, int y);
    
    /*
     block holder:
     */
    void storeBlock(); // store curBlock in blockHolder; swap if holder not empty
    std::shared_ptr<Block> getHolderBlock();
};

#endif /* grid_hpp */

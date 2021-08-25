#ifndef block_hpp
#define block_hpp

#include <vector>
#include "grid.h"

class Grid;
enum class CellType;


struct Point {
    int x;
    int y;
    bool state;
};

class Block {
  public:
    virtual void setHeavy() = 0;
    virtual bool getHeavy() = 0;
    virtual void unHeavy() = 0;
    virtual void down() = 0;                 // decrease all y coords of Points in theBlock
    virtual bool isdown() = 0;
    virtual void moveLeft() = 0;
    virtual void moveRight() = 0;
    virtual void drop() = 0;
    virtual void moveUp() = 0;
    
    virtual void clockwise() = 0;
    virtual void counterClockwise() = 0;
    
    virtual std::shared_ptr<Block> makeBlock(Grid &grid, CellType type) = 0;
    virtual void updateBoundaries() = 0;      // get updated bottom, left and right
    virtual int getBottom() = 0;
    virtual int getLeft() = 0;
    virtual int getRight() = 0;
    virtual CellType getType() = 0;
    virtual std::vector<Point> getPoints() = 0;
    virtual void addCount() = 0;
    virtual Grid & getGrid() = 0;
    
    virtual ~Block();
};

#endif


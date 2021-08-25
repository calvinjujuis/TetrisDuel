#ifndef baseblock_hpp
#define baseblock_hpp

#include "block.h"
#include <vector>

class Block;
enum class CellType;
struct Point;

class BaseBlock: public Block {
    std::vector<Point> theBlock;
    int up, bottom, left, right;  // y-coord, x-coord, x-coord
    CellType type;
    bool heavy;
    int level;
    int count = 0;
    bool downcheck;
protected:
    Grid & grid;
    
public:
    
    BaseBlock(Grid &grid, std::vector<Point> theBlock, int up, int bottom, int left, int right, CellType type);
    void setHeavy() override;
    void unHeavy() override;
    bool getHeavy() override;
    void down() override;                 // decrease all y coords of Points in theBlock
    bool isdown() override;
    void moveLeft() override;
    void moveRight() override;
    void drop() override;
    void moveUp() override;
    
    void clockwise() override;
    void counterClockwise() override;
    
    std::shared_ptr<Block> makeBlock(Grid &grid, CellType type) override;
    void updateBoundaries() override;      // get updated bottom, left and right
    int getBottom() override;
    int getLeft() override;
    int getRight() override;
    CellType getType() override;
    std::vector<Point> getPoints() override;
    void addCount() override;
    Grid & getGrid() override;
    
};

class IBlock : public BaseBlock {
  public:
    IBlock(Grid &grid);
};

class JBlock : public BaseBlock {
  public:
    JBlock(Grid &grid);
};

class LBlock : public BaseBlock {
  public:
    LBlock(Grid &grid);
};

class OBlock : public BaseBlock {
  public:
    OBlock(Grid &grid);
};

class SBlock : public BaseBlock {
  public:
    SBlock(Grid &grid);
};

class ZBlock : public BaseBlock {
  public:
    ZBlock(Grid &grid);
};

class TBlock : public BaseBlock {
  public:
    TBlock(Grid &grid);
};

class StarBlock : public BaseBlock {
  public:
    StarBlock(Grid &grid);
};

#endif /* baseblock_hpp */


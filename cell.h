#ifndef cell_hpp
#define cell_hpp
#include <memory>
#include "block.h"

// an enum class stroing all types of blocks :
enum class CellType { I, J, L, O, S, Z, T, Star, Selected};
    

class Block;

class Cell {
    bool state;
    CellType type;
    std::shared_ptr<Block> theBlock;
public:
    Cell(std::shared_ptr<Block> theBlock);
    bool getState();
    CellType getType();
    std::shared_ptr<Block> getBlock();
    void flip();
    void turnOff();
    void turnOn();
    void setType(CellType type);
    void setBlock(std::shared_ptr<Block> newBlock);
    void cleared(); // notifies theBlock that this cell has been cleared
};

#endif /* cell_hpp */

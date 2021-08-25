#ifndef decorator_hpp
#define decorator_hpp

#include "block.h"

class Decorator: public Block {
protected:
    std::shared_ptr<Block> component;
public:
    Decorator( std::shared_ptr<Block> component );
    virtual void down() override;                 // decrease all y coords of Points in theBlock
    virtual bool isdown() override;
    virtual void moveLeft() override;
    virtual void moveRight() override;
    virtual void moveUp() override;
    
    virtual void clockwise() override;
    virtual void counterClockwise() override;
    
    virtual std::shared_ptr<Block> makeBlock(Grid &grid, CellType type) override;
    virtual void updateBoundaries() override;     // get updated bottom, left and right
    virtual int getBottom() override;
    virtual int getLeft() override;
    virtual int getRight() override;
    virtual CellType getType() override;
    virtual std::vector<Point> getPoints() override;
    virtual void setHeavy() override;
    virtual void unHeavy() override;
    virtual bool getHeavy() override;
    virtual void drop() override;
    virtual void addCount() override;
    virtual Grid & getGrid() override;
    virtual ~Decorator();
    
};

#endif /* decorator_hpp */

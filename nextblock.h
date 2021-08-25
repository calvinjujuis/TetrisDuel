#ifndef nextblock_hpp
#define nextblock_hpp

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Block;
class Grid;
enum class CellType;

class NextBlock {
    
protected:
    int index = 0;
    CellType type;
public:
    virtual std::shared_ptr<Block> makeNextBlock(Grid &grid) = 0;
    CellType getType();
};


class Level0 : public NextBlock {
    std::vector<std::string> files;
public:
    Level0(std::vector<std::string> files);
    std::shared_ptr<Block> makeNextBlock(Grid &grid) override;
};

class Level1 : public NextBlock {
public:
    Level1();
    std::shared_ptr<Block> makeNextBlock(Grid &grid) override;
};

class Level2 : public NextBlock {
public:
    Level2();
    std::shared_ptr<Block> makeNextBlock(Grid &grid) override;
};

class Level3 : public NextBlock {
public:
    Level3();
    std::shared_ptr<Block> makeNextBlock(Grid &grid) override;
};

class Level4 : public NextBlock {
public:
    Level4();
    std::shared_ptr<Block> makeNextBlock(Grid &grid) override;
};

class Level5 : public NextBlock {
public:
    Level5();
    std::shared_ptr<Block> makeNextBlock(Grid &grid) override;
};



#endif


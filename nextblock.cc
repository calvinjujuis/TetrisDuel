#include "nextblock.h"
#include "baseblock.h"
#include "cell.h"
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
using namespace std;


std::shared_ptr<Block> makeBlock(Grid &grid, CellType type) {
    switch(type){
        case CellType::I: {
            return std::make_shared<IBlock>(grid);
            break;
        }
        case CellType::J:
            return std::make_shared<JBlock>(grid);
            break;
        case CellType::L:
            return std::make_shared<LBlock>(grid);
            break;
        case CellType::O:
            return std::make_shared<OBlock>(grid);
            break;
        case CellType::S:
            return std::make_shared<SBlock>(grid);
            break;
        case CellType::Z:
            return std::make_shared<ZBlock>(grid);
            break;
        case CellType::T:
            return std::make_shared<TBlock>(grid);
            break;
        case CellType::Star:
            return std::make_shared<SBlock>(grid);
            break;
        default:
            return nullptr;
            break;
    }
}


CellType NextBlock::getType() {
    return type;
}

Level0::Level0(vector<string> files): files{files}{}

std::shared_ptr<Block> Level0::makeNextBlock(Grid &grid) {
    if (index == (int)files.size()) {
        index = 0;
    }
    string file_type = files[index];
    if (file_type == "I") {
        type = CellType::I;
    } else if (file_type == "J") {
        type = CellType::I;
    } else if (file_type == "L") {
        type = CellType::J;
    } else if (file_type == "O") {
        type = CellType::O;
    } else if (file_type == "S") {
        type = CellType::S;
    } else if (file_type == "Z") {
        type = CellType::Z;
    } else if (file_type == "T") {
        type = CellType::T;
    }
    index++;
    std::shared_ptr<Block> returnBlock = makeBlock(grid,type);
    return returnBlock;
}

Level1::Level1(){};

std::shared_ptr<Block> Level1::makeNextBlock(Grid &grid) {
    int block = rand() % 6;
    if (block == 0){  //1/12
        if(rand() % 2 == 0){
            type = CellType::S;
        }else{
            type = CellType::Z;
        }
    }else if (block == 1){
        type = CellType::I;
    }else if (block == 2){
        type = CellType::J;
    }else if (block == 3){
        type = CellType::L;
    }else if (block == 4){
        type = CellType::O;
    }else if (block == 5){
        type = CellType::T;
    }
    std::shared_ptr<Block> returnBlock = makeBlock(grid,type);
    return returnBlock;

}

Level2::Level2(){};

std::shared_ptr<Block> Level2::makeNextBlock(Grid &grid) {
    int block = rand() % 7;
    if (block == 0){
        type = CellType::Z;
    }else if (block == 1){
        type = CellType::I;
    }else if (block == 2){
        type = CellType::J;
    }else if (block == 3){
        type = CellType::L;
    }else if (block == 4){
        type = CellType::O;
    }else if (block == 5){
        type = CellType::S;
    }else if (block == 6){
        type = CellType::T;
    }
    std::shared_ptr<Block> returnBlock = makeBlock(grid,type);
    return returnBlock;
}

Level3::Level3(){};

std::shared_ptr<Block> Level3::makeNextBlock(Grid &grid) {
    int block = rand() % 9;
    if (block == 0 || block == 1){ // 2/9
        type = CellType::S;
    }else if (block == 2 || block == 3){
        type = CellType::Z;
    }else if (block == 4){
        type = CellType::I;
    }else if (block == 5){
        type = CellType::J;
    }else if (block == 6){
        type = CellType::L;
    }else if (block == 7){
        type = CellType::O;
    }else if (block == 8){
        type = CellType::T;
    }
    std::shared_ptr<Block> returnBlock = makeBlock(grid,type);
    returnBlock->setHeavy();
    return returnBlock;
}


Level4::Level4(){};

std::shared_ptr<Block> Level4::makeNextBlock(Grid &grid) {
    int block = rand() % 9;
    if (block == 0 || block == 1){ // 2/9
        type = CellType::S;
    }else if (block == 2 || block == 3){
        type = CellType::Z;
    }else if (block == 4){
        type = CellType::I;
    }else if (block == 5){
        type = CellType::J;
    }else if (block == 6){
        type = CellType::L;
    }else if (block == 7){
        type = CellType::O;
    }else if (block == 8){
        type = CellType::T;
    }
    std::shared_ptr<Block> returnBlock = makeBlock(grid,type);
    returnBlock->setHeavy();
    return returnBlock;
}

Level5::Level5(){};

std::shared_ptr<Block> Level5::makeNextBlock(Grid &grid) {
    int block = rand() % 9;
    if (block == 0 || block == 1){ // 2/9
        type = CellType::S;
    }else if (block == 2 || block == 3){
        type = CellType::Z;
    }else if (block == 4){
        type = CellType::I;
    }else if (block == 5){
        type = CellType::J;
    }else if (block == 6){
        type = CellType::L;
    }else if (block == 7){
        type = CellType::O;
    }else if (block == 8){
        type = CellType::T;
    }
    std::shared_ptr<Block> returnBlock = makeBlock(grid,type);
    returnBlock->setHeavy();
    return returnBlock;
}

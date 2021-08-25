#include "baseblock.h"
#include <vector>
#include <memory>
using namespace std;

BaseBlock::BaseBlock(Grid &grid, vector<Point> theBlock, int up, int bottom, int left, int right,
                     CellType type):
    theBlock{theBlock},
    up{up},
    bottom{bottom},
    left{left},
    right{right},
    type{type},
    level{grid.getLevel()},
    grid{grid}
{
    if (grid.getLevel() == 3 || grid.getLevel() == 4) {
        heavy = true;
    } else {
        heavy = false;
    }
}

// IMPORTANT: THE FUNCTIONS BELOW DOES NOT CHECK VALIDITY

void BaseBlock::setHeavy() {
    heavy = true;
}

void BaseBlock::unHeavy() {
    heavy = false;
}

bool BaseBlock::getHeavy() {
    return heavy;
}

void BaseBlock::updateBoundaries() { // get updated bottom, left and right
    // update top
    int temp_top = theBlock[0].y;
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].y < temp_top) {
            temp_top = theBlock[i].y;
        }
    }
    this->up = temp_top;
    // update bottom
    int temp_bottom = theBlock[0].y;
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].y > temp_bottom) {
            temp_bottom = theBlock[i].y;
        }
    }
    this->bottom = temp_bottom;
    // update left
    int temp_left = theBlock[0].x;
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].x < temp_left) {
            temp_left = theBlock[i].x;
        }
    }
    this->left = temp_left;
    // update right
    int temp_right = theBlock[0].x;
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].x > temp_right) {
            temp_right = theBlock[i].x;
        }
    }
    this->right = temp_right;
}

void BaseBlock::moveUp() {
    bool goUp = true;
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].y - 1 < 0 || (theBlock[i].state && grid.getGrid()[theBlock[i].y - 1][theBlock[i].x].getState())) {
            goUp = false;
            break;
        }
    }
    if (goUp) {
        for (size_t i = 0; i < theBlock.size(); i++) {
            theBlock[i].y--;
        }
        this->up -= 1;
        this->bottom -= 1;
    }
    updateBoundaries();
}


void BaseBlock::down() {  // decrease all y coords of Points in theBlock
    bool down = true;
    int row = grid.getRow();
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].y + 1 >= row || (theBlock[i].state && grid.getGrid()[theBlock[i].y + 1][theBlock[i].x].getState())) {
            down = false;
            break;
        }
    }
    if (down) {
        for (size_t i = 0; i < theBlock.size(); i++) {
            theBlock[i].y++;
        }
        this->up += 1;
        this->bottom += 1;
    }
    downcheck = down;
}

bool BaseBlock::isdown() {
    return downcheck;
}

void BaseBlock::drop() {
    int row = grid.getRow();
    int maxdown = row - bottom;
    for (int i = 0; i < maxdown; i++) {
        down();
    }
}

void BaseBlock::moveLeft() {
    bool left = true;
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].x - 1 < 0 || (theBlock[i].state && grid.getGrid()[theBlock[i].y][theBlock[i].x - 1].getState())) {
            left = false;
            break;
        }
    }
    if (left) {
        for (size_t i = 0; i < theBlock.size(); i++) {
            theBlock[i].x--;
        }
        this->left -= 1;
        this->right -= 1;
    }
}

void BaseBlock::moveRight() {
    bool right = true;
    int col = grid.getCol();
    for (size_t i = 0; i < theBlock.size(); i++) {
        if (theBlock[i].x + 1 >= col || (theBlock[i].state && grid.getGrid()[theBlock[i].y][theBlock[i].x + 1].getState())) {
            right = false;
            break;
        }
    }
    if (right) {
        for (size_t i = 0; i < theBlock.size(); i++) {
            theBlock[i].x++;
        }
        this->right += 1;
        this->left += 1;
    }
}

// Note, O shape does not change after rotation
void BaseBlock::clockwise() {
    std::vector <Point> newCoord;
    // Note O does not change its state
    if (right - left == 1 && up - bottom == 1) {
        return;
    }
    // First we need to know the direction of 2 X 3 rectangular or 2 X 4 rectangular
    if (right - left == 2) { // horizonrtal 2 X 3 direction
        std::vector <bool> temp_state;
        for (size_t i = 0; i < theBlock.size(); i++) {
            temp_state.emplace_back(theBlock[i].state);
        }
        // starting from 2 X 4 vertical left corner
        bool temp;
        temp = temp_state[3];
        // coord for top left corner
        int x_r = theBlock[3].x;
        int y_r = theBlock[3].y - 2;
        Point newpoint = {x_r, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[0];
        newpoint = {x_r + 1, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[4];
        newpoint = {x_r, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[1];
        newpoint = {x_r + 1, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[5];
        newpoint = {x_r, y_r + 2, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[2];
        newpoint = {x_r + 1, y_r + 2, temp};
        newCoord.emplace_back(newpoint);
    } else if (right - left == 1) { // vertical 2 X 3 direction
        std::vector <bool> temp_state;
        for (size_t i = 0; i < theBlock.size(); i++) {
            temp_state.emplace_back(theBlock[i].state);
        }
        bool temp;
        temp = temp_state[4];
        int x_r = theBlock[4].x;
        int y_r = theBlock[4].y - 1;
        Point newpoint = {x_r, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[2];
        newpoint = {x_r + 1, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[0];
        newpoint = {x_r + 2, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[5];
        newpoint = {x_r, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[3];
        newpoint = {x_r + 1, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[1];
        newpoint = {x_r + 2, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
    } else if (right - left == 3) { // horinzontal 1 X 4 rectangular  XXXX
        // ALL TRUE
        // lowerleft
        int x_r = theBlock[0].x;
        int y_r = theBlock[0].y;
        Point newpoint = {x_r, y_r - 3, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r, y_r - 2, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r, y_r - 1, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r, y_r, true};
        newCoord.emplace_back(newpoint);
    } else if (right == left && bottom - up == 3) { // vertical 1 X 4 rectangular
        // ALL TRUE
        int x_r = theBlock[3].x;
        int y_r = theBlock[3].y;
        Point newpoint = {x_r, y_r, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r + 1, y_r, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r + 2, y_r, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r + 3, y_r, true};
        newCoord.emplace_back(newpoint);
    } // else we do nothing for the O shape
    // now we go through the newCoord list to c if all works
    bool crotate = true;
    for (size_t i = 0; i < newCoord.size(); i++) {
        int check_x = newCoord[i].x;
        int check_y = newCoord[i].y;
        // if out of bound
        if (check_x < 0 || check_x >= grid.getCol() || check_y < 0 || check_y >= grid.getRow()) {
            crotate = false;
            break;
        }
        // if the place was taken
        if (grid.getGrid()[check_y][check_x].getState()) {
            crotate = false;
            break;
        }
    }
    if (crotate) {
        theBlock = newCoord;
        updateBoundaries();
    }
}

void BaseBlock::counterClockwise() {
    std::vector <Point> newCoord;
    // First we need to know the direction of 2 X 3 rectangular or 1 X 4 rectangular
    // NOTE O does not change its shape
    if (right - left == 1 && bottom - up == 1) {
        return;
    }
    if (right - left == 2) { // horizonrtal 2 X 3 direction
        std::vector <bool> temp_state;
        for (size_t i = 0; i < theBlock.size(); i++) {
            temp_state.emplace_back(theBlock[i].state);
        }
        // starting from 2 X 3 vertical left corner
        bool temp;
        temp = temp_state[2];
        // coord for top left corner
        int x_r = theBlock[2].x - 2;
        int y_r = theBlock[2].y - 1;
        Point newpoint = {x_r, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[5];
        newpoint = {x_r + 1, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[1];
        newpoint = {x_r, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[4];
        newpoint = {x_r + 1, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[0];
        newpoint = {x_r, y_r + 2, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[3];
        newpoint = {x_r + 1, y_r + 2, temp};
        newCoord.emplace_back(newpoint);
    } else if (right - left == 1) { // vertical 2 X 3 direction
        std::vector <bool> temp_state;
        for (size_t i = 0; i < theBlock.size(); i++) {
            temp_state.emplace_back(theBlock[i].state);
        }
        bool temp;
        temp = temp_state[1];
        int x_r = theBlock[1].x - 1;
        int y_r = theBlock[1].y + 1;
        Point newpoint = {x_r, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[3];
        newpoint = {x_r + 1, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[5];
        newpoint = {x_r + 2, y_r, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[0];
        newpoint = {x_r, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[2];
        newpoint = {x_r + 1, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
        
        temp = temp_state[4];
        newpoint = {x_r + 2, y_r + 1, temp};
        newCoord.emplace_back(newpoint);
    } else if (right - left == 3) { // horinzontal 1 X 4 rectangular  XXXX
        // ALL TRUE
        // lowerleft
        int x_r = theBlock[0].x;
        int y_r = theBlock[0].y;
        Point newpoint = {x_r, y_r - 3, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r, y_r - 2, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r, y_r - 1, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r, y_r, true};
        newCoord.emplace_back(newpoint);
    } else if (right == left && bottom - up == 3) { // vertical 1 X 4 rectangular
        // ALL TRUE
        // ALL TRUE
        int x_r = theBlock[3].x;
        int y_r = theBlock[3].y;
        Point newpoint = {x_r, y_r, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r + 1, y_r, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r + 2, y_r, true};
        newCoord.emplace_back(newpoint);
        newpoint = {x_r + 3, y_r, true};
        newCoord.emplace_back(newpoint);
    } // else we do nothing for the O shape
    bool ccrotate = true;
    for (size_t i = 0; i < newCoord.size(); i++) {
        int check_x = newCoord[i].x;
        int check_y = newCoord[i].y;
        // if out of bound
        if (check_x < 0 || check_x >= grid.getCol() || check_y < 0 || check_y >= grid.getRow()) {
            ccrotate = false;
            break;
        }
        // if the place was taken
        if (grid.getGrid()[check_y][check_x].getState()) {
            ccrotate = false;
            break;
        }
    }
    if (ccrotate) {
        theBlock = newCoord;
        updateBoundaries();
    }
}


std::shared_ptr<Block> BaseBlock::makeBlock(Grid &grid, CellType type) {
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
        case CellType::Selected:
            return nullptr;
            break;
    }
    return nullptr;
}


int BaseBlock::getBottom() {
    return bottom;
}

int BaseBlock::getLeft()  {
    return left;
}


int BaseBlock::getRight() {
    return right;
}

CellType BaseBlock::getType() {
    return type;
}

std::vector<Point> BaseBlock::getPoints() {
    return theBlock;
}


void BaseBlock::addCount() {
    count++;
    if (count == 4) {
        grid.blockCleared(level);
    }
}

Grid & BaseBlock::getGrid(){
    return grid;
}




IBlock:: IBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 3, true}, Point{1, 3, true}, Point{2, 3, true}, Point{3, 3, true}},
          3, 3, 0, 3, CellType::I) {}

JBlock:: JBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 2, true}, Point{1, 2, false},Point{2, 2, false},
                             Point{0, 3, true}, Point{1, 3, true}, Point{2, 3, true}},
          2, 3, 0, 2, CellType::J) {}

LBlock:: LBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 2, false},Point{1, 2, false}, Point{2, 2, true},
                             Point{0, 3, true}, Point{1, 3, true}, Point{2, 3, true}},
          2, 3, 0, 2, CellType::L){}

OBlock:: OBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 2, true}, Point{0, 3, true},
                             Point{1, 2, true}, Point{1, 3, true}},
          2, 3, 0, 1, CellType::O) {}

SBlock:: SBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 2, false}, Point{1, 2, true}, Point{2, 2, true},
                             Point{0, 3, true}, Point{1, 3, true},Point{2, 3, false}},
          2, 3, 0, 2, CellType::S){}

ZBlock:: ZBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 2, true}, Point{1, 2, true}, Point{2, 2, false},
                             Point{0, 3, false}, Point{1, 3, true}, Point{2, 3, true}},
          2, 3, 0, 2, CellType::Z) {}

TBlock:: TBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{0, 2, true}, Point{1, 2, true}, Point{2, 2, true},
                             Point{0, 3, false}, Point{1, 3, true}, Point{2, 3, false}},
          2, 3, 0, 2, CellType::T){}

StarBlock:: StarBlock(Grid &grid):
BaseBlock(grid, std::vector<Point>{Point{5, 3, true}},
          3, 3, 5, 5, CellType::Star){}

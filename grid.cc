#include "grid.h"
#include "decorator.h"
#include "baseblock.h"
#include "SpecialActions.h"
using namespace std;

class Block;
enum class CellType;

const int maxWidthMove = 10;
const int maxHeightMove = 14;
const int maxLevel = 5;
const int minLevel = 0;
const int reserveRows = 3;

Grid::Grid(int player) :
    player{player}, level{0}, score{0},
    curBlock{nullptr}, nextBlock{nullptr}, blockHolder{nullptr} {}

void Grid::init() {
    // initilize theGrid
    for (int i = 0; i < row; ++i) {
        vector<Cell> oneRow;
        theGrid.push_back(oneRow);
        for (int j = 0; j < col; ++j) {
            Cell oneCell{nullptr};
            theGrid[i].push_back(oneCell);
        }
    }
    // initialize current and next block
    curBlock = makeBlockFromFile();
    nextBlock = makeBlockFromFile();
    
    update();
    observerPrintGrid();
}

void Grid::checkState() {
    vector<Cell> &checkRow = theGrid[2];
    for (int i = 0; i < col; ++i) {
        if (checkRow[i].getState() == true) { // if some cell has exceeded
            lost = true;
            break;
        }
    }
    // check if next block can be generated
    switch (nextBlock->getType()) {
        case CellType::I:
            if (theGrid[3][0].getState() ||
                theGrid[3][1].getState() ||
                theGrid[3][2].getState() ||
                theGrid[3][3].getState()) lost = true;
            break;
        case CellType::J:
            if (theGrid[3][0].getState() ||
                theGrid[3][1].getState() ||
                theGrid[3][2].getState()) lost = true;
            break;
        case CellType::L:
            if (theGrid[3][0].getState() ||
                theGrid[3][1].getState() ||
                theGrid[3][2].getState()) lost = true;
            break;
        case CellType::O:
            if (theGrid[3][0].getState() ||
                theGrid[3][1].getState()) lost = true;
            break;
        case CellType::S:
            if (theGrid[3][0].getState() ||
                theGrid[3][1].getState()) lost = true;
            break;
        case CellType::Z:
            if (theGrid[3][1].getState() ||
                theGrid[3][2].getState()) lost = true;
            break;
        case CellType::T:
            if (theGrid[3][1].getState()) lost = true;
            break;
        default:
            break;
    }
}

void Grid::dropOneRow(int row) {
    // copy all cells in row - 1 to row
    for (int i = 0; i < col; ++i) {
        Cell &theCell = theGrid[row][i];
        Cell &cellCopied = theGrid[row - 1][i];
        theCell.cleared();
        theCell.setType(cellCopied.getType());
        theCell.setBlock(cellCopied.getBlock());
        cellCopied.setBlock(nullptr);
        if (theCell.getState() != theGrid[row - 1][i].getState()) {
            theCell.flip();
        }
    }
}

bool Grid::checkFull(int row) {
    for (int i = 0; i < col; ++i) {
        if (theGrid[row][i].getState() == false) return false;
    }
    return true;
}

int Grid::clearRow() {
    int clearedRows = 0;
    // loop thru every row in Grid and clear rows that are full
    for (int i = 0; i < row; ++i) {
        if (checkFull(i)) { // if a row is full
            ++clearedRows;
            // clear this row and move all rows above it
            for (int j = i; j >= reserveRows; --j) {
                dropOneRow(j);
            }
        }
    }
    if (clearedRows > 0) {
        score += (level + clearedRows) * (level + clearedRows);
    }
    return clearedRows;
}

void Grid::erase() {
    vector<Point> points = curBlock->getPoints();
    erased = points;
    size_t numofPoints = points.size();
    for (size_t i = 0; i < numofPoints; ++i) {
        Point thePoint = points[i];
        if (thePoint.state) {
            Cell &theCell = theGrid[thePoint.y][thePoint.x];
            // set each cell corresponding to the curBlock to off:
            theCell.turnOff();
        }
    }
}

void Grid::update() {
    CellType type = curBlock->getType();
    vector<Point> points = curBlock->getPoints();
    size_t numofPoints = points.size();
    for (size_t i = 0; i < numofPoints; ++i) {
        Point thePoint = points[i];
        Cell &theCell = theGrid[thePoint.y][thePoint.x];
        theCell.setBlock(curBlock);
        // set each cell corresponding to the curBlock to on:
        if (thePoint.state == true && theCell.getState() == false) {
            theCell.flip();
        }
        if (thePoint.state) {
            theCell.setType(type);
        }
    }
}

void Grid::heavyAction() {
    if (curBlock->getHeavy()) {
        curBlock->down();
    }
}

shared_ptr<Block> Grid::getNextBlock() {
    shared_ptr<Block> newBlock;
    // get the type of next block
    shared_ptr<NextBlock> newNextBlock;
    if (level == 1) {
        newNextBlock = make_shared<Level1>();
    } else if (level == 2) {
        newNextBlock = make_shared<Level2>();
    } else if (level == 3 && random) {
        newNextBlock = make_shared<Level3>();
    } else if (level == 4 && random) {
        newNextBlock = make_shared<Level4>();
    } else if (level == 5 && random) {
        newNextBlock = make_shared<Level5>();
    } else { // level 0 or no random
        newBlock = makeBlockFromFile();
        if (level == 3 || level == 4) {
            newBlock->setHeavy();
        }
        return newBlock;
    }
    newBlock = newNextBlock->makeNextBlock(*this);

    return newBlock;
}

shared_ptr<Block> Grid::makeBlockFromFile() {
    if (curNoRandomBlock == (int)Level0Blocks.size()) {
        curNoRandomBlock = 0;
    }
    shared_ptr<Block> returnBlock = shared_ptr<Block>();
    switch(Level0Blocks[curNoRandomBlock]){
        case CellType::I:
            returnBlock = std::make_shared<IBlock>(*this);
            break;
        case CellType::J:
            returnBlock = std::make_shared<JBlock>(*this);
            break;
        case CellType::L:
            returnBlock = std::make_shared<LBlock>(*this);
            break;
        case CellType::O:
            returnBlock = std::make_shared<OBlock>(*this);
            break;
        case CellType::S:
            returnBlock = std::make_shared<SBlock>(*this);
            break;
        case CellType::Z:
            returnBlock = std::make_shared<ZBlock>(*this);
            break;
        case CellType::T:
            returnBlock = std::make_shared<TBlock>(*this);
            break;
        default:
            break;
    }
    ++curNoRandomBlock;
    return returnBlock;
}


void Grid::clear() {
    curBlock = nullptr;
    nextBlock = nullptr;
    erased.clear();
    blockHolder = nullptr;
    theGrid.clear();
    level = 0;
    level4count = 0;
    level5count = 0;
    score = 0;
    lost = false;
    random = true;
    curNoRandomBlock = 0;
    init();
}

int Grid::getPlayer() { return player; }

int Grid::getRow() { return row; }

int Grid::getCol() { return col; }

int Grid::getLevel() { return level; }

int Grid::getScore() { return score; }

bool Grid::isLost() { return lost; }

vector<vector<Cell>> &Grid::getGrid() { return theGrid; }

shared_ptr<Block> Grid::getCurBlock() { return curBlock; }

shared_ptr<Block> Grid::getnextBlock() { return nextBlock; }

vector<Point> Grid::getErased() { return erased; }

void Grid::setLevel0(std::vector<CellType> blocks) {
    Level0Blocks.clear();
    size_t blocksLen = blocks.size();
    for (size_t i = 0; i < blocksLen; ++i) {
        Level0Blocks.push_back(blocks[i]);
    }
}

void Grid::attachObserver(shared_ptr<Observer> obs) {
    observers.push_back(obs);
}

void Grid::notifyObservers() {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->notify(*this);
    }
}

void Grid::observerPrintGrid() {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->printGrid(*this);
    }
}

void Grid::moveLeft(int multiplier) {
    erase();
    if (multiplier > maxWidthMove) multiplier = maxWidthMove;
    for (int count = 0; count < multiplier; ++count) {
        curBlock->moveLeft();
    }
    heavyAction();
    update();
    notifyObservers();
}

void Grid::moveRight(int multiplier) {
    erase();
    if (multiplier > maxWidthMove) multiplier = maxWidthMove;
    for (int count = 0; count < multiplier; ++count) {
        curBlock->moveRight();
    }
    heavyAction();
    update();
    notifyObservers();
}

void Grid::moveDown(int multiplier) {
    erase();
    if (multiplier > maxHeightMove) multiplier = maxHeightMove;
    for (int count = 0; count < multiplier; ++count) {
        curBlock->down();
    }
    heavyAction();
    update();
    notifyObservers();
}

void Grid::clockwise(int multiplier) {
    erase();
    for (int count = 0; count < multiplier; ++count) {
        curBlock->clockwise();
    }
    heavyAction();
    update();
    notifyObservers();
}

void Grid::counterClockwise(int multiplier){
    erase();
    for (int count = 0; count < multiplier; ++count) {
        curBlock->counterClockwise();
    }
    heavyAction();
    update();
    notifyObservers();
}

void Grid::dropStar() {
    shared_ptr<Block> returnBlock = shared_ptr<Block>();
    returnBlock = std::make_shared<StarBlock>(*this);
    returnBlock->drop();
    vector<Point> starpoint = returnBlock->getPoints();
    Cell & starcell = theGrid[starpoint[0].y][starpoint[0].x];
    starcell.turnOn();
    starcell.setType(CellType::Star);
    clearRow();
    checkState();
    notifyObservers();
    observerPrintGrid();
}



int Grid::drop(int multiplier) {
    int rowsCleared = 0;
    for (int count = 0; count < multiplier; ++count) {
        erase();
        curBlock->drop();
        update();
        int temp = clearRow();
        if (temp > rowsCleared) rowsCleared = temp;
        if (temp > 0) {
            level4count = 0;
        } else if (level == 4) {
            ++level4count;
        } else if (level == 5) {
            ++level5count;
        }
        if (level == 4 && level4count != 0 && level4count % 5 == 0) {
            dropStar();
        }
        if (level == 5 && level5count != 0 && level5count % 5 == 0) {
            level5RandomBlock();
        }
        notifyObservers();
        checkState();
        if (lost) break;
        curBlock = nextBlock;
        if (level >= 3) {
            curBlock->setHeavy();
        } else {
            curBlock->unHeavy();
        }
        nextBlock = getNextBlock();
        update();
        notifyObservers();
    }
    observerPrintGrid();
    return rowsCleared;
}

string Grid::hint() {
    int oldLeft = curBlock->getLeft();
    int oldRight = curBlock->getRight();
    int oldBottom = curBlock->getBottom();
    int lowestBottom = 0;
    int position = 0;
    int right = 0;
    int left = 0;
    vector<Point> oldErased = erased;
    erase();
    while (oldRight + right < col) {
        if (right > 0) curBlock->moveRight();
        curBlock->drop();
        if (curBlock->getBottom() > lowestBottom) {
            lowestBottom = curBlock->getBottom();
            position = right;
        }
        while (true) {
            if (curBlock->getBottom() == oldBottom) break;
            curBlock->moveUp();
        }
        ++right;
    }
    while (right > 0) { // move block back to its initial position
        curBlock->moveLeft();
        --right;
    }
    while (oldLeft - left > 0) {
        if (left > 0) curBlock->moveLeft();
        curBlock->drop();
        if (curBlock->getBottom() > lowestBottom) {
            lowestBottom = curBlock->getBottom();
            left = -left;
            position = left;
            left = -left;
        }
        while (true) {
            if (curBlock->getBottom() == oldBottom) break;
            curBlock->moveUp();
        }
        ++left;
    }
    while (left > 0) { // move block back to its initial position
        curBlock->moveRight();
        --left;
    }
    string hintMsg;
    if (position == 0) { // drop directly
        hintMsg += "Drop directly!";
    } else if (position > 0) { // move right and drop
        hintMsg += "Move " + to_string(position) + " right, then drop!";
    } else if (position < 0) { // move left then drop
        position = -position;
        hintMsg += "Move " + to_string(position) + " left, then drop!";
    }
    erased = oldErased;
    update();
    notifyObservers();
    return hintMsg;
}

void Grid::levelUp(int multiplier) {
    level += multiplier;
    if (level > maxLevel) level = maxLevel;
    notifyObservers();
}

void Grid::levelDown(int multiplier) {
    level -= multiplier;
    if (level < minLevel) level = minLevel;
    notifyObservers();
}

void Grid::noRandom() {
    random = false;
}

void Grid::restoreRandom() {
    random = true;
}

void Grid::changeCurrentBlock(CellType type) {
    erase();
    curBlock = curBlock->makeBlock(*this, type);
    update();
    notifyObservers();
}

void Grid::blockCleared(int level) {
    score += (level + 1) * (level + 1);
}

void Grid::blind() {
    curBlock = make_shared<Blind>(curBlock);
    makeBlind();
}

void Grid::heavy() {
    curBlock = make_shared<Heavy>(curBlock);
}

void Grid::makeBlind() {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->blind();
    }
    notifyObservers();
}

void Grid::undoBlind() {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->unblind();
    }
    notifyObservers();
}

void Grid::justDrop() {
    curBlock = make_shared<JustDrop>(curBlock);
}


bool Grid::selected(int x, int y) {
    // if out-of-bound
    if (x > 10 || x < 0 || y > 17 || y < 0) {
        return false;
    }
    // if its on extra rows
    if (y <= 2 && y >= 0) {
        return false;
    }
    // if the spot is already taken
    if (theGrid[y][x].getState()) {
        return false;
    }
    theGrid[y][x].turnOn();
    theGrid[y][x].setType(CellType::Selected);
    clearRow();
    observerPrintGrid();
    return true;
}

void Grid::storeBlock() {
    CellType curType = curBlock->getType();
    if (blockHolder == nullptr) { // no blocks are stored
        switch (curType) {
            case CellType::I:
                blockHolder = std::make_shared<IBlock>(*this);
                break;
            case CellType::J:
                blockHolder = std::make_shared<JBlock>(*this);
                break;
            case CellType::L:
                blockHolder = std::make_shared<LBlock>(*this);
                break;
            case CellType::O:
                blockHolder = std::make_shared<OBlock>(*this);
                break;
            case CellType::S:
                blockHolder = std::make_shared<SBlock>(*this);
                break;
            case CellType::Z:
                blockHolder = std::make_shared<ZBlock>(*this);
                break;
            case CellType::T:
                blockHolder = std::make_shared<TBlock>(*this);
                break;
            default:
                break;
        }
        erase();
        curBlock = nextBlock;
        nextBlock = getNextBlock();
    } else { // swap blocks if not empty
        CellType holderType = blockHolder->getType();
        switch (curType) {
            case CellType::I:
                blockHolder = std::make_shared<IBlock>(*this);
                break;
            case CellType::J:
                blockHolder = std::make_shared<JBlock>(*this);
                break;
            case CellType::L:
                blockHolder = std::make_shared<LBlock>(*this);
                break;
            case CellType::O:
                blockHolder = std::make_shared<OBlock>(*this);
                break;
            case CellType::S:
                blockHolder = std::make_shared<SBlock>(*this);
                break;
            case CellType::Z:
                blockHolder = std::make_shared<ZBlock>(*this);
                break;
            case CellType::T:
                blockHolder = std::make_shared<TBlock>(*this);
                break;
            default:
                break;
        }
        erase();
        switch (holderType) {
            case CellType::I:
                curBlock = std::make_shared<IBlock>(*this);
                break;
            case CellType::J:
                curBlock = std::make_shared<JBlock>(*this);
                break;
            case CellType::L:
                curBlock = std::make_shared<LBlock>(*this);
                break;
            case CellType::O:
                curBlock = std::make_shared<OBlock>(*this);
                break;
            case CellType::S:
                curBlock = std::make_shared<SBlock>(*this);
                break;
            case CellType::Z:
                curBlock = std::make_shared<ZBlock>(*this);
                break;
            case CellType::T:
                curBlock = std::make_shared<TBlock>(*this);
                break;
            default:
                break;
        }
    }
    update();
    observerPrintGrid();
}

shared_ptr<Block> Grid::getHolderBlock() {
    return blockHolder;
}

bool Grid::level5RandomPlace(int x, int y, CellType type) {
    switch (type) {
        case CellType::J:
            if (x + 2 > col - 1) return false;
            if (theGrid[y - 1][x].getState() ||
                theGrid[y][x + 1].getState() ||
                theGrid[y][x + 2].getState()) return false;
            theGrid[y][x].turnOn();
            theGrid[y - 1][x].turnOn();
            theGrid[y][x + 1].turnOn();
            theGrid[y][x + 2].turnOn();
            theGrid[y][x].setType(type);
            theGrid[y - 1][x].setType(type);
            theGrid[y][x + 1].setType(type);
            theGrid[y][x + 2].setType(type);
            break;
        case CellType::L:
            if (x + 2 > col - 1) return false;
            if (theGrid[y][x + 1].getState() ||
                theGrid[y][x + 2].getState() ||
                theGrid[y - 1][x + 2].getState()) return false;
            theGrid[y][x].turnOn();
            theGrid[y][x + 1].turnOn();
            theGrid[y][x + 2].turnOn();
            theGrid[y - 1][x + 2].turnOn();
            theGrid[y][x].setType(type);
            theGrid[y][x + 1].setType(type);
            theGrid[y][x + 2].setType(type);
            theGrid[y - 1][x + 2].setType(type);
            break;
        case CellType::O:
            if (x + 1 > col - 1) return false;
            if (theGrid[y][x + 1].getState() ||
                theGrid[y - 1][x].getState() ||
                theGrid[y - 1][x + 1].getState()) return false;
            theGrid[y][x].turnOn();
            theGrid[y][x + 1].turnOn();
            theGrid[y - 1][x].turnOn();
            theGrid[y - 1][x + 1].turnOn();
            theGrid[y][x].setType(type);
            theGrid[y][x + 1].setType(type);
            theGrid[y - 1][x].setType(type);
            theGrid[y - 1][x + 1].setType(type);
            break;
        case CellType::S:
            if (x + 2 > col - 1) return false;
            if (theGrid[y][x + 1].getState() ||
                theGrid[y - 1][x + 1].getState() ||
                theGrid[y - 2][x + 2].getState()) return false;
            theGrid[y][x + 1].turnOn();
            theGrid[y - 1][x + 1].turnOn();
            theGrid[y - 2][x + 2].turnOn();
            theGrid[y][x].turnOn();
            theGrid[y][x + 1].setType(type);
            theGrid[y - 1][x + 1].setType(type);
            theGrid[y - 2][x + 2].setType(type);
            theGrid[y][x].setType(type);
            break;
        case CellType::Z:
            if (x + 2 > col - 1 || y + 1 > row - 1) return false;
            if (theGrid[y][x + 1].getState() ||
                theGrid[y + 1][x + 1].getState() ||
                theGrid[y + 1][x + 2].getState()) return false;
            theGrid[y][x + 1].turnOn();
            theGrid[y + 1][x + 1].turnOn();
            theGrid[y + 1][x + 2].turnOn();
            theGrid[y][x].turnOn();
            theGrid[y][x + 1].setType(type);
            theGrid[y + 1][x + 1].setType(type);
            theGrid[y + 1][x + 2].setType(type);
            theGrid[y][x].setType(type);
            break;
        case CellType::T:
            if (x + 2 > col - 1 || y + 1 > row - 1) return false;
            if (theGrid[y][x + 1].getState() ||
                theGrid[y][x + 2].getState() ||
                theGrid[y + 1][x + 1].getState()) return false;
            theGrid[y][x].turnOn();
            theGrid[y][x + 1].turnOn();
            theGrid[y][x + 2].turnOn();
            theGrid[y + 1][x + 1].turnOn();
            theGrid[y][x].setType(type);
            theGrid[y][x + 1].setType(type);
            theGrid[y][x + 2].setType(type);
            theGrid[y + 1][x + 1].setType(type);
            break;
        default:
            break;
    }
    return true;
}

void Grid::level5RandomBlock() {
    // determine CellType
    int type = rand() % 5;
    CellType newType;
    if (type == 0){
        newType = CellType::Z;
    } else if (type == 1){
        newType = CellType::T;
    } else if (type == 2){
        newType = CellType::J;
    } else if (type == 3){
        newType = CellType::L;
    } else if (type == 4){
        newType = CellType::O;
    } else {
        newType = CellType::S;
    }
    int count = 0;
    while (true) {
        // determine coordinates of the random block
        int blockx;
        int blocky = rand() % 15 + 3;
        if (blocky == 4) {
            blockx = rand() % 7 + 4;
        } else {
            blockx = rand() % 10;
        }
        if (theGrid[blocky][blockx].getState()) continue;
        if (level5RandomPlace(blockx, blocky, newType)) break;
        ++count;
        if (count == 30) break;
    }
}

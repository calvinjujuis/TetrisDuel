#include "graphicsdisplay.h"
#include "window.h"
#include <sstream>

const int windowSize = 500;
const int cellSize = 20;
const int blockSize = 18;
const int playerWidth = 220;
const int infoBoardHeight = 45;
const int gridHeight = 360;
const int nextBlockHeight = 75;
const int nextBlockx = 22;
const int nextBlocky = 25;
const int heightGap = 5;
const int widthGap = 15;
const int lineSize = 2;
const int funcBoardGrid = 120;

GraphicsDisplay::GraphicsDisplay() {
    init();
}

GraphicsDisplay::~GraphicsDisplay() {}

void GraphicsDisplay::drawLevelnScore(int player) {
    int currentPlayer = player - 1;
    // levels
    xw.fillRectangle(widthGap + currentPlayer * windowSize / 2, heightGap, playerWidth, infoBoardHeight, Xwindow::Board);
    xw.drawText(20 + currentPlayer * windowSize / 2, 20, "Level:", Xwindow::Text);
    // draw a divider
    xw.fillRectangle(widthGap + cellSize * 3 + currentPlayer * windowSize / 2, heightGap, 5, infoBoardHeight, Xwindow::BackGround);
    // scores
    xw.drawText(widthGap + cellSize * 3 + 10 + currentPlayer * windowSize / 2, 20, "Score:", Xwindow::Text);
    
}

void GraphicsDisplay::drawBoard(int player) {
    int currentPlayer = player - 1;
    xw.fillRectangle(widthGap + currentPlayer * windowSize / 2, heightGap + infoBoardHeight + heightGap, playerWidth, gridHeight, Xwindow::Board);
}

void GraphicsDisplay::drawNextBlock(int player) {
    int currentPlayer = player - 1;
    xw.fillRectangle(widthGap + currentPlayer * windowSize / 2, heightGap + infoBoardHeight + heightGap + gridHeight + heightGap, playerWidth, nextBlockHeight, Xwindow::Board);
    xw.drawText(widthGap + 5 + currentPlayer * windowSize / 2, heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + 15, "Next Block:", Xwindow::Text);
    xw.fillRectangle(widthGap + 5 + currentPlayer * windowSize / 2 + 105, heightGap + infoBoardHeight + heightGap + gridHeight + heightGap, 3, nextBlockHeight, Xwindow::BackGround);
    xw.drawText(widthGap - 3 + funcBoardGrid + currentPlayer * windowSize / 2, heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + 15, "Holder:", Xwindow::Text);
}

void GraphicsDisplay::drawGrid(int player) {
    int currentPlayer = player - 1;
    for (int i = 0; i <= row; ++i) {
        xw.fillRectangle(widthGap + currentPlayer * 250, heightGap + infoBoardHeight + heightGap + i * cellSize, playerWidth + lineSize, 2, Xwindow::BackGround);
    }
    for (int j = 0; j <= col; ++j) {
        xw.fillRectangle(widthGap + j * cellSize + currentPlayer * windowSize / 2, heightGap + infoBoardHeight + heightGap, 2, gridHeight, Xwindow::BackGround);
    }
}


void GraphicsDisplay::init() {
    xw.fillRectangle(0, 0, windowSize, windowSize, Xwindow::BackGround);
    // Levels and scores
    drawLevelnScore(1);
    drawLevelnScore(2);
    // Boards
    drawBoard(1);
    drawBoard(2);
    // Next Block
    drawNextBlock(1);
    drawNextBlock(2);
    // draw bars on boards
    drawGrid(1);
    drawGrid(2);
}

void GraphicsDisplay::clear(int player) {
    drawLevelnScore(player);
    drawBoard(player);
    drawNextBlock(player);
}

void GraphicsDisplay::printByType(size_t x, size_t y,
                                  int width, int height, CellType type) {
    switch (type) {
        case CellType::I:
            xw.fillRectangle(x, y, width, height, Xwindow::Pink);
            break;
        case CellType::J:
            xw.fillRectangle(x, y, width, height, Xwindow::Red);
            break;
        case CellType::L:
            xw.fillRectangle(x, y, width, height, Xwindow::Orange);
            break;
        case CellType::O:
            xw.fillRectangle(x, y, width, height, Xwindow::Green);
            break;
        case CellType::S:
            xw.fillRectangle(x, y, width, height, Xwindow::Purple);
            break;
        case CellType::Z:
            xw.fillRectangle(x, y, width, height, Xwindow::Blue);
            break;
        case CellType::T:
            xw.fillRectangle(x, y, width, height, Xwindow::Yellow);
            break;
        case CellType::Star:
            xw.fillRectangle(x, y, width, height, Xwindow::Brown);
            break;
        case CellType::Selected:
            xw.fillRectangle(x, y, width, height, Xwindow::Selected);
            break;
    }
}

void GraphicsDisplay::printBlockByType(CellType type, int currentPlayer, int which) {
    // 2 X 4 Grid
    for (int i = 0; i < 3; i++) {
        xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + which * funcBoardGrid,
                         heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + i * cellSize,
                        2 * 5 + blockSize * 4, 2, Xwindow::BackGround);
    }
    
    for (int j = 0; j < 5; j++) {
        xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + j * cellSize + which * funcBoardGrid,
                         heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky,
                         2, 2 * 3 + blockSize * 2, Xwindow::BackGround);
    }
    for (int i = 0; i < 4; i++) {
        xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                         heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                         blockSize, blockSize, Xwindow::Board);
    }
    for (int i = 0; i < 4; i++) {
        xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                         heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                         blockSize, blockSize, Xwindow::Board);
    }
    switch (type) {
        case CellType::I:
            for (int i = 0; i < 4; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                                 blockSize, blockSize, Xwindow::Pink);
            }
            break;
        case CellType::J:
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                                 blockSize, blockSize, Xwindow::Red);
            for (int i = 0; i < 3; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                                 blockSize, blockSize, Xwindow::Red);
            }
            break;
        case CellType::L:
            xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + 2 * cellSize + which * funcBoardGrid,
                             heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                             blockSize, blockSize, Xwindow::Orange);
            for (int i = 0; i < 3; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                                 blockSize, blockSize, Xwindow::Orange);
            }
            break;
        case CellType::O:
            for (int i = 0; i < 2; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                                 blockSize, blockSize, Xwindow::Green);
            }
            for (int i = 0; i < 2; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                                 blockSize, blockSize, Xwindow::Green);
            }
            break;
        case CellType::S:
            for (int i = 1; i < 3; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                                 blockSize, blockSize, Xwindow::Purple);
            }
            for (int i = 0; i < 2; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                                 blockSize, blockSize, Xwindow::Purple);
            }
            break;
        case CellType::Z:
            for (int i = 0; i < 2; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                                 blockSize, blockSize, Xwindow::Blue);
            }
            for (int i = 1; i < 3; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                                 blockSize, blockSize, Xwindow::Blue);
            }
            break;
        case CellType::T:
            for (int i = 0; i < 3; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + lineSize,
                                 blockSize, blockSize, Xwindow::Yellow);
            }
            for (int i = 1; i < 2; i++) {
                xw.fillRectangle(nextBlockx + currentPlayer * currentPlayer * windowSize / 2 + lineSize + i * cellSize + which * funcBoardGrid,
                                 heightGap + infoBoardHeight + heightGap + gridHeight + heightGap + nextBlocky + 2 * lineSize + blockSize,
                                 blockSize, blockSize, Xwindow::Yellow);
            }
            break;
        default:
            break;
    }
}

void GraphicsDisplay::printLevel(int player, int level) {
    int currentPlayer = player - 1;
    xw.fillRectangle(20 + currentPlayer * windowSize / 2, 30, 45, 10, Xwindow::Board);
    if (level > 999) level = 999;
    string str = to_string(level);
    xw.drawText(20 + currentPlayer * windowSize / 2, 40, str, Xwindow::Text);
}

void GraphicsDisplay::printScore(int player, int score) {
    int currentPlayer = player - 1;
    xw.fillRectangle(widthGap + cellSize * 3 + 10 + currentPlayer * windowSize / 2 , 30, 100, 10, Xwindow::Board);
    if (score > 99999) score = 99999;
    string str = to_string(score);
    xw.drawText(widthGap + cellSize * 3 + 10 + currentPlayer * windowSize / 2, 40, str, Xwindow::Text);
    
}

void GraphicsDisplay::notify(Grid& theGrid) {
    int currentPlayer = theGrid.getPlayer() - 1;
    printLevel(theGrid.getPlayer(), theGrid.getLevel());
    printScore(theGrid.getPlayer(), theGrid.getScore());
    vector<Point> erasedPoints = theGrid.getErased();
    for (size_t i = 0; i < erasedPoints.size(); ++i) {
        size_t erasedx = widthGap + lineSize + erasedPoints[i].x * cellSize + currentPlayer * windowSize / 2;
        size_t erasedy = heightGap + infoBoardHeight + heightGap + lineSize + erasedPoints[i].y * cellSize;
        xw.fillRectangle(erasedx, erasedy, blockSize, blockSize, Xwindow::Board);
    }
    shared_ptr<Block> curBlock = theGrid.getCurBlock();
    vector<Point> allPoints = curBlock->getPoints();
    for (size_t i = 0; i < allPoints.size(); ++i) {
        if (allPoints[i].state) {
            size_t blockx = widthGap + lineSize + allPoints[i].x * cellSize + currentPlayer * windowSize / 2;
            size_t blocky = heightGap + infoBoardHeight + heightGap + lineSize + allPoints[i].y * cellSize;
            printByType(blockx, blocky, blockSize, blockSize, theGrid.getGrid()[allPoints[i].y][allPoints[i].x].getType());
        }
    }
    if (blinded) {
        for (int row = 2; row < 12; ++row) {
            for (int col = 2; col < 9; ++col) {
                size_t blockx = widthGap + lineSize + col * cellSize + currentPlayer * windowSize / 2;
                size_t blocky = heightGap + infoBoardHeight + heightGap + lineSize + row * cellSize;
                xw.fillRectangle(blockx, blocky, blockSize, blockSize, Xwindow::Black);
            }
        }
    }
    
}


void GraphicsDisplay::printGrid(Grid &theGrid) {
    int currentPlayer = theGrid.getPlayer() - 1;
    printLevel(theGrid.getPlayer(), theGrid.getLevel());
    printScore(theGrid.getPlayer(), theGrid.getScore());
    vector<vector<Cell>> board = theGrid.getGrid();
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                size_t blockx = widthGap + lineSize + j * cellSize + currentPlayer * windowSize / 2;
                size_t blocky = heightGap + infoBoardHeight + heightGap + lineSize + i * cellSize;
                if (board[i][j].getState()) { // if the block is on, print
                    printByType(blockx, blocky, blockSize, blockSize, board[i][j].getType());
                } else { // if the cell is off, erase
                    xw.fillRectangle(blockx, blocky, blockSize, blockSize, Xwindow::Board);
                }
            }
        }
    int NEXTBLOCK = 0;
    int HOLDERBLOCK = 1;
    CellType nextType = theGrid.getnextBlock()->getType();
    printBlockByType(nextType, currentPlayer, NEXTBLOCK);
    if (theGrid.getHolderBlock() != nullptr) {
        CellType holderType = theGrid.getHolderBlock()->getType();
        printBlockByType(holderType, currentPlayer, HOLDERBLOCK);
    }
}

void GraphicsDisplay::blind() { blinded = true; }

void GraphicsDisplay::unblind() { blinded = false; }

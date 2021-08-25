#include "controller.h"
#include "observer.h"
using namespace std;

Controller::Controller(vector<string> player1blocks,
                       vector<string> player2blocks,
               bool isTextOnly) :
    result{0}, highestScore{0} {
    g1 = make_shared<Grid>(1);
    g2 = make_shared<Grid>(2);
    td = make_shared<TextDisplay>();
    if (! isTextOnly) {
        gd = make_shared<GraphicsDisplay>();
    }
    g1->setLevel0(convert(player1blocks));
    g2->setLevel0(convert(player2blocks));
}


shared_ptr<Grid> Controller::determinePlayer(int p) {
    shared_ptr<Grid> theGrid;
    if (p == 1) {
        theGrid = g1;
    } else if (p == 2) {
        theGrid = g2;
    }
    return theGrid;
}

// toLowerCase(str) returns str with all chars in lowercase
string toLowerCase(string str) {
    string newstr;
    size_t strlen = str.length();
    for (size_t i = 0; i < strlen; ++i) {
        newstr += tolower(str[i]);
    }
    return newstr;
}

vector<CellType> Controller::convert(vector<string> playerBlocks) {
    vector<CellType> level0Blocks;
    CellType type;
    for (size_t i = 0; i < playerBlocks.size(); ++i) {
        string blockType = toLowerCase(playerBlocks[i]);
        if (blockType == "i") {
            type = CellType::I;
        } else if (blockType == "j") {
            type = CellType::J;
        } else if (blockType == "l") {
            type = CellType::L;
        } else if (blockType == "o") {
            type = CellType::O;
        } else if (blockType == "s") {
            type = CellType::S;
        } else if (blockType == "z") {
            type = CellType::Z;
        } else if (blockType == "t") {
            type = CellType::T;
        } else if (blockType == "star") {
            type = CellType::Star;
        }
        level0Blocks.push_back(type);
    }
    return level0Blocks;
}

void Controller::init(bool isTextOnly) {
    g1->attachObserver(td);
    g2->attachObserver(td);
    if (isTextOnly == false) {
        g1->attachObserver(gd);
        g2->attachObserver(gd);
    }
    g1->init();
    g2->init();
}

int Controller::getResult() { return result; }

void Controller::restart() {
    result = 0;
    g1->clear();
    g2->clear();
}

int Controller::getHighestScore() {
    int p1Score = g1->getScore();
    int p2Score = g2->getScore();
    if (p1Score > p2Score) {
        highestScore = p1Score;
    } else {
        highestScore = p2Score;
    }
    return highestScore;
}

void Controller::moveLeft(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->moveLeft(multiplier);
}

void Controller::moveRight(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->moveRight(multiplier);
}

void Controller::moveDown(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->moveDown(multiplier);
}

void Controller::clockwise(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->clockwise(multiplier);
}

void Controller::counterClockwise(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->counterClockwise(multiplier);
}

int Controller::drop(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    int rowsCleared = theGrid->drop(multiplier);
    if (theGrid->isLost() == true) {
        if (result == 1 || result == 2) { // one player has lost
            result = 3; // both players have lost
        } else {        // no player has lost
            result = p; // p has lost
        }
    }
    return rowsCleared;
}

string Controller::hint(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    string hintMsg = theGrid->hint();
    return hintMsg;
}

void Controller::levelUp(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->levelUp(multiplier);
}

void Controller::levelDown(int multiplier, int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->levelDown(multiplier);
}

void Controller::noRandom(int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->noRandom();
}

void Controller::restoreRandom(int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->restoreRandom();
}

void Controller::changeCurrentBlock(int p, CellType type) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->changeCurrentBlock(type);
}

void Controller::makeBlind(int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->blind();
}

void Controller::makeHeavy(int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->heavy();
}

void Controller::makeJustDrop(int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->justDrop();
}

bool Controller::selected(int p, int x, int y) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    return theGrid->selected(x, y);
}

void Controller::storeBlock(int p) {
    shared_ptr<Grid> theGrid = determinePlayer(p);
    theGrid->storeBlock();
}

ostream &operator<<(ostream &out, const Controller &ctrl) {
    out << *ctrl.td;
    return out;
}

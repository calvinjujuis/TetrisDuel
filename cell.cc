#include "cell.h"
using namespace std;

Cell::Cell(shared_ptr<Block> theBlock) :
    state{false}, theBlock{theBlock} {}

bool Cell::getState() { return state; }

CellType Cell::getType() { return type; }

shared_ptr<Block> Cell::getBlock() { return theBlock; }

void Cell::flip() {
    state = !state;
}

void Cell::turnOff() {
    state = false;
}

void Cell::turnOn() {
    state = true;
}

void Cell::setType(CellType type) {
    this->type = type;
}

void Cell::setBlock(shared_ptr<Block> newBlock) {
    theBlock = newBlock;
}


void Cell::cleared() {
    if (theBlock == nullptr) return;
    theBlock->addCount();
}


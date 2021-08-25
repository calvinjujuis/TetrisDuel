#include "decorator.h"
using namespace std;
class Block;

Decorator::Decorator( shared_ptr<Block> component ): component(component) {}
void Decorator:: down() {
    component->down();
}

void Decorator::moveUp() {
    component->moveUp();
}

bool Decorator::isdown() {
    return component->isdown();
}
void Decorator::moveLeft() {
    component->moveLeft();
}
void Decorator::moveRight() {
    component->moveRight();
}

void Decorator::clockwise() {
    component->clockwise();
}
void Decorator::counterClockwise() {
    component->counterClockwise();
}

std::shared_ptr<Block> Decorator::makeBlock(Grid & grid, CellType type) {
    return component->makeBlock(grid, type);
}

void Decorator::updateBoundaries() {
    component->updateBoundaries();
}

int Decorator::getBottom() {
    return component->getBottom();
}
int Decorator::getLeft() {
    return component->getLeft();
}

int Decorator::getRight() {
    return component->getRight();
}
CellType Decorator::getType() {
    return component->getType();
}
std::vector<Point> Decorator::getPoints() {
    return component->getPoints();
}
void Decorator::setHeavy() {
    component->setHeavy();
}

void Decorator::unHeavy() {
    component->unHeavy();
}

bool Decorator::getHeavy() {
    return component->getHeavy();
}

void Decorator::drop() {
    component->drop();
}
void Decorator::addCount() {
    component->addCount();
}

Grid & Decorator::getGrid() {
    return component->getGrid();
}

Decorator::~Decorator() {}

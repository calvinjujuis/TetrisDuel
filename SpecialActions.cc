#include "SpecialActions.h"
#include "block.h"
using namespace std;

Blind::Blind(shared_ptr<Block> component): Decorator{component}{}

void Blind::down() {
    component->getGrid().makeBlind();
    component->down();
}

void Blind::drop() {
    component->getGrid().makeBlind();
    component->drop();
    component->getGrid().undoBlind();
}

void Blind::moveLeft() {
    component->getGrid().makeBlind();
    component->moveLeft();
}

void Blind::moveRight() {
    component->getGrid().makeBlind();
    component->moveRight();
}

void Blind::clockwise() {
    component->getGrid().makeBlind();
    component->clockwise();
}

void Blind::counterClockwise() {
    component->getGrid().makeBlind();
    component->counterClockwise();
}


Heavy::Heavy(shared_ptr<Block> component): Decorator{component}{}

void Heavy::down() {
    component->down();
}

void Heavy::drop() {
    component->drop();
}

void Heavy::moveLeft() {
    component->moveLeft();
    // Falls by two rows;
    component->down();
    component->down();
    bool down = component->isdown();
    if (!down) {
        component->drop();
    }
}

void Heavy::moveRight() {
    component->moveRight();
    component->down();
    component->down();
    bool down = component->isdown();
    if (!down) {
        component->drop();
    }
}

void Heavy::clockwise() {
    component->clockwise();
}

void Heavy::counterClockwise() {
    component->counterClockwise();
}


Force::Force(shared_ptr<Block> component): Decorator{component}{}

JustDrop::JustDrop(shared_ptr<Block> component): Decorator{component}{}

void JustDrop::down() {}

void JustDrop::drop() {
    component->drop();
}

void JustDrop::moveLeft() {
    component->moveLeft();
}

void JustDrop::moveRight() {
    component->moveRight();
}

void JustDrop::clockwise() {
    component->clockwise();
}

void JustDrop::counterClockwise() {
    component->counterClockwise();
}

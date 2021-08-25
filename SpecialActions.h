#ifndef SpecialActions_hpp
#define SpecialActions_hpp

#include "decorator.h"


class Block;

class Blind: public Decorator {
public:
    Blind(std::shared_ptr<Block> component);
    void down() override;
    void drop() override;
    void moveLeft() override;
    void moveRight() override;
    void clockwise() override;
    void counterClockwise() override;
};

class Heavy: public Decorator {
public:
    Heavy(std::shared_ptr<Block> component);
    void down() override;
    void drop() override;
    void moveLeft() override;
    void moveRight() override;
    void clockwise() override;
    void counterClockwise() override;
};

class Force: public Decorator {
public:
    Force(std::shared_ptr<Block> component);
};

class JustDrop: public Decorator {
public:
    JustDrop(std::shared_ptr<Block> component);
    void down() override;
    void drop() override;
    void moveLeft() override;
    void moveRight() override;
    void clockwise() override;
    void counterClockwise() override;
};

#endif /* SpecialActions_hpp */


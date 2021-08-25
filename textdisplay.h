#ifndef textdisplay_hpp
#define textdisplay_hpp

#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include "observer.h"

enum class CellType;

class TextDisplay : public Observer {
    const int row = 18;
    const int col = 11;
    int highestScore;
    int p1Score, p2Score;
    int p1Level, p2Level;
    bool blinded;
    std::vector<std::vector<char>> g1;
    std::vector<std::vector<char>> g2;
    shared_ptr<Block> p1NextBlock;
    shared_ptr<Block> p2NextBlock;
    shared_ptr<Block> p1HolderBlock;
    shared_ptr<Block> p2HolderBlock;
    void init();  // initializes g1 and g2
    void clear(); // doesn't reset highScore
    /*void updateTD(int n, std::vector<std::vector<Cell>> &newGrid) const;
    void updateScoreTD(int n, int score);
    void updateHighScoreTD(int score);
    void updateLevelTD(int n, int level);
    void updateNextBlockTD(int n, char type);*/
    
public:
    TextDisplay();
    ~TextDisplay();
    
    void notify(Grid& theGrid) override;
    void printGrid(Grid& theGrid) override;
    void blind() override;
    void unblind() override;
    
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif /* textdisplay_hpp */

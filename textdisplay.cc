#include "textdisplay.h"
using namespace std;

TextDisplay::TextDisplay() {
    init();
}

TextDisplay::~TextDisplay() {}

void TextDisplay::init() {
    // initialize g1
    for (int i = 0; i < row; ++i) {
        vector<char> oneRow;
        g1.push_back(oneRow);
        for (int j = 0; j < col; ++j) {
            char c = ' ';
            g1[i].push_back(c);
        }
    }
    // initialize g2
    for (int i = 0; i < row; ++i) {
        vector<char> oneRow;
        g2.push_back(oneRow);
        for (int j = 0; j < col; ++j) {
            char c = ' ';
            g2[i].push_back(c);
        }
    }
    highestScore = 0;
    p1Score = 0;
    p2Score = 0;
    p1Level = 0;
    p2Level = 0;
    blinded = false;
}

void TextDisplay::clear() {
    int temp = highestScore;
    g1.clear();
    g2.clear();
    init();
    highestScore = temp;
}

// cellTypetoChar(type) converts all CellType to type
char cellTypetoChar(CellType type) {
    char c = ' ';
    if (type == CellType::I) {
        c = 'I';
    } else if (type == CellType::J) {
        c = 'J';
    } else if (type == CellType::L) {
        c = 'L';
    } else if (type == CellType::O) {
        c = 'O';
    } else if (type == CellType::S) {
        c = 'S';
    } else if (type == CellType::Z) {
        c = 'Z';
    } else if (type == CellType::T) {
        c = 'T';
    } else if (type == CellType::Star) {
        c = '*';
    } else if (type == CellType::Selected) {
        c = '~';
    }
    return c;
}

void update(vector<vector<char>> &newTD,
            vector<vector<Cell>> &theGrid, bool blinded) {
    for (int row = 0; row < 18; ++row) {
        for (int col = 0; col < 11; ++col) {
            if (theGrid[row][col].getState() == true) { // if the Cell is on
                newTD[row][col] = cellTypetoChar(theGrid[row][col].getType());
            } else {
                newTD[row][col] = ' ';
            }
        }
    }
    if (blinded) {
        for (int row = 2; row < 12; ++row) {
            for (int col = 2; col < 9; ++col) {
                newTD[row][col] = '?';
            }
        }
    }
}

void TextDisplay::notify(Grid &theGrid) {
    int player = theGrid.getPlayer();
    if (player == 1) {
        p1Level = theGrid.getLevel();
        p1Score = theGrid.getScore();
        p1NextBlock = theGrid.getnextBlock();
        p1HolderBlock = theGrid.getHolderBlock();
        update(g1, theGrid.getGrid(), blinded);
    } else if (player == 2) {
        p2Level = theGrid.getLevel();
        p2Score = theGrid.getScore();
        p2NextBlock = theGrid.getnextBlock();
        p2HolderBlock = theGrid.getHolderBlock();
        update(g2, theGrid.getGrid(), blinded);
    }
    if (p1Score > highestScore) highestScore = p1Score;
    if (p2Score > highestScore) highestScore = p2Score;
}

void TextDisplay::printGrid(Grid &theGrid) { notify(theGrid); }

void TextDisplay::blind() { blinded = true; }

void TextDisplay::unblind() { blinded = false; }

// cellTypetoStr(type, line) returns the corresponding line
//                           of block "type" in the form of string
string cellTypetoStr(CellType type, int line) {
    string lineOfType;
    if (line == 1) {
        if (type == CellType::I) {
            lineOfType = "IIII";
        } else if (type == CellType::J) {
            lineOfType = "J   ";
        } else if (type == CellType::L) {
            lineOfType = "  L ";
        } else if (type == CellType::O) {
            lineOfType = "OO  ";
        } else if (type == CellType::S) {
            lineOfType = " SS ";
        } else if (type == CellType::Z) {
            lineOfType = "ZZ  ";
        } else if (type == CellType::T) {
            lineOfType = "TTT ";
        } else {
            lineOfType = "    ";
        }
    } else if (line == 2) {
        if (type == CellType::I) {
            lineOfType = "    ";
        } else if (type == CellType::J) {
            lineOfType = "JJJ ";
        } else if (type == CellType::L) {
            lineOfType = "LLL ";
        } else if (type == CellType::O) {
            lineOfType = "OO  ";
        } else if (type == CellType::S) {
            lineOfType = "SS  ";
        } else if (type == CellType::Z) {
            lineOfType = " ZZ ";
        } else if (type == CellType::T) {
            lineOfType = " T  ";
        } else {
            lineOfType = "    ";
        }
    }
    return lineOfType;
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
    string gap = "        ";
    string divider = "-----------";
    // Levels & Scores
    out << endl;
    out << "Level:" << setw(5) << td.p1Level;
    out << gap;
    out << "Level:" << setw(5) << td.p2Level;
    out << endl;
    out << "Score:" << setw(5) << td.p1Score;
    out << gap;
    out << "Score:" << setw(5) << td.p2Score;
    out << endl;
    out << divider << gap << divider << endl;
    // Grids
    for (int row = 0; row < 18; ++row) {
        for (int col = 0; col < 11; ++col) {
            out << td.g1[row][col];
        }
        out << gap;
        for (int col = 0; col < 11; ++col) {
            out << td.g2[row][col];
        }
        out << endl;
    }
    out << divider << gap << divider << endl;
    
    // Next blocks:
    out << "Next:      " << gap << "Next:      " << endl;
    // print row1 and row2 of next blocks based on their types
    string nextBlockGap = "       ";
    int line = 1;
    CellType p1Type = td.p1NextBlock->getType();
    CellType p2Type = td.p2NextBlock->getType();
    string p1Next, p2Next;
    p1Next = cellTypetoStr(p1Type, line);
    p2Next = cellTypetoStr(p2Type, line);
    // "print" first line
    out << p1Next << nextBlockGap << gap << p2Next << endl;
    // second line
    ++line;
    p1Next = cellTypetoStr(p1Type, line);
    p2Next = cellTypetoStr(p2Type, line);
    out << p1Next << nextBlockGap << gap << p2Next << endl;
    
    // Holder:
    string holderBlockGap = "       ";
    out << "Holder:    " << gap << "Holder:    " << endl;
    // print row1 and row2 of Holder blocks based on their types
    line = 1;
    if (td.p1HolderBlock == nullptr) {
        p1Next = "    ";
    } else {
        p1Type = td.p1HolderBlock->getType();
        p1Next = cellTypetoStr(p1Type, line);
    }
    if (td.p2HolderBlock == nullptr) {
        p2Next = "    ";
    } else {
        p2Type = td.p2HolderBlock->getType();
        p2Next = cellTypetoStr(p2Type, line);
    }
    // "print" first line
    out << p1Next << holderBlockGap << gap << p2Next << endl;
    // second line
    ++line;
    if (td.p1HolderBlock == nullptr) {
        p1Next = "    ";
    } else {
        p1Type = td.p1HolderBlock->getType();
        p1Next = cellTypetoStr(p1Type, line);
    }
    if (td.p2HolderBlock == nullptr) {
        p2Next = "    ";
    } else {
        p2Type = td.p2HolderBlock->getType();
        p2Next = cellTypetoStr(p2Type, line);
    }
    out << p1Next << holderBlockGap << gap << p2Next << endl;
    return out;
}


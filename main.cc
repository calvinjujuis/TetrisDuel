#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include "controller.h"

using namespace std;

class InvalidFile {
public:
    string errorMessage() {
        string errorMsg = "Your file is not readable :( \nPlease check if you insert the correct name";
        return errorMsg;
    }
};

// readFromFile(fileName, commands) clears commands and stores every line of
//                                  fileName as elements in commands
void readFromFile(std::string fileName, std::vector<std::string> &commands) {
    commands.clear();
    string line;
    ifstream file{fileName};
    if (file.is_open()) {
        while (file >> line) {
            commands.push_back(line);
        }
    } else {
        throw InvalidFile{};
    }
}

bool gameEnded(int lost) {
    if (lost == 1) {
        cout << "Player 2 Has Won!" << endl;
    } else if (lost == 2) {
        cout << "Player 1 Has Won!" << endl;
    } else {
        cout << "It's a Tie!" << endl;
    }
    while (true) {
        cout << "Would you like to play again? [y / n]" << endl;
        char c;
        cin >> c;
        c = tolower(c);
        if (c == 'y') {
            return true;
        }
        return false;
    }
}

// getMultiplier(cmd) returns the multiplier prefix in cmd
//                    and wipes the prefix off from cmd
int getMultiplier(std::string &cmd) {
    size_t len = cmd.length();
    std::string multiplier;
    // extract preceding digits from cmd
    for (size_t i = 0; i < len; ++i) {
        if (isdigit(cmd[i])) {
            multiplier += cmd[i];
        } else {
            break;
        }
    }
    // delete multiplier from cmd
    size_t multiplierLen = multiplier.length();
    cmd = cmd.substr(multiplierLen);
    //convert multiplier into integer
    if (multiplier == "") return 1; // execute the cmd once if prefix not given
    int multiplierInt = std::stoi(multiplier);
    return multiplierInt;
}

// allToLower(str) returns str with all chars in lowercase
string allToLower(string str) {
    string newstr;
    size_t strlen = str.length();
    for (size_t i = 0; i < strlen; ++i) {
        newstr += tolower(str[i]);
    }
    return newstr;
}

void fillInfo(map<string, string> &commandsInfo, map<string, string> &specialActions) {
    commandsInfo["left"] = "moves the current block one cell to the left";
    commandsInfo["right"] = "moves the current block one cell to the right";
    commandsInfo["down"] = "moves the current block one cell downward";
    commandsInfo["clockwise"] = "rotates the block 90 degrees clockwise";
    commandsInfo["counterclockwise"] = "rotates the block 90 degrees counterclockwise";
    commandsInfo["drop"] = "drops the current block";
    commandsInfo["levelup"] = "increases the difficulty level of the game by one";
    commandsInfo["leveldown"] = "decreases the difficulty level of the game by one";
    commandsInfo["norandom"] = "Takes input from sequence file in levels 3 and 4";
    commandsInfo["random"] = "Restore randomness in levels 3 and 4";
    commandsInfo["sequence"] = "Executes the sequence of commands found in file";
    commandsInfo["I, J, L, etc"] = "changes the current block to the block given";
    commandsInfo["restart"] = "starts a new game (Highest score is kept)";
    commandsInfo["* store"] = "stores the current block in the block holder;\n        swaps blocks with the block holder if already stored";
    commandsInfo["* alias"] = "adds an alias to an existing command";
    commandsInfo["* group"] = "groups existing commands as a new command";
    
    
    specialActions["blind"] = "Blind Opponent's Board!\nPart of your opponent's board will be covered with question marks until they drop the next block.\n";
    specialActions["heavy"] = "Heavy Opponent's Current Block!\nA heavy block automatically falls by two rows when moved left or right.\n";
    specialActions["force"] = "Change Opponent's Current Block!\nPlayer gets to choose the next block for the opponent.\n";
    specialActions["justdrop"] = "Force Opponent's Current Block!\nOpponent's \"down\" command is disabled next turn.\n";
    specialActions["select"] = "Select a cell on your board to turn on.\nProvide a cell's x and y coordinates and it will be filled by a special block that can be cleared but doesn't provide extra scores\n";
}

int main(int argc, const char *argv[]) {
    cin.exceptions(ios::eofbit|ios::failbit);
    bool onlyText = false;
    string player1File = "sequence1.txt";
    string player2File = "sequence2.txt";
    int startLevel = 0;
    const int minLevel = 0;
    const int maxLevel = 4;
    

    // read in and apply command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string flag = argv[i];
        if (flag == "-text") {
            onlyText = true;
        } else if (flag == "-seed") {
            ++i;
            int seed = stoi(argv[i]);
            if (seed < 0) {
                cerr << "Seed needs to be positive!" << endl;
                return 1;
            }
            srand(seed);
        } else if (flag == "-scriptfile1") {
            ++i;
            player1File = argv[i];
        } else if (flag == "-scriptfile2") {
            ++i;
            player2File = argv[i];
        } else if (flag == "-startlevel") {
            ++i;
            startLevel = stoi(argv[i]);
            if (startLevel < minLevel || startLevel > maxLevel) {
                cerr << "Starting level needs to be between " <<
                minLevel << " to " << maxLevel << endl;
                return 1;
            }
        } else {
            cerr << "Unrecognized flags" << endl;
            return 1;
        }
    }
    
    // set level 0 blocks
    std::vector<std::string> player1Blocks;
    std::vector<std::string> player2Blocks;
    try {
        readFromFile(player1File, player1Blocks);
        readFromFile(player2File, player2Blocks);
    } catch (InvalidFile invalid) {
        cerr << endl << invalid.errorMessage() << endl << endl;
        return 1;
    }
    
    
    // execute commands
    string cmd;
    Controller ctrl{player1Blocks, player2Blocks, onlyText};
    ctrl.init(onlyText);
    map<string, string> commandsInfo;
    map<string, string> specialActions; // first is name, second is description
    fillInfo(commandsInfo, specialActions); // fill in description
    int currentPlayer = 1; // switch between 1 (player 1) and 2 (player 2)
    int commandCount = -1; // records the idx of the last executed command;
                           // is -1 when no command files
    int groupedCommandCount = -1; // records how many commands in a grouped commands have been executed;
                                  // is -1 when no grouped commands are being executed
    int highestScore = 0;
    vector<string> commands;
    map<string, string> renamedCommands;
    map<string, vector<string>> groupedCommands;
    vector<string> currentGroupedCommands;
    
    ctrl.levelUp(startLevel, 1);
    ctrl.levelUp(startLevel, 2);
    cout << ctrl;
    cout << "Welcome!\nType \"see\" to see all available commands.\nType \"highscore\" to see the current highest score.\n";
    try {
        while (true) {
            if (groupedCommandCount != -1) {
                cmd = currentGroupedCommands[groupedCommandCount];
                ++groupedCommandCount;
                if (groupedCommandCount == (int)currentGroupedCommands.size()) {
                    groupedCommandCount = -1;
                }
            } else if (commandCount != -1) { // if reading from file
                cmd = commands[commandCount];
                ++commandCount;
                if (commandCount == (int)commands.size()) {
                    commandCount = -1; // end reading from file
                }
            } else { // if accepting user input
                cout << "Please type your command:" << endl;
                cin >> cmd;
            }
            cmd = allToLower(cmd);
            
            
            if (groupedCommandCount == -1) {
                map<string, vector<string>>::iterator itGroup;
                itGroup = groupedCommands.find(cmd);
                if (itGroup != groupedCommands.end()) {
                    // if cmd is a key to a group of commands, we execute the group of commands
                    groupedCommandCount = 0;
                    currentGroupedCommands = itGroup->second;
                    continue;
                }
            }
            
            
            map<string, string>::iterator itRename;
            itRename = renamedCommands.find(cmd);
            if (itRename != renamedCommands.end()) {
                cmd = itRename->second; // if cmd == key, cmd is the renamed command (value)
            }
            
                
            if (cmd.substr(0, 3) == "seq") { // sequence
                // get filename
                string fileName;
                cin >> fileName;
                try {
                    readFromFile(fileName, commands);
                } catch (InvalidFile invalid) {
                    cerr << endl << invalid.errorMessage() << endl << endl;
                    return 1;
                }
                commandCount = 0;
                continue;
            }
            
            int multiplier = getMultiplier(cmd);
            if (multiplier == 0) continue; // execute 0 times of the command
            if (cmd.substr(0, 3) == "hig") {            // highestscore
                cout << "Highest Score: " << highestScore << endl;
                continue;
            } else if (cmd.substr(0, 2) == "al") {     // alias
                string newName;
                cout << "\nPlease type in the command that you want to add an alias to:" << endl;
                cin >> cmd;
                cmd = allToLower(cmd);
                cout << "What's the alias to \"" << cmd << "\":" << endl;
                cin >> newName;
                newName = allToLower(newName);
                renamedCommands[newName] = cmd;
                
                cout << "\n\"" << cmd << "\" now has an alias: "<< newName << endl << endl;
                continue;
                
            } else if (cmd.substr(0, 2) == "gr") {     // group commands
                vector<string> grouped;
                cout << "\nInsert names of the commands you want to group (each on a line)\nEnd by typing \"end\"\n:" << endl;
                while (cin >> cmd) {
                    cmd = allToLower(cmd);
                    if (cmd == "end") break;
                    grouped.push_back(cmd);
                }
                cout << "Please name your grouped commands:" << endl;
                cin >> cmd;
                cmd = allToLower(cmd);
                groupedCommands[cmd] = grouped;
                
                cout << "\n\"" << cmd << "\" now contains the following commands:" << endl;
                for(const auto& elem : grouped) {
                   cout << elem << endl;
                }
                cout << endl;
                continue;
                
            } else if (cmd.substr(0, 3) == "see") {            // see all commands
                cout << endl << endl;
                for(const auto& elem : commandsInfo) {
                   cout << elem.first << ": " << elem.second << endl;
                }
                cout << endl;
                continue;
            } else if (cmd.substr(0, 3) == "lef") {   // left
                ctrl.moveLeft(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 2) == "ri") {     // right
                ctrl.moveRight(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 2) == "do") {     // down
                ctrl.moveDown(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 2) == "cl") {     // clockwise
                ctrl.clockwise(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 2) == "co") {     // counterclockwise
                ctrl.counterClockwise(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 2) == "dr") {     // drop
                int rowsCleared = ctrl.drop(multiplier, currentPlayer);
                cout << ctrl;
                if (rowsCleared >= 2) { // triggers special actions
                    cout << "You cleared two or more rows in one move. Congratulations! You earned a special action." << endl;
                    cout << "Type \"see\" to see all available special actions\n" << endl;
                    int affectedPlayer = currentPlayer % 2 + 1; // the other player
                    while (true) {
                        cout << "Choose Your Special Action:" << endl;
                        cin >> cmd;
                        cmd = allToLower(cmd);
                        if (cmd == "see") {
                            cout << endl;
                            for(const auto& elem : specialActions) {
                               cout << elem.first << ":\n" << elem.second << endl;
                            }
                        } else if (cmd == "blind") {
                            ctrl.makeBlind(affectedPlayer);
                            break;
                        } else if (cmd == "heavy") {
                            ctrl.makeHeavy(affectedPlayer);
                            break;
                        } else if (cmd == "justdrop") {
                            ctrl.makeJustDrop(affectedPlayer);
                            break;
                        } else if (cmd == "force") {
                            cout << "Choose one from: I, J, L, O, S, Z and T" << endl;
                            cin >> cmd;
                            cmd = allToLower(cmd);
                            char type = cmd[0];
                            switch (type) {
                                case 'i':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::I);
                                    break;
                                case 'j':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::J);
                                    break;
                                case 'l':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::L);
                                    break;
                                case 'o':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::O);
                                    break;
                                case 's':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::S);
                                    break;
                                case 'z':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::Z);
                                    break;
                                case 't':
                                    ctrl.changeCurrentBlock(affectedPlayer, CellType::T);
                                    break;
                                default:
                                    cout << "Unrecognized Block!\n" << endl;
                                    continue;
                            }
                            break;
                        } else if (cmd == "select") {
                            bool isselected = false;
                            while (!isselected) {
                                cout << "Please input the coordinate of the block you wish to add on to the board:" << endl;
                                int x;
                                int y;
                                cin >> x >> y;
                                isselected = ctrl.selected(currentPlayer, x, y);
                                cout << isselected << endl;
                                if (!isselected) {
                                    cout << "The coordinate you input is invalid" << endl;
                                }
                            }
                            break;
                        } else {
                            cout << "Unrecognized Action! Please see all available actions" << endl;
                        }
                    }
                    cout << ctrl;
                }
            } else if (cmd.substr(0, 6) == "levelu") { // levelup
                ctrl.levelUp(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 6) == "leveld") { // leveldown
                ctrl.levelDown(multiplier, currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 3) == "hin") {    // hint
                string hintMsg = ctrl.hint(multiplier, currentPlayer);
                cout << endl << "Hint: " << hintMsg << endl;
                continue;
            } else if (cmd.substr(0, 2) == "no") {      // norandom
                ctrl.noRandom(currentPlayer);
                continue;
            } else if (cmd.substr(0, 2) == "ra") {     // random
                ctrl.restoreRandom(currentPlayer);
                continue;
            } else if (cmd.length() == 1) {            // change Block type
                char type = cmd[0];
                switch (type) {
                    case 'i':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::I);
                        break;
                    case 'j':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::J);
                        break;
                    case 'l':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::L);
                        break;
                    case 'o':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::O);
                        break;
                    case 's':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::S);
                        break;
                    case 'z':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::Z);
                        break;
                    case 't':
                        ctrl.changeCurrentBlock(currentPlayer, CellType::T);
                        break;
                }
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 2) == "st") {     // block holder
                ctrl.storeBlock(currentPlayer);
                cout << ctrl;
                continue;
            } else if (cmd.substr(0, 3) == "res") {    // restart
                cout << "Gonna restart, are you sure? [y / n] (Highest score will be kept)." << endl;
                cin >> cmd;
                if (cmd.substr(0, 1) == "y") {
                    ctrl.restart();
                } else {
                    continue;
                }
                cout << ctrl;
                cout << "\nRestarted" << endl;
                cout << "Highest Score: " << highestScore << "\n" << endl;
                continue;
            } else {
                cout << "\nUnrecognized Command! Try again.\nType \"see\" to see all available commands.\n" << endl;
                continue;
            }
            highestScore = ctrl.getHighestScore();
            if (currentPlayer == 2) { // after each round
                                      // (both player have played), check result
                int result = ctrl.getResult();
                if (result != 0) { // if at least one player has lost
                    bool playAgain = gameEnded(result);
                    if (playAgain == true) {
                        ctrl.restart();
                    } else {
                        return 0;
                    }
                }
            }
            currentPlayer = currentPlayer % 2 + 1; // switch between 1 and 2
        }
    } catch (ios::failure &) {
        cout << "Game Ended" << endl;
    }
}

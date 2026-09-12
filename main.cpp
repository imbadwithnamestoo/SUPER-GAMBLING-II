#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#include "src/Game.h"
#include "src/UI.h"
#include "src/Shop.h"
#include "src/Profile.h"
#include "src/Player.h"
#include "src/Utils.h"

void showIntro() {
    clear();
    int termWidth = getTerminalWidth();
    int boxWidth = std::min(60, termWidth - 4);
    int leftPadding = (termWidth - boxWidth) / 2;
    
    std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    
    std::string line1 = "You owe us. 100 grand. You know why. You know the terms.";
    printBoxLine(T_INFO + line1 + T_RESET, boxWidth, leftPadding);
    
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
    
    std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
    _getch();
    
    clear();
    
    std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    
    std::string line2 = "Get to work, kid.";
    printBoxLine(T_INFO + line2 + T_RESET, boxWidth, leftPadding);
    
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
    
    std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
    _getch();
}

void viewBadges() {
    clear();
    int termWidth = getTerminalWidth();
    int boxWidth = std::min(60, termWidth - 4);
    int leftPadding = (termWidth - boxWidth) / 2;
    
    std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
    
    std::string title = " ACHIEVEMENTS ";
    printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
    
    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
    
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    printBoxLine("Coming Soon", boxWidth, leftPadding);
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    
    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
    
    std::string hint = "Press Enter to return";
    printBoxLine(hint, boxWidth, leftPadding);
    
    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
    
    _getch();
}

int main() {
    clear();
    flushInput();
    std::srand(std::time(0));
    
    bool isNewGame = false;
    std::ifstream checkFile("save.txt");
    if (checkFile.is_open()) {
        checkFile.close();
        loadGame();
    } else {
        isNewGame = true;
        saveGame();
    }
    
    if (isNewGame) {
        showIntro();
    }

    int selected = 1;
    while (true) {
        clear();
        bool choiceMade = false;
        while (!choiceMade) {
            resetCursor();
            int termWidth = getTerminalWidth();
            int boxWidth = std::min(60, termWidth - 4);
            int leftPadding = (termWidth - boxWidth) / 2;

            std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;

            std::string title = " SUPER GAMBLING II ";
            printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);

            std::string chipsDisplay = formatNumber(totalChips);
            std::string debtDisplay = formatNumber(totalDebt);
            std::string stats = " Chips: " + T_PRICE + chipsDisplay + T_RESET + " | Debt: " + T_LOSS + debtDisplay + T_RESET + " ";
            printBoxLine(stats, boxWidth, leftPadding);

            std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;

            std::vector<std::string> options = {
                "Start",
                "Black Market",
                "Profile",
                "Achievements",
                "Quit"
            };

            int innerWidth = boxWidth - 2;

            for (size_t i = 0; i < options.size(); ++i) {
                bool selectedOption = selected == static_cast<int>(i) + 1;
                int textLen = visibleLength(options[i]);

                if (selectedOption) {
                    std::string color = (i == 4) ? T_LOSS : T_WIN;
                    std::string item = color + "► " + options[i] + " ◄" + T_RESET;
                    int itemLen = visibleLength(item);
                    int leftSpace = (innerWidth - itemLen) / 2;
                    int rightSpace = innerWidth - itemLen - leftSpace;

                    if (leftSpace < 0) leftSpace = 0;
                    if (rightSpace < 0) rightSpace = 0;

                    std::string displayText = repeatStr(" ", leftSpace) + item + repeatStr(" ", rightSpace);

                    std::cout << std::string(leftPadding, ' ') << "║" << displayText << "║" << std::endl;
                } else {
                    int leftSpace = (innerWidth - textLen) / 2;
                    int rightSpace = innerWidth - textLen - leftSpace;

                    if (leftSpace < 0) leftSpace = 0;
                    if (rightSpace < 0) rightSpace = 0;

                    std::string displayText = repeatStr(" ", leftSpace) + options[i] + repeatStr(" ", rightSpace);

                    std::cout << std::string(leftPadding, ' ') << "║" << displayText << "║" << std::endl;
                }
            }

            std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;

            std::string hint = "Arrow keys to navigate | Enter to select";
            printBoxLine(hint, boxWidth, leftPadding);

            std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;

            int input = _getch();
            if (input == 224 || input == 27) {
                int arrow = _getch();
                if (input == 27 && arrow == 91) {
                    arrow = _getch();
                }
                if (arrow == 72 || arrow == 'A') {
                    if (selected > 1) selected--;
                    else selected = 5;
                } else if (arrow == 80 || arrow == 'B') {
                    if (selected < 5) selected++;
                    else selected = 1;
                }
            } else if (input == 13 || input == 10) {
                choiceMade = true;
            }
        }
        if (selected == 1) {
            playBlackjackGame();
        } else if (selected == 2) {
            openStore();
        } else if (selected == 3) {
            viewProfile();
        } else if (selected == 4) {
            viewBadges();
        } else if (selected == 5) {
            break;
        }
    }
    return 0;
}

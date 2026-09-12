#include "Shop.h"
#include "UI.h"
#include "Player.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <vector>

void openStore() {
    clear();
    bool inStore = true;
    int selected = 1;
    
    while (inStore) {
        resetCursor();
        int termWidth = getTerminalWidth();
        int boxWidth = std::min(60, termWidth - 4);
        int leftPadding = (termWidth - boxWidth) / 2;
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        
        std::string title = " BLACK MARKET ";
        printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
        
        std::string chipsDisplay = "Chips: " + T_PRICE + std::to_string(totalChips) + T_RESET;
        std::string debtDisplay = "Debt: " + T_LOSS + formatNumber(totalDebt) + T_RESET;
        printBoxLine(chipsDisplay + " | " + debtDisplay, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        std::string luckyCharmDisplay = hasLuckyCharm ? "Lucky Charm [OWNED]" : "Lucky Charm [500 Chips]";
        std::string cardCounterDisplay = "Card Counter [1000 Chips] x" + std::to_string(cardCounterCount);
        std::string insuranceDisplay = hasInsurancePolicy ? "Insurance Policy [OWNED]" : "Insurance Policy [2000 Chips]";
        
        std::vector<std::string> items = {
            luckyCharmDisplay,
            cardCounterDisplay,
            insuranceDisplay,
            "Return"
        };
        
        int innerWidth = boxWidth - 2;
        
        for (size_t i = 0; i < items.size(); ++i) {
            bool selectedOption = selected == static_cast<int>(i) + 1;
            int textLen = visibleLength(items[i]);
            
            if (selectedOption) {
                std::string color = T_WIN;
                std::string item = color + "► " + items[i] + " ◄" + T_RESET;
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
                
                std::string displayText = repeatStr(" ", leftSpace) + items[i] + repeatStr(" ", rightSpace);
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
                else selected = 4;
            } else if (arrow == 80 || arrow == 'B') {
                if (selected < 4) selected++;
                else selected = 1;
            }
        } else if (input == 13 || input == 10) {
            if (selected == 4) {
                inStore = false;
            } else {
                clear();
                int subSelected = 1;
                bool inDetails = true;
                int innerWidth = boxWidth - 2;
                
                while (inDetails) {
                    resetCursor();
                    std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                    
                    std::string title = " ITEM DETAILS ";
                    printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
                    
                    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                    
                    if (selected == 1) {
                        printBoxLine("Lucky Charm", boxWidth, leftPadding);
                        printBoxLine("Cost: 500 Chips", boxWidth, leftPadding);
                        printBoxLine("Type: Passive", boxWidth, leftPadding);
                        printBoxLine("Effect: +10% bonus on all wins", boxWidth, leftPadding);
                    } else if (selected == 2) {
                        printBoxLine("Card Counter", boxWidth, leftPadding);
                        printBoxLine("Cost: 1000 Chips", boxWidth, leftPadding);
                        printBoxLine("Type: One-time", boxWidth, leftPadding);
                        printBoxLine("Effect: Reveal dealer's hidden card", boxWidth, leftPadding);
                        printBoxLine("Owned: x" + std::to_string(cardCounterCount), boxWidth, leftPadding);
                    } else if (selected == 3) {
                        printBoxLine("Insurance Policy", boxWidth, leftPadding);
                        printBoxLine("Cost: 2000 Chips", boxWidth, leftPadding);
                        printBoxLine("Type: Passive", boxWidth, leftPadding);
                        printBoxLine("Effect: Refund 10% of lost bets", boxWidth, leftPadding);
                    }
                    
                    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                    
                    std::vector<std::string> options = {"Buy", "Return"};
                    
                    for (size_t i = 0; i < options.size(); ++i) {
                        bool selectedOption = subSelected == static_cast<int>(i) + 1;
                        int textLen = visibleLength(options[i]);
                        
                        if (selectedOption) {
                            std::string color = T_WIN;
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
                    
                    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                    
                    int input2 = _getch();
                    if (input2 == 224 || input2 == 27) {
                        int arrow = _getch();
                        if (input2 == 27 && arrow == 91) {
                            arrow = _getch();
                        }
                        if (arrow == 72 || arrow == 'A') {
                            if (subSelected > 1) subSelected--;
                            else subSelected = 2;
                        } else if (arrow == 80 || arrow == 'B') {
                            if (subSelected < 2) subSelected++;
                            else subSelected = 1;
                        }
                    } else if (input2 == 13 || input2 == 10) {
                        if (subSelected == 1) {
                            int cost = 0;
                            if (selected == 1) cost = 500;
                            else if (selected == 2) cost = 1000;
                            else if (selected == 3) cost = 2000;
                            
                            bool alreadyOwned = (selected == 1 && hasLuckyCharm) || (selected == 3 && hasInsurancePolicy);
                            
                            if (alreadyOwned) {
                                clear();
                                resetCursor();
                                std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                                std::string msg = " Already owned! ";
                                printBoxLine(msg, boxWidth, leftPadding);
                                std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
                                _getch();
                            } else if (totalChips >= cost) {
                                totalChips -= cost;
                                if (selected == 1) hasLuckyCharm = true;
                                else if (selected == 2) cardCounterCount++;
                                else if (selected == 3) hasInsurancePolicy = true;
                                saveGame();
                                clear();
                                resetCursor();
                                std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                                std::string msg = " Purchased! ";
                                printBoxLine(msg, boxWidth, leftPadding);
                                std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
                                _getch();
                                inDetails = false;
                            } else {
                                clear();
                                resetCursor();
                                std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                                std::string msg = " Not enough chips! ";
                                printBoxLine(msg, boxWidth, leftPadding);
                                std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                                std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
                                _getch();
                            }
                        } else {
                            inDetails = false;
                        }
                    }
                }
            }
        }
    }
}
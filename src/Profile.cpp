#include "Profile.h"
#include "UI.h"
#include "Player.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

void viewProfile() {
    clear();
    bool inProfile = true;
    int selected = 1;
    
    while (inProfile) {
        resetCursor();
        int termWidth = getTerminalWidth();
        int boxWidth = std::min(60, termWidth - 4);
        int leftPadding = (termWidth - boxWidth) / 2;
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        
        std::string title = " PROFILE ";
        printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        printBoxLine("Chips: " + T_PRICE + std::to_string(totalChips) + T_RESET, boxWidth, leftPadding);
        printBoxLine("Debt: " + T_LOSS + std::to_string(totalDebt) + T_RESET, boxWidth, leftPadding);
        printBoxLine("Games Played: " + std::to_string(gamesPlayed), boxWidth, leftPadding);
        
        std::string wlDisplay = "Wins: " + T_WIN + std::to_string(gamesWon) + T_RESET + " | Losses: " + T_LOSS + std::to_string(gamesLost) + T_RESET;
        printBoxLine(wlDisplay, boxWidth, leftPadding);
        
        std::string ratioDisplay;
        if (gamesWon + gamesLost > 0) {
            double ratio = (double)gamesWon / (gamesWon + gamesLost) * 100.0;
            char buf[32];
            snprintf(buf, sizeof(buf), "Win Rate: %.1f%%", ratio);
            ratioDisplay = std::string(buf);
        } else {
            ratioDisplay = "Win Rate: N/A";
        }
        printBoxLine(ratioDisplay, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        std::vector<std::string> items = {
            "Pay Debt",
            "Inventory",
            "Return"
        };
        
        int innerWidth = boxWidth - 2;
        
        for (size_t i = 0; i < items.size(); ++i) {
            bool selectedOption = selected == static_cast<int>(i) + 1;
            int textLen = visibleLength(items[i]);
            
            if (selectedOption) {
                std::string color = (i == 2) ? T_LOSS : T_WIN;
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
                else selected = 3;
            } else if (arrow == 80 || arrow == 'B') {
                if (selected < 3) selected++;
                else selected = 1;
            }
        } else if (input == 13 || input == 10) {
            if (selected == 1) {
                // Pay Debt
                clear();
                resetCursor();
                
                int maxPayment = std::min(totalChips, totalDebt);
                
                std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                
                std::string payTitle = " PAY DEBT ";
                printBoxLine(T_TITLE + payTitle + T_RESET, boxWidth, leftPadding);
                
                std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                
                printBoxLine("Chips: " + T_PRICE + std::to_string(totalChips) + T_RESET, boxWidth, leftPadding);
                printBoxLine("Debt: " + T_LOSS + std::to_string(totalDebt) + T_RESET, boxWidth, leftPadding);
                
                std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                
                if (maxPayment < 1) {
                    printBoxLine("You have no chips to pay with.", boxWidth, leftPadding);
                    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                    std::string payHint = "Press Enter to return";
                    printBoxLine(payHint, boxWidth, leftPadding);
                    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                    _getch();
                } else {
                    std::string prompt = "Enter amount to pay (1 - " + std::to_string(maxPayment) + ") or 0 to cancel:";
                    printBoxLine(prompt, boxWidth, leftPadding);
                    
                    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                    std::cout << std::string(leftPadding, ' ') << "> ";
                    
                    int payment = 0;
                    std::cin >> payment;
                    
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        
                        if (payment >= 1 && payment <= maxPayment) {
                            totalChips -= payment;
                            totalDebt -= payment;
                            saveGame();
                            
                            clear();
                            resetCursor();
                            std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                            std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                            
                            std::string msg = " Paid " + std::to_string(payment) + " toward debt. ";
                            printBoxLine(T_WIN + msg + T_RESET, boxWidth, leftPadding);
                            
                            std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
                            std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                            std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
                            _getch();
                        }
                    }
                }
            } else if (selected == 2) {
                // Inventory
                clear();
                resetCursor();
                std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
                
                std::string invTitle = " INVENTORY ";
                printBoxLine(T_TITLE + invTitle + T_RESET, boxWidth, leftPadding);
                
                std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                
                bool hasAnyItem = hasLuckyCharm || hasInsurancePolicy || cardCounterCount > 0;
                
                if (!hasAnyItem) {
                    printBoxLine("No items owned.", boxWidth, leftPadding);
                } else {
                    if (hasLuckyCharm) {
                        printBoxLine("Lucky Charm", boxWidth, leftPadding);
                    }
                    if (cardCounterCount > 0) {
                        printBoxLine("Card Counter x" + std::to_string(cardCounterCount), boxWidth, leftPadding);
                    }
                    if (hasInsurancePolicy) {
                        printBoxLine("Insurance Policy", boxWidth, leftPadding);
                    }
                }
                
                std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
                
                std::string invHint = "Press Enter to return";
                printBoxLine(invHint, boxWidth, leftPadding);
                
                std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
                
                _getch();
            } else {
                inProfile = false;
            }
        }
    }
}
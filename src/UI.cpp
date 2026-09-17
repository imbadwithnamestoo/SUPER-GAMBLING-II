#include "UI.h"
#include "Player.h"
#include "Utils.h"
#include "Game.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#define wait(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

std::string T_TITLE = "\033[1;37m";
std::string T_USER = "\033[32m";
std::string T_PRICE = "\033[33m";
std::string T_INFO = "\033[36m";
std::string T_WIN = "\033[32m";
std::string T_LOSS = "\033[31m";
std::string T_ACCENT = "\033[33m";
std::string T_SPECIAL = "\033[35m";
std::string T_EXTRA = "\033[34m";
std::string T_RESET = "\033[0m";

void printBoxLine(const std::string& content, int boxWidth, int leftPadding) {
    int contentLen = visibleLength(content);
    int innerWidth = boxWidth - 2;
    int totalPadding = innerWidth - contentLen;
    if (totalPadding < 0) totalPadding = 0;
    int leftSpace = totalPadding / 2;
    int rightSpace = totalPadding - leftSpace;

    std::cout << std::string(leftPadding, ' ')
              << "║"
              << std::string(leftSpace, ' ')
              << content
              << std::string(rightSpace, ' ')
              << "║"
              << std::endl;
}

void printBoxLineLeft(const std::string& content, int boxWidth, int leftPadding) {
    int contentLen = visibleLength(content);
    int innerWidth = boxWidth - 2;
    int rightSpace = innerWidth - contentLen - 1;
    if (rightSpace < 0) rightSpace = 0;

    std::cout << std::string(leftPadding, ' ')
              << "║ "
              << content
              << std::string(rightSpace, ' ')
              << "║"
              << std::endl;
}

void printBlackjackUI(const std::vector<Card>& playerHand, const std::vector<Card>& dealerHand, const std::string& dealerDialogue, bool hideFirstCard, bool showFinal) {
    resetCursor();
    int termWidth = getTerminalWidth();
    int boxWidth = std::min(70, termWidth - 4);
    int leftPadding = (termWidth - boxWidth) / 2;

    std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;

    std::string title = " SUPER GAMBLING II ";
    printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);

    std::string fullDialogue = "Dealer: \"" + dealerDialogue + "\"";
    printBoxLine(T_INFO + fullDialogue + T_RESET, boxWidth, leftPadding);

    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;

    std::string dealerLabel = " DEALER ";
    printBoxLine(T_TITLE + dealerLabel + T_RESET, boxWidth, leftPadding);

    std::string dealerHandStr = getHandString(dealerHand, hideFirstCard);
    std::string dScore = hideFirstCard ? "" : "  [Total: " + std::to_string(calculateScore(dealerHand)) + "]";
    std::string dealerLine = "  " + dealerHandStr + dScore;
    printBoxLine(dealerLine, boxWidth, leftPadding);

    std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;

    std::string playerLabel = " Player [" + formatNumber(totalChips) + " Chips] ";
    printBoxLine(T_USER + playerLabel + T_RESET, boxWidth, leftPadding);

    std::string playerHandStr = getHandString(playerHand, false);
    std::string pScore = "  [Total: " + std::to_string(calculateScore(playerHand)) + "]";
    std::string playerLine = "  " + playerHandStr + pScore;
    printBoxLine(playerLine, boxWidth, leftPadding);

    if (showFinal) {
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;

        std::string finalMsg = "=== FINAL RESULTS ===";
        printBoxLine(finalMsg, boxWidth, leftPadding);

        std::string playerResult = "Player Total: " + std::to_string(calculateScore(playerHand));
        printBoxLine(playerResult, boxWidth, leftPadding);

        std::string dealerResult = "Dealer Total: " + std::to_string(calculateScore(dealerHand));
        printBoxLine(dealerResult, boxWidth, leftPadding);

        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;

        std::string resultMsg;
        int dealerScore = calculateScore(dealerHand);
        int playerScore = calculateScore(playerHand);

        if (playerScore > 21) {
            resultMsg = T_LOSS + "*** YOU BUSTED! DEALER WINS! ***" + T_RESET;
        } else if (dealerScore > 21) {
            resultMsg = T_WIN + "*** DEALER BUSTED! YOU WIN! ***" + T_RESET;
        } else if (playerScore > dealerScore) {
            resultMsg = T_WIN + "*** YOU WIN! ***" + T_RESET;
        } else if (playerScore < dealerScore) {
            resultMsg = T_LOSS + "*** DEALER WINS. ***" + T_RESET;
        } else {
            resultMsg = T_SPECIAL + "*** IT'S A TIE (PUSH)! ***" + T_RESET;
        }

        printBoxLine(resultMsg, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
    }
}

void showItemsMenu(int boxWidth, int leftPadding, bool& revealDealer, int& cardCounterRef) {
    bool inItems = true;
    int subSelected = 1;
    int innerWidth = boxWidth - 2;
    
    while (inItems) {
        clear();
        resetCursor();
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        
        std::string title = " ITEMS ";
        printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        std::vector<std::string> items;
        if (cardCounterRef > 0) {
            items.push_back("Card Counter [Use] x" + std::to_string(cardCounterRef));
        }
        items.push_back("Back");
        
        for (size_t i = 0; i < items.size(); ++i) {
            bool selectedOption = subSelected == static_cast<int>(i) + 1;
            int textLen = visibleLength(items[i]);
            
            if (selectedOption) {
                std::string color = (i == items.size() - 1) ? T_LOSS : T_SPECIAL;
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
                if (subSelected > 1) subSelected--;
                else subSelected = items.size();
            } else if (arrow == 80 || arrow == 'B') {
                if (subSelected < static_cast<int>(items.size())) subSelected++;
                else subSelected = 1;
            }
        } else if (input == 13 || input == 10) {
            if (subSelected == static_cast<int>(items.size())) {
                inItems = false;
            } else if (cardCounterRef > 0) {
                cardCounterRef--;
                revealDealer = true;
                saveGame();
                inItems = false;
            }
        }
    }
}
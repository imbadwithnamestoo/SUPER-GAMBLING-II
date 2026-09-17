#pragma once
#include <string>
#include <vector>
#include "Game.h"

extern std::string T_TITLE;
extern std::string T_USER;
extern std::string T_PRICE;
extern std::string T_INFO;
extern std::string T_WIN;
extern std::string T_LOSS;
extern std::string T_ACCENT;
extern std::string T_SPECIAL;
extern std::string T_EXTRA;
extern std::string T_RESET;

void printBoxLine(const std::string& content, int boxWidth, int leftPadding);
void printBoxLineLeft(const std::string& content, int boxWidth, int leftPadding);
void printBlackjackUI(const std::vector<Card>& playerHand, const std::vector<Card>& dealerHand, const std::string& dealerDialogue, bool hideFirstCard, bool showFinal);
void showItemsMenu(int boxWidth, int leftPadding, bool& revealDealer, int& cardCounterRef);
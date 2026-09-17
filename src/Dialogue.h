#pragma once
#include <string>
#include <vector>

std::string pickRandom(const std::vector<std::string>& options);

std::string getOpeningLine(bool isAllIn, bool isMinBet, bool isTwoBet, bool hasBeatenGame);
std::string getAllInWinLine(bool hasBeatenGame);
std::string getAllInLossLine(bool hasBeatenGame);
std::string getDealerBustLine(bool hasBeatenGame);
std::string getWinLine(bool firstWin, bool hugeBet, bool hasBeatenGame);
std::string getLossLine(bool firstLoss, bool hugeBet, bool hasBeatenGame);
std::string getPushLine(bool hasBeatenGame);
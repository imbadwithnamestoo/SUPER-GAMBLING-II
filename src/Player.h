#pragma once
#include <string>

extern int totalChips;
extern int totalDebt;
extern bool hasLuckyCharm;
extern bool hasInsurancePolicy;
extern int cardCounterCount;
extern int gamesPlayed;
extern int gamesWon;
extern int gamesLost;
extern bool hasBeatenGame;

void saveGame();
void loadGame();
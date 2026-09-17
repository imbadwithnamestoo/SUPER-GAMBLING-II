#pragma once
#include <string>

extern int totalChips;
extern int totalDebt;
extern bool hasLuckyCharm;
extern bool hasLuckyStreak;
extern bool hasInsurancePolicy;
extern int cardCounterCount;
extern int housesMercyCount;
extern int gamesPlayed;
extern int gamesWon;
extern int gamesLost;
extern bool hasBeatenGame;

extern int currentWinStreak;
extern int currentLossStreak;
extern int longestWinStreak;
extern int longestLossStreak;
extern int biggestWin;
extern int biggestLoss;
extern int totalChipsWon;
extern int totalChipsLost;
extern int totalWagered;
extern int blackjacksHit;
extern int playerBusts;
extern int dealerBusts;
extern int pushes;
extern int allInsMade;
extern int totalDebtPaid;
extern int highestChipsEver;

extern std::string playerName;

extern bool ach_firstSteps;
extern bool ach_firstWin;
extern bool ach_firstLoss;
extern bool ach_firstPush;
extern bool ach_onARoll;
extern bool ach_unstoppable;
extern bool ach_houseWatching;
extern bool ach_roughNight;
extern bool ach_rockBottom;
extern bool ach_pocketChange;
extern bool ach_comfortable;
extern bool ach_highRoller;
extern bool ach_millionaire;
extern bool ach_allIn;
extern bool ach_yolo;
extern bool ach_twentyOne;
extern bool ach_twiceAsNice;
extern bool ach_bust;
extern bool ach_houseAlwaysWins;
extern bool ach_debtFree;

void saveGame();
void loadGame();

bool hasAchievement(const std::string& id);
void unlockAchievement(const std::string& id);
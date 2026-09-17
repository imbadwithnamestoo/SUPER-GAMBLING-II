#include "Player.h"
#include <fstream>
#include <string>
#include <sstream>

int totalChips = 250;
int totalDebt = 100000;
bool hasLuckyCharm = false;
bool hasLuckyStreak = false;
bool hasInsurancePolicy = false;
int cardCounterCount = 0;
int housesMercyCount = 0;
int gamesPlayed = 0;
int gamesWon = 0;
int gamesLost = 0;
bool hasBeatenGame = false;

int currentWinStreak = 0;
int currentLossStreak = 0;
int longestWinStreak = 0;
int longestLossStreak = 0;
int biggestWin = 0;
int biggestLoss = 0;
int totalChipsWon = 0;
int totalChipsLost = 0;
int totalWagered = 0;
int blackjacksHit = 0;
int playerBusts = 0;
int dealerBusts = 0;
int pushes = 0;
int allInsMade = 0;
int totalDebtPaid = 0;
int highestChipsEver = 250;

std::string playerName = "";

bool ach_firstSteps = false;
bool ach_firstWin = false;
bool ach_firstLoss = false;
bool ach_firstPush = false;
bool ach_onARoll = false;
bool ach_unstoppable = false;
bool ach_houseWatching = false;
bool ach_roughNight = false;
bool ach_rockBottom = false;
bool ach_pocketChange = false;
bool ach_comfortable = false;
bool ach_highRoller = false;
bool ach_millionaire = false;
bool ach_allIn = false;
bool ach_yolo = false;
bool ach_twentyOne = false;
bool ach_twiceAsNice = false;
bool ach_bust = false;
bool ach_houseAlwaysWins = false;
bool ach_debtFree = false;

static const bool DISABLE_SAVE_ENCRYPTION = false;
static const std::string SAVE_KEY = "SuperGamblingII";

std::string xorCipher(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

bool hasAchievement(const std::string& id) {
    if (id == "first_steps") return ach_firstSteps;
    if (id == "first_win") return ach_firstWin;
    if (id == "first_loss") return ach_firstLoss;
    if (id == "first_push") return ach_firstPush;
    if (id == "on_a_roll") return ach_onARoll;
    if (id == "unstoppable") return ach_unstoppable;
    if (id == "house_watching") return ach_houseWatching;
    if (id == "rough_night") return ach_roughNight;
    if (id == "rock_bottom") return ach_rockBottom;
    if (id == "pocket_change") return ach_pocketChange;
    if (id == "comfortable") return ach_comfortable;
    if (id == "high_roller") return ach_highRoller;
    if (id == "millionaire") return ach_millionaire;
    if (id == "all_in") return ach_allIn;
    if (id == "yolo") return ach_yolo;
    if (id == "twenty_one") return ach_twentyOne;
    if (id == "twice_as_nice") return ach_twiceAsNice;
    if (id == "bust") return ach_bust;
    if (id == "house_always_wins") return ach_houseAlwaysWins;
    if (id == "debt_free") return ach_debtFree;
    return false;
}

void unlockAchievement(const std::string& id) {
    if (id == "first_steps") ach_firstSteps = true;
    else if (id == "first_win") ach_firstWin = true;
    else if (id == "first_loss") ach_firstLoss = true;
    else if (id == "first_push") ach_firstPush = true;
    else if (id == "on_a_roll") ach_onARoll = true;
    else if (id == "unstoppable") ach_unstoppable = true;
    else if (id == "house_watching") ach_houseWatching = true;
    else if (id == "rough_night") ach_roughNight = true;
    else if (id == "rock_bottom") ach_rockBottom = true;
    else if (id == "pocket_change") ach_pocketChange = true;
    else if (id == "comfortable") ach_comfortable = true;
    else if (id == "high_roller") ach_highRoller = true;
    else if (id == "millionaire") ach_millionaire = true;
    else if (id == "all_in") ach_allIn = true;
    else if (id == "yolo") ach_yolo = true;
    else if (id == "twenty_one") ach_twentyOne = true;
    else if (id == "twice_as_nice") ach_twiceAsNice = true;
    else if (id == "bust") ach_bust = true;
    else if (id == "house_always_wins") ach_houseAlwaysWins = true;
    else if (id == "debt_free") ach_debtFree = true;
}

void saveGame() {
    std::ostringstream plain;
    plain << totalChips << "\n";
    plain << totalDebt << "\n";
    plain << hasLuckyCharm << "\n";
    plain << hasLuckyStreak << "\n";
    plain << hasInsurancePolicy << "\n";
    plain << cardCounterCount << "\n";
    plain << housesMercyCount << "\n";
    plain << gamesPlayed << "\n";
    plain << gamesWon << "\n";
    plain << gamesLost << "\n";
    plain << hasBeatenGame << "\n";
    plain << currentWinStreak << "\n";
    plain << currentLossStreak << "\n";
    plain << longestWinStreak << "\n";
    plain << longestLossStreak << "\n";
    plain << biggestWin << "\n";
    plain << biggestLoss << "\n";
    plain << totalChipsWon << "\n";
    plain << totalChipsLost << "\n";
    plain << totalWagered << "\n";
    plain << blackjacksHit << "\n";
    plain << playerBusts << "\n";
    plain << dealerBusts << "\n";
    plain << pushes << "\n";
    plain << allInsMade << "\n";
    plain << totalDebtPaid << "\n";
    plain << highestChipsEver << "\n";
    plain << ach_firstSteps << "\n";
    plain << ach_firstWin << "\n";
    plain << ach_firstLoss << "\n";
    plain << ach_firstPush << "\n";
    plain << ach_onARoll << "\n";
    plain << ach_unstoppable << "\n";
    plain << ach_houseWatching << "\n";
    plain << ach_roughNight << "\n";
    plain << ach_rockBottom << "\n";
    plain << ach_pocketChange << "\n";
    plain << ach_comfortable << "\n";
    plain << ach_highRoller << "\n";
    plain << ach_millionaire << "\n";
    plain << ach_allIn << "\n";
    plain << ach_yolo << "\n";
    plain << ach_twentyOne << "\n";
    plain << ach_twiceAsNice << "\n";
    plain << ach_bust << "\n";
    plain << ach_houseAlwaysWins << "\n";
    plain << ach_debtFree << "\n";
    plain << playerName << "\n";

    std::string data = DISABLE_SAVE_ENCRYPTION ? plain.str() : xorCipher(plain.str(), SAVE_KEY);

    std::ofstream saveFile("save.txt", std::ios::binary);
    if (saveFile.is_open()) {
        saveFile.write(data.data(), data.size());
        saveFile.close();
    }
}

void loadGame() {
    std::ifstream saveFile("save.txt", std::ios::binary);
    if (saveFile.is_open()) {
        std::string data((std::istreambuf_iterator<char>(saveFile)),
                         std::istreambuf_iterator<char>());
        saveFile.close();

        std::string plain = DISABLE_SAVE_ENCRYPTION ? data : xorCipher(data, SAVE_KEY);

        std::istringstream stream(plain);
        stream >> totalChips;
        stream >> totalDebt;
        stream >> hasLuckyCharm;
        stream >> hasLuckyStreak;
        stream >> hasInsurancePolicy;
        stream >> cardCounterCount;
        stream >> housesMercyCount;
        stream >> gamesPlayed;
        stream >> gamesWon;
        stream >> gamesLost;
        stream >> hasBeatenGame;
        stream >> currentWinStreak;
        stream >> currentLossStreak;
        stream >> longestWinStreak;
        stream >> longestLossStreak;
        stream >> biggestWin;
        stream >> biggestLoss;
        stream >> totalChipsWon;
        stream >> totalChipsLost;
        stream >> totalWagered;
        stream >> blackjacksHit;
        stream >> playerBusts;
        stream >> dealerBusts;
        stream >> pushes;
        stream >> allInsMade;
        stream >> totalDebtPaid;
        stream >> highestChipsEver;
        stream >> ach_firstSteps;
        stream >> ach_firstWin;
        stream >> ach_firstLoss;
        stream >> ach_firstPush;
        stream >> ach_onARoll;
        stream >> ach_unstoppable;
        stream >> ach_houseWatching;
        stream >> ach_roughNight;
        stream >> ach_rockBottom;
        stream >> ach_pocketChange;
        stream >> ach_comfortable;
        stream >> ach_highRoller;
        stream >> ach_millionaire;
        stream >> ach_allIn;
        stream >> ach_yolo;
        stream >> ach_twentyOne;
        stream >> ach_twiceAsNice;
        stream >> ach_bust;
        stream >> ach_houseAlwaysWins;
        stream >> ach_debtFree;
        stream >> playerName;
    }
}
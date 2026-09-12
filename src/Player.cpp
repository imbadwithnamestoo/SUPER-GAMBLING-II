#include "Player.h"
#include <fstream>
#include <string>
#include <sstream>

int totalChips = 250;
int totalDebt = 100000;
bool hasLuckyCharm = false;
bool hasInsurancePolicy = false;
int cardCounterCount = 0;
int gamesPlayed = 0;
int gamesWon = 0;
int gamesLost = 0;
bool hasBeatenGame = false;

static const std::string SAVE_KEY = "SuperGamblingII_secret_key_2025";

std::string xorCipher(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

void saveGame() {
    std::ostringstream plain;
    plain << totalChips << "\n";
    plain << totalDebt << "\n";
    plain << hasLuckyCharm << "\n";
    plain << hasInsurancePolicy << "\n";
    plain << cardCounterCount << "\n";
    plain << gamesPlayed << "\n";
    plain << gamesWon << "\n";
    plain << gamesLost << "\n";
    plain << hasBeatenGame << "\n";

    std::string encrypted = xorCipher(plain.str(), SAVE_KEY);

    std::ofstream saveFile("save.txt", std::ios::binary);
    if (saveFile.is_open()) {
        saveFile.write(encrypted.data(), encrypted.size());
        saveFile.close();
    }
}

void loadGame() {
    std::ifstream saveFile("save.txt", std::ios::binary);
    if (saveFile.is_open()) {
        std::string encrypted((std::istreambuf_iterator<char>(saveFile)),
                              std::istreambuf_iterator<char>());
        saveFile.close();

        std::string plain = xorCipher(encrypted, SAVE_KEY);

        std::istringstream stream(plain);
        stream >> totalChips;
        stream >> totalDebt;
        stream >> hasLuckyCharm;
        stream >> hasInsurancePolicy;
        stream >> cardCounterCount;
        stream >> gamesPlayed;
        stream >> gamesWon;
        stream >> gamesLost;
        stream >> hasBeatenGame;
    }
}
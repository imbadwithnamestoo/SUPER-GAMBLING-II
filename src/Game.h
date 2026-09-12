#pragma once
#include <vector>
#include <string>

struct Card {
    std::string suit;
    std::string rank;
    int value;
};

std::vector<Card> createDeck();
void shuffleDeck(std::vector<Card>& deck);
void dealCard(std::vector<Card>& deck, std::vector<Card>& hand);
int calculateScore(const std::vector<Card>& hand);
bool isBlackjack(const std::vector<Card>& hand);
std::string getHandString(const std::vector<Card>& hand, bool hideFirstCard);
void playBlackjackGame();
int placeBet(int maxBet);
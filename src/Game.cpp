#include "Game.h"
#include "UI.h"
#include "Player.h"
#include "Utils.h"
#include "Dialogue.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <cstdio>
#include <cctype>

std::vector<Card> createDeck() {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"♥", "♦", "♣", "♠"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    for (const auto& suit : suits) {
        for (size_t i = 0; i < ranks.size(); ++i) {
            int val = 0;
            if (i < 9) {
                val = std::stoi(ranks[i]);
            } else if (ranks[i] == "A") {
                val = 11;
            } else {
                val = 10;
            }
            deck.push_back({suit, ranks[i], val});
        }
    }
    return deck;
}

void shuffleDeck(std::vector<Card>& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void dealCard(std::vector<Card>& deck, std::vector<Card>& hand) {
    hand.push_back(deck.back());
    deck.pop_back();
}

void dealCardBiased(std::vector<Card>& deck, std::vector<Card>& hand) {
    Card card = deck.back();
    deck.pop_back();

    int score = calculateScore(hand);

    if (score >= 14) {
        if (std::rand() % 100 < 35) {
            std::vector<size_t> safeIndices;
            for (size_t i = 0; i < deck.size(); ++i) {
                if (score + deck[i].value <= 21) {
                    safeIndices.push_back(i);
                }
            }

            if (!safeIndices.empty()) {
                size_t pick = safeIndices[std::rand() % safeIndices.size()];
                std::swap(deck[pick], card);
            }
        }
    }

    hand.push_back(card);
}

void dealDealerOpening(std::vector<Card>& deck, std::vector<Card>& hand) {
    while (true) {
        Card c1 = deck.back();
        deck.pop_back();
        Card c2 = deck.back();
        deck.pop_back();

        std::vector<Card> temp = {c1, c2};
        int score = calculateScore(temp);

        if (score != 20 && score != 21) {
            hand.push_back(c1);
            hand.push_back(c2);
            return;
        }

        deck.insert(deck.begin(), c1);
        deck.insert(deck.begin(), c2);
    }
}

int calculateScore(const std::vector<Card>& hand) {
    int total = 0;
    int aceCount = 0;
    for (const auto& card : hand) {
        total += card.value;
        if (card.rank == "A") {
            aceCount++;
        }
    }
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    return total;
}

bool isBlackjack(const std::vector<Card>& hand) {
    return hand.size() == 2 && calculateScore(hand) == 21;
}

std::string getHandString(const std::vector<Card>& hand, bool hideFirstCard) {
    if (hand.empty()) return "";
    std::string result = "";
    if (hideFirstCard) {
        result += "[???] ";
        for (size_t i = 1; i < hand.size(); ++i) {
            result += hand[i].rank + hand[i].suit + (i == hand.size() - 1 ? "" : " ");
        }
    } else {
        for (size_t i = 0; i < hand.size(); ++i) {
            std::string suitColor = (hand[i].suit == "♥" || hand[i].suit == "♦") ? "\033[31m" : "\033[0m";
            result += suitColor + hand[i].rank + hand[i].suit + T_RESET + (i == hand.size() - 1 ? "" : " ");
        }
    }
    return result;
}

int placeBet(int maxBet) {
    clear();
    int bet = 0;
    bool validBet = false;
    
    while (!validBet) {
        resetCursor();
        int termWidth = getTerminalWidth();
        int boxWidth = std::min(60, termWidth - 4);
        int leftPadding = (termWidth - boxWidth) / 2;
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        
        std::string title = " PLACE YOUR BET ";
        printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
        
        std::string chipsDisplay = "Chips: " + T_PRICE + formatNumber(totalChips) + T_RESET;
        printBoxLine(chipsDisplay, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        std::string betPrompt = "Enter your bet (1 - " + std::to_string(maxBet) + ") or 0 to go back: ";
        printBoxLine(betPrompt, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
        
        std::cout << std::string(leftPadding, ' ') << "> ";
        
        if (!readInt(bet)) {
            clear();
            continue;
        }
        
        if (bet == 0) {
            return 0;
        }
        
        if (bet < 1) {
            clear();
            continue;
        } else if (bet > maxBet) {
            clear();
            continue;
        } else {
            validBet = true;
        }
    }
    return bet;
}

bool playEndingSequence() {
    clear();
    int termWidth = getTerminalWidth();
    int boxWidth = std::min(60, termWidth - 4);
    int leftPadding = (termWidth - boxWidth) / 2;

    std::vector<std::string> lines = {
        "Well.",
        "Look at that.",
        "Most people don't come back from a debt like yours.",
        "They disappear. Or worse.",
        "But you? You sat down. You played. You paid.",
        "Huh."
    };

    for (const auto& line : lines) {
        clear();
        resetCursor();
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        printBoxLine(T_INFO + line + T_RESET, boxWidth, leftPadding);
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
        
        std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
        _getch();
    }

    clear();
    resetCursor();
    
    std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    printBoxLine(T_TITLE + "Thank you for playing" + T_RESET, boxWidth, leftPadding);
    printBoxLine(T_TITLE + "Super Gambling II !" + T_RESET, boxWidth, leftPadding);
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
    std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
    
    std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
    _getch();

    if (playerName.empty()) {
        clear();
        resetCursor();
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        printBoxLine(T_INFO + "What do we call you?" + T_RESET, boxWidth, leftPadding);
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
        
        std::cout << std::string(leftPadding, ' ') << "> ";
        
        std::string rawInput;
        std::getline(std::cin, rawInput);
        
        std::string clean = sanitizeName(rawInput);
        if (clean.empty()) {
            clean = "Kid";
        }
        playerName = clean;
        
        clear();
        resetCursor();
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        printBoxLine(T_INFO + playerName + "." + T_RESET, boxWidth, leftPadding);
        printBoxLine(T_INFO + "I'll remember it." + T_RESET, boxWidth, leftPadding);
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
        
        std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
        _getch();
    }

    return true;
}

void playBlackjackGame() {
    clear();

    if (totalDebt <= 0 && !hasBeatenGame) {
        playEndingSequence();
        hasBeatenGame = true;
        if (!hasAchievement("debt_free")) unlockAchievement("debt_free");
        saveGame();
        return;
    }

    int maxBet = totalChips;
    if (maxBet < 1) {
        int termWidth = getTerminalWidth();
        int boxWidth = std::min(70, termWidth - 4);
        int leftPadding = (termWidth - boxWidth) / 2;

        if (housesMercyCount > 0) {
            housesMercyCount--;
            if (hasBeatenGame) {
                totalChips = 1000;
            } else {
                totalChips = 250;
                totalDebt += 25000;
            }
            saveGame();

            std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
            std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
            std::string line1 = "The house isn't done with you yet.";
            printBoxLine(T_INFO + line1 + T_RESET, boxWidth, leftPadding);
            std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
            std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
            std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
            _getch();

            clear();
            std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
            std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
            std::string line2 = hasBeatenGame ? "+1000 Chips." : "+250 Chips. +25000 Debt.";
            printBoxLine(T_LOSS + line2 + T_RESET, boxWidth, leftPadding);
            std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
            std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
            std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
            _getch();
            return;
        }

        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        
        std::string line = "You know what happens when you can't pay us back, right?";
        printBoxLine(T_LOSS + line + T_RESET, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "║" << std::string(boxWidth - 2, ' ') << "║" << std::endl;
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
        
        std::cout << std::string(leftPadding, ' ') << "Press Enter to continue...";
        _getch();
        
        std::remove("save.txt");
        std::exit(0);
    }
    
    int currentBet = placeBet(maxBet);
    if (currentBet == 0) {
        return;
    }
    
    flushInput();
    
    totalChips -= currentBet;
    totalWagered += currentBet;
    if (currentBet == maxBet) {
        allInsMade++;
        if (!hasAchievement("all_in")) unlockAchievement("all_in");
    }
    saveGame();

    bool isAllIn = (currentBet == maxBet);
    bool isMinBet = (currentBet == 1);
    bool isTwoBet = (currentBet == 2);
    bool isHugeBet = (maxBet > 0 && currentBet >= maxBet / 2);

    std::vector<Card> deck = createDeck();
    shuffleDeck(deck);

    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;

    dealCardBiased(deck, playerHand);
    dealCard(deck, dealerHand);
    dealCardBiased(deck, playerHand);
    dealDealerOpening(deck, dealerHand);

    flushInput();

    std::string dealerDialogue = getOpeningLine(isAllIn, isMinBet, isTwoBet, hasBeatenGame);

    int selectedOption = 1;
    int boxWidth = 0;
    int leftPadding = 0;
    bool revealDealer = false;

    while (true) {
        printBlackjackUI(playerHand, dealerHand, dealerDialogue, !revealDealer, false);

        int playerScore = calculateScore(playerHand);

        if (playerScore >= 21) {
            break;
        }

        int termWidth = getTerminalWidth();
        if (termWidth <= 4) {
            termWidth = 80;
        }

        boxWidth = std::min(70, termWidth - 4);
        leftPadding = (termWidth - boxWidth) / 2;
        int innerWidth = boxWidth - 2;

        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;

        std::vector<std::string> actionOptions = {"Hit", "Stand", "Items"};

        for (size_t i = 0; i < actionOptions.size(); ++i) {
            bool selected = selectedOption == static_cast<int>(i) + 1;
            int textLen = visibleLength(actionOptions[i]);
            int textStart = (innerWidth - textLen) / 2;
            std::string displayText;

            if (selected) {
                int leftSpaces = std::max(0, textStart - 2);
                int rightSpaces = innerWidth - leftSpaces - 2 - textLen - 2;
                rightSpaces = std::max(0, rightSpaces);
                std::string color = (i == 0) ? T_WIN : (i == 1) ? T_LOSS : T_SPECIAL;

                displayText = repeatStr(" ", leftSpaces) + color + "► " + actionOptions[i] + " ◄" + T_RESET + repeatStr(" ", rightSpaces);
            } else {
                int leftSpaces = textStart;
                int rightSpaces = innerWidth - leftSpaces - textLen;
                rightSpaces = std::max(0, rightSpaces);

                displayText = repeatStr(" ", leftSpaces) + actionOptions[i] + repeatStr(" ", rightSpaces);
            }

            std::cout << std::string(leftPadding, ' ') << "║" << displayText << "║" << std::endl;
        }

        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;

        int input = _getch();

        if (input == 224 || input == 27) {
            int arrow = _getch();
            if (input == 27 && arrow == 91) {
                arrow = _getch();
            }

            if (arrow == 72 || arrow == 'A') {
                if (selectedOption > 1) {
                    selectedOption--;
                } else {
                    selectedOption = 3;
                }
            } else if (arrow == 80 || arrow == 'B') {
                if (selectedOption < 3) {
                    selectedOption++;
                } else {
                    selectedOption = 1;
                }
            }
        } else if (input == 13 || input == 10) {
            if (selectedOption == 1) {
                dealCardBiased(deck, playerHand);
                dealerDialogue = "Taking another card? Bold move.";
                selectedOption = 1;
            } else if (selectedOption == 2) {
                dealerDialogue = "Standing? Let's see if it pays off.";
                break;
            } else if (selectedOption == 3) {
                showItemsMenu(boxWidth, leftPadding, revealDealer, cardCounterCount);
                selectedOption = 1;
            }
        }
    }

    int playerScore = calculateScore(playerHand);
    gamesPlayed++;
    
    if (!hasAchievement("first_steps")) unlockAchievement("first_steps");
    
    if (isBlackjack(playerHand)) {
        blackjacksHit++;
        if (!hasAchievement("twenty_one")) unlockAchievement("twenty_one");
        if (blackjacksHit >= 10 && !hasAchievement("twice_as_nice")) unlockAchievement("twice_as_nice");
    }

    if (playerScore > 21) {
        gamesLost++;
        playerBusts++;
        currentLossStreak++;
        currentWinStreak = 0;
        if (currentLossStreak > longestLossStreak) longestLossStreak = currentLossStreak;
        
        if (!hasAchievement("bust")) unlockAchievement("bust");
        if (!hasAchievement("first_loss")) unlockAchievement("first_loss");
        if (currentLossStreak >= 5 && !hasAchievement("rough_night")) unlockAchievement("rough_night");
        if (currentLossStreak >= 10 && !hasAchievement("rock_bottom")) unlockAchievement("rock_bottom");
        
        int lossAmount = currentBet;
        totalChipsLost += lossAmount;
        if (lossAmount > biggestLoss) biggestLoss = lossAmount;
        
        if (totalChipsLost >= 100000 && !hasAchievement("house_always_wins")) unlockAchievement("house_always_wins");
        
        bool firstLoss = (gamesLost == 1);
        if (isAllIn) {
            dealerDialogue = getAllInLossLine(hasBeatenGame);
        } else {
            dealerDialogue = getLossLine(firstLoss, isHugeBet, hasBeatenGame);
        }
        if (hasInsurancePolicy) {
            int refund = currentBet / 10;
            totalChips += refund;
        }
        saveGame();
        printBlackjackUI(playerHand, dealerHand, dealerDialogue, false, true);

        std::string continueMsg = "Press Enter to return to menu...";
        int termWidth = getTerminalWidth();
        int padding = (termWidth - static_cast<int>(continueMsg.length())) / 2;
        if (padding < 0) padding = 0;

        std::cout << std::string(padding, ' ') << continueMsg;
        std::cin.get();
        return;
    }

    dealerDialogue = "My turn. Let's see how close I can get.";
    printBlackjackUI(playerHand, dealerHand, dealerDialogue, false, false);
    wait(1000);

    while (calculateScore(dealerHand) < 16) {
        dealerDialogue = "Dealer hits...";
        dealCard(deck, dealerHand);
        printBlackjackUI(playerHand, dealerHand, dealerDialogue, false, false);
        wait(1000);
    }

    int dealerScore = calculateScore(dealerHand);

    if (dealerScore > 21) {
        gamesWon++;
        dealerBusts++;
        currentWinStreak++;
        currentLossStreak = 0;
        if (currentWinStreak > longestWinStreak) longestWinStreak = currentWinStreak;
        
        if (!hasAchievement("first_win")) unlockAchievement("first_win");
        if (currentWinStreak >= 5 && !hasAchievement("on_a_roll")) unlockAchievement("on_a_roll");
        if (currentWinStreak >= 10 && !hasAchievement("unstoppable")) unlockAchievement("unstoppable");
        if (currentWinStreak >= 20 && !hasAchievement("house_watching")) unlockAchievement("house_watching");
        
        if (isAllIn && !hasAchievement("yolo")) unlockAchievement("yolo");
        
        if (isAllIn) {
            dealerDialogue = getAllInWinLine(hasBeatenGame);
        } else {
            dealerDialogue = getDealerBustLine(hasBeatenGame);
        }
        int winnings = (currentBet * 5 + 1) / 2;
        if (hasLuckyCharm) winnings += winnings / 10;
        if (hasLuckyStreak) {
            int streakBonus = std::min(currentWinStreak, 5) * 10;
            winnings += winnings * streakBonus / 100;
        }
        totalChips += winnings;
        totalChipsWon += (winnings - currentBet);
        if ((winnings - currentBet) > biggestWin) biggestWin = winnings - currentBet;
        if (totalChips > highestChipsEver) highestChipsEver = totalChips;
        
        if (totalChips >= 1000 && !hasAchievement("pocket_change")) unlockAchievement("pocket_change");
        if (totalChips >= 10000 && !hasAchievement("comfortable")) unlockAchievement("comfortable");
        if (totalChips >= 100000 && !hasAchievement("high_roller")) unlockAchievement("high_roller");
        if (totalChips >= 1000000 && !hasAchievement("millionaire")) unlockAchievement("millionaire");
        
        saveGame();
    } else if (playerScore > dealerScore) {
        gamesWon++;
        currentWinStreak++;
        currentLossStreak = 0;
        if (currentWinStreak > longestWinStreak) longestWinStreak = currentWinStreak;
        
        if (!hasAchievement("first_win")) unlockAchievement("first_win");
        if (currentWinStreak >= 5 && !hasAchievement("on_a_roll")) unlockAchievement("on_a_roll");
        if (currentWinStreak >= 10 && !hasAchievement("unstoppable")) unlockAchievement("unstoppable");
        if (currentWinStreak >= 20 && !hasAchievement("house_watching")) unlockAchievement("house_watching");
        
        if (isAllIn && !hasAchievement("yolo")) unlockAchievement("yolo");
        
        bool firstWin = (gamesWon == 1);
        if (isAllIn) {
            dealerDialogue = getAllInWinLine(hasBeatenGame);
        } else {
            dealerDialogue = getWinLine(firstWin, isHugeBet, hasBeatenGame);
        }
        int winnings = currentBet * 2;
        if (hasLuckyCharm) winnings += winnings / 10;
        if (hasLuckyStreak) {
            int streakBonus = std::min(currentWinStreak, 5) * 10;
            winnings += winnings * streakBonus / 100;
        }
        totalChips += winnings;
        totalChipsWon += (winnings - currentBet);
        if ((winnings - currentBet) > biggestWin) biggestWin = winnings - currentBet;
        if (totalChips > highestChipsEver) highestChipsEver = totalChips;
        
        if (totalChips >= 1000 && !hasAchievement("pocket_change")) unlockAchievement("pocket_change");
        if (totalChips >= 10000 && !hasAchievement("comfortable")) unlockAchievement("comfortable");
        if (totalChips >= 100000 && !hasAchievement("high_roller")) unlockAchievement("high_roller");
        if (totalChips >= 1000000 && !hasAchievement("millionaire")) unlockAchievement("millionaire");
        
        saveGame();
    } else if (playerScore < dealerScore) {
        gamesLost++;
        currentLossStreak++;
        currentWinStreak = 0;
        if (currentLossStreak > longestLossStreak) longestLossStreak = currentLossStreak;
        
        if (!hasAchievement("first_loss")) unlockAchievement("first_loss");
        if (currentLossStreak >= 5 && !hasAchievement("rough_night")) unlockAchievement("rough_night");
        if (currentLossStreak >= 10 && !hasAchievement("rock_bottom")) unlockAchievement("rock_bottom");
        
        totalChipsLost += currentBet;
        if (currentBet > biggestLoss) biggestLoss = currentBet;
        
        if (totalChipsLost >= 100000 && !hasAchievement("house_always_wins")) unlockAchievement("house_always_wins");
        
        bool firstLoss = (gamesLost == 1);
        if (isAllIn) {
            dealerDialogue = getAllInLossLine(hasBeatenGame);
        } else {
            dealerDialogue = getLossLine(firstLoss, isHugeBet, hasBeatenGame);
        }
        if (hasInsurancePolicy) {
            int refund = currentBet / 10;
            totalChips += refund;
        }
        saveGame();
    } else {
        pushes++;
        currentWinStreak = 0;
        currentLossStreak = 0;
        if (!hasAchievement("first_push")) unlockAchievement("first_push");
        dealerDialogue = getPushLine(hasBeatenGame);
        totalChips += currentBet;
        saveGame();
    }

    printBlackjackUI(playerHand, dealerHand, dealerDialogue, false, true);
    std::cout << std::string(leftPadding, ' ') << "Press Enter to return to menu...";
    std::cin.get();
}
#include "Game.h"
#include "UI.h"
#include "Player.h"
#include "Utils.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <cstdio>

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
        std::cin >> bet;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear();
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
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

std::string pickRandom(const std::vector<std::string>& options) {
    return options[std::rand() % options.size()];
}

std::string getOpeningLine() {
    std::vector<std::string> lines = {
        "Let's see what you've got.",
        "Careful now, the deck is feeling cold.",
        "The house always has an ace up its sleeve.",
        "Feeling lucky today, kid?",
        "Don't go blowing all your chips at once."
    };
    return pickRandom(lines);
}

std::string getAllInWinLine() {
    std::vector<std::string> lines = {
        "All in... and you won. Hm.",
        "The whole stack. Doubled. Don't let it go to your head.",
        "Bold. And lucky. Dangerous combination.",
        "You just doubled your entire worth. Remember that feeling."
    };
    return pickRandom(lines);
}

std::string getAllInLossLine() {
    std::vector<std::string> lines = {
        "All in... and it's gone. All of it.",
        "The whole stack. Down the drain.",
        "That's the game, kid. Nothing left to bet.",
        "Back to the bottom. Or the Black Market."
    };
    return pickRandom(lines);
}

std::string getDealerBustLine() {
    std::vector<std::string> lines = {
        "Busted. The deck turned on me.",
        "Over. Take it. This round is yours.",
        "Well. That's embarrassing.",
        "The house breaks. Enjoy it while it lasts."
    };
    return pickRandom(lines);
}

std::string getWinLine(bool firstWin, bool hugeBet, int bet, int maxBet) {
    if (firstWin) {
        return "Beginner's luck. Enjoy it while it lasts.";
    }
    if (hugeBet) {
        std::vector<std::string> lines = {
            "...noted.",
            "Hm. That's a big one.",
            "The house remembers this.",
            "Don't get used to it."
        };
        return pickRandom(lines);
    }
    std::vector<std::string> lines = {
        "Got lucky.",
        "You beat me clean. Nice hand.",
        "Hmph. Enjoy it.",
        "The deck likes you tonight."
    };
    return pickRandom(lines);
}

std::string getLossLine(bool firstLoss) {
    if (firstLoss) {
        return "Welcome to the real game.";
    }
    std::vector<std::string> lines = {
        "House takes this round.",
        "Better luck next time.",
        "The deck is cold tonight.",
        "You'll get 'em next hand."
    };
    return pickRandom(lines);
}

std::string getPushLine() {
    std::vector<std::string> lines = {
        "A push. We trade even.",
        "Tie. Nobody wins, nobody loses.",
        "Dead even. How boring.",
        "A wash. Try again.",
        "Same score. The house shrugs."
    };
    return pickRandom(lines);
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

    return true;
}

void playBlackjackGame() {
    clear();

    if (totalDebt <= 0 && !hasBeatenGame) {
        playEndingSequence();
        hasBeatenGame = true;
        saveGame();
        return;
    }

    int maxBet = totalChips;
    if (maxBet < 1) {
        int termWidth = getTerminalWidth();
        int boxWidth = std::min(70, termWidth - 4);
        int leftPadding = (termWidth - boxWidth) / 2;
        
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
    
    totalChips -= currentBet;
    saveGame();

    bool isAllIn = (currentBet == maxBet);
    bool isMinBet = (currentBet == 1);
    bool isTwoBet = (currentBet == 2);
    bool isHugeBet = (maxBet > 0 && currentBet >= maxBet / 2);

    std::vector<Card> deck = createDeck();
    shuffleDeck(deck);

    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;

    dealCard(deck, playerHand);
    dealCard(deck, dealerHand);
    dealCard(deck, playerHand);
    dealCard(deck, dealerHand);

    std::string dealerDialogue;
    
    if (isAllIn) {
        std::vector<std::string> lines = {
            "All in? Bold. Stupid, but bold.",
            "Everything on the line. I like it.",
            "All in. Let's see if you've got the nerve.",
            "Going for broke? Fine by me."
        };
        dealerDialogue = pickRandom(lines);
    } else if (isMinBet) {
        std::vector<std::string> lines = {
            "Really? That's your play?",
            "One chip. You're really going for it.",
            "Playing it safe, huh?",
            "A single chip. Bold strategy."
        };
        dealerDialogue = pickRandom(lines);
    } else if (isTwoBet) {
        dealerDialogue = "Ah, two chips. You've clearly heard the one-chip line before.";
    } else {
        dealerDialogue = getOpeningLine();
    }

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
                dealCard(deck, playerHand);
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

    if (playerScore > 21) {
        gamesLost++;
        bool firstLoss = (gamesLost == 1);
        if (isAllIn) {
            dealerDialogue = getAllInLossLine();
        } else {
            dealerDialogue = getLossLine(firstLoss);
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

    while (calculateScore(dealerHand) < 17) {
        dealerDialogue = "Dealer hits...";
        dealCard(deck, dealerHand);
        printBlackjackUI(playerHand, dealerHand, dealerDialogue, false, false);
        wait(1000);
    }

    int dealerScore = calculateScore(dealerHand);

    if (dealerScore > 21) {
        gamesWon++;
        if (isAllIn) {
            dealerDialogue = getAllInWinLine();
        } else {
            dealerDialogue = getDealerBustLine();
        }
        int winnings = currentBet * 2;
        if (hasLuckyCharm) winnings += winnings / 10;
        totalChips += winnings;
        saveGame();
    } else if (playerScore > dealerScore) {
        gamesWon++;
        bool firstWin = (gamesWon == 1);
        if (isAllIn) {
            dealerDialogue = getAllInWinLine();
        } else {
            dealerDialogue = getWinLine(firstWin, isHugeBet, currentBet, maxBet);
        }
        int winnings = currentBet * 2;
        if (hasLuckyCharm) winnings += winnings / 10;
        totalChips += winnings;
        saveGame();
    } else if (playerScore < dealerScore) {
        gamesLost++;
        bool firstLoss = (gamesLost == 1);
        if (isAllIn) {
            dealerDialogue = getAllInLossLine();
        } else {
            dealerDialogue = getLossLine(firstLoss);
        }
        if (hasInsurancePolicy) {
            int refund = currentBet / 10;
            totalChips += refund;
        }
        saveGame();
    } else {
        dealerDialogue = getPushLine();
        totalChips += currentBet;
        saveGame();
    }

    printBlackjackUI(playerHand, dealerHand, dealerDialogue, false, true);
    std::cout << std::string(leftPadding, ' ') << "Press Enter to return to menu...";
    std::cin.get();
}

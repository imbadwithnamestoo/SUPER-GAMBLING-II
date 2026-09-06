#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>

struct Card {
    std::string suit;
    std::string rank;
    int value;
};

std::vector<Card> createDeck() {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    
    for (const auto& suit : suits) {
        for (size_t i = 0; i < ranks.size(); ++i) {
            int val = 0;
            if (i < 9) {
                val = std::stoi(ranks[i]);
            } else if (ranks[i] == "Ace") {
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
        if (card.rank == "Ace") {
            aceCount++;
        }
    }
    
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    
    return total;
}

void displayHand(const std::vector<Card>& hand, const std::string& owner, bool hideFirstCard = false) {
    std::cout << owner << "'s hand: ";
    if (hideFirstCard && hand.size() > 0) {
        std::cout << "[Hidden Card], ";
        for (size_t i = 1; i < hand.size(); ++i) {
            std::cout << hand[i].rank << " of " << hand[i].suit << (i == hand.size() - 1 ? "" : ", ");
        }
        std::cout << "\n";
    } else {
        for (size_t i = 0; i < hand.size(); ++i) {
            std::cout << hand[i].rank << " of " << hand[i].suit << (i == hand.size() - 1 ? "" : ", ");
        }
        std::cout << " (Total: " << calculateScore(hand) << ")\n";
    }
}

int main() {
    std::cout << "=== Welcome to Basic C++ Blackjack ===\n\n";
    
    std::vector<Card> deck = createDeck();
    shuffleDeck(deck);
    
    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;
    
    dealCard(deck, playerHand);
    dealCard(deck, dealerHand);
    dealCard(deck, playerHand);
    dealCard(deck, dealerHand);
    
    char choice;
    while (true) {
        displayHand(playerHand, "Player");
        displayHand(dealerHand, "Dealer", true);
        
        int playerScore = calculateScore(playerHand);
        if (playerScore >= 21) {
            break;
        }
        
        std::cout << "Do you want to (h)it or (s)tand? ";
        std::cin >> choice;
        std::cout << "\n";
        
        if (choice == 'h' || choice == 'H') {
            dealCard(deck, playerHand);
            std::cout << "You chose to hit!\n";
        } else if (choice == 's' || choice == 'S') {
            std::cout << "You chose to stand.\n";
            break;
        } else {
            std::cout << "Invalid choice. Please enter 'h' or 's'.\n";
        }
    }
    
    int playerScore = calculateScore(playerHand);
    
    if (playerScore > 21) {
        displayHand(playerHand, "Player");
        std::cout << "\n*** You busted! Dealer wins. ***\n";
        return 0;
    }
    
    std::cout << "\n--- Dealer's Turn ---\n";
    displayHand(dealerHand, "Dealer");
    
    while (calculateScore(dealerHand) < 17) {
        std::cout << "Dealer hits...\n";
        dealCard(deck, dealerHand);
        displayHand(dealerHand, "Dealer");
    }
    
    int dealerScore = calculateScore(dealerHand);
    
    std::cout << "\n=== Final Results ===\n";
    std::cout << "Player Total: " << playerScore << "\n";
    std::cout << "Dealer Total: " << dealerScore << "\n\n";
    
    if (dealerScore > 21) {
        std::cout << "*** Dealer busted! You win! ***\n";
    } else if (playerScore > dealerScore) {
        std::cout << "*** You win! ***\n";
    } else if (playerScore < dealerScore) {
        std::cout << "*** Dealer wins. ***\n";
    } else {
        std::cout << "*** It's a tie (push)! ***\n";
    }
    
    return 0;
}

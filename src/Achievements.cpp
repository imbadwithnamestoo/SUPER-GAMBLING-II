#include "Achievements.h"
#include "UI.h"
#include "Player.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <vector>

struct AchEntry {
    std::string id;
    std::string name;
    std::string desc;
    bool unlocked;
};

void viewBadges() {
    clear();
    int termWidth = getTerminalWidth();
    int boxWidth = std::min(76, termWidth - 4);
    int leftPadding = (termWidth - boxWidth) / 2;
    
    std::vector<AchEntry> entries = {
        {"debt_free", "Debt Free", "Pay off your entire debt", hasAchievement("debt_free")},
        {"first_steps", "First Steps", "Play your first hand", hasAchievement("first_steps")},
        {"first_win", "Beginner's Luck", "Win your first hand", hasAchievement("first_win")},
        {"first_loss", "Welcome to the Real Game", "Lose your first hand", hasAchievement("first_loss")},
        {"first_push", "Not a Total Loss", "Push your first hand", hasAchievement("first_push")},
        {"on_a_roll", "On a Roll", "Win 5 hands in a row", hasAchievement("on_a_roll")},
        {"unstoppable", "Unstoppable", "Win 10 hands in a row", hasAchievement("unstoppable")},
        {"house_watching", "The House Is Watching", "Win 20 hands in a row", hasAchievement("house_watching")},
        {"rough_night", "Rough Night", "Lose 5 hands in a row", hasAchievement("rough_night")},
        {"rock_bottom", "Rock Bottom", "Lose 10 hands in a row", hasAchievement("rock_bottom")},
        {"pocket_change", "Pocket Change", "Reach 1,000 chips", hasAchievement("pocket_change")},
        {"comfortable", "Comfortable", "Reach 10,000 chips", hasAchievement("comfortable")},
        {"high_roller", "High Roller", "Reach 100,000 chips", hasAchievement("high_roller")},
        {"millionaire", "Millionaire", "Reach 1,000,000 chips", hasAchievement("millionaire")},
        {"all_in", "All In", "Bet every chip you have", hasAchievement("all_in")},
        {"yolo", "YOLO", "Win an all-in bet", hasAchievement("yolo")},
        {"twenty_one", "Twenty-One", "Hit blackjack", hasAchievement("twenty_one")},
        {"twice_as_nice", "Twice as Nice", "Hit 10 blackjacks", hasAchievement("twice_as_nice")},
        {"bust", "Bust", "Go over 21", hasAchievement("bust")},
        {"house_always_wins", "House Always Wins", "Lose 100,000 chips total", hasAchievement("house_always_wins")}
    };
    
    int perPage = 10;
    int totalPages = (entries.size() + perPage - 1) / perPage;
    int page = 0;
    bool inMenu = true;
    
    while (inMenu) {
        resetCursor();
        
        std::cout << std::string(leftPadding, ' ') << "╔" << repeatStr("═", boxWidth - 2) << "╗" << std::endl;
        
        std::string title = " ACHIEVEMENTS (" + std::to_string(page + 1) + "/" + std::to_string(totalPages) + ") ";
        printBoxLine(T_TITLE + title + T_RESET, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        int start = page * perPage;
        int end = std::min(start + perPage, (int)entries.size());
        
        for (int i = start; i < end; ++i) {
            const auto& e = entries[i];
            
            std::string check;
            if (e.unlocked) {
                check = T_WIN + "[X]" + T_RESET;
            } else {
                check = T_LOSS + "[ ]" + T_RESET;
            }
            
            std::string line = "  " + check + " " + e.name;
            
            int padLen = 30 - (int)e.name.length();
            if (padLen < 1) padLen = 1;
            line += std::string(padLen, ' ');
            line += e.desc;
            
            printBoxLineLeft(line, boxWidth, leftPadding);
        }
        
        std::cout << std::string(leftPadding, ' ') << "╠" << repeatStr("═", boxWidth - 2) << "╣" << std::endl;
        
        std::string hint = "Left/Right to change page | Enter to return";
        printBoxLine(hint, boxWidth, leftPadding);
        
        std::cout << std::string(leftPadding, ' ') << "╚" << repeatStr("═", boxWidth - 2) << "╝" << std::endl;
        
        int input = _getch();
        if (input == 224 || input == 27) {
            int arrow = _getch();
            if (input == 27 && arrow == 91) {
                arrow = _getch();
            }
            if (arrow == 75 || arrow == 'D') {
                if (page > 0) page--;
                else page = totalPages - 1;
            } else if (arrow == 77 || arrow == 'C') {
                if (page < totalPages - 1) page++;
                else page = 0;
            }
        } else if (input == 13 || input == 10) {
            inMenu = false;
        }
    }
}
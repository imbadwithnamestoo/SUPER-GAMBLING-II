#include "Dialogue.h"
#include "Player.h"
#include <vector>
#include <cstdlib>

std::string pickRandom(const std::vector<std::string>& options) {
    return options[std::rand() % options.size()];
}

static std::string pickPreVictory(const std::vector<std::string>& lines) {
    return pickRandom(lines);
}

static std::string pickPostVictory(const std::vector<std::string>& lines) {
    return pickRandom(lines);
}

std::string getOpeningLine(bool isAllIn, bool isMinBet, bool isTwoBet, bool hasBeatenGame) {
    if (isAllIn) {
        if (hasBeatenGame) {
            std::vector<std::string> lines = {
                "All in, huh? That's freedom.",
                "Betting it all with nothing on the line. Wild.",
                "All in. Bold as ever."
            };
            return pickPostVictory(lines);
        }
        std::vector<std::string> lines = {
            "All in? Bold. Stupid, but bold.",
            "Everything on the line. I like it.",
            "All in. Let's see if you've got the nerve.",
            "Going for broke? Fine by me.",
            "Everything you've got. One hand.",
            "The whole stack on the table. Let's see.",
            "Bold move. Reckless. My favorite kind.",
            "All in. Win big or walk home empty."
        };
        return pickPreVictory(lines);
    }
    
    if (isMinBet) {
        std::vector<std::string> lines = {
            "Really? That's your play?",
            "One chip. You're really going for it.",
            "Playing it safe, huh?",
            "A single chip. Bold strategy.",
            "One chip. The house pays the rent on bets like this.",
            "Minimum bet. Minimum effort."
        };
        return pickRandom(lines);
    }
    
    if (isTwoBet) {
        return "Ah, two chips. You've clearly heard the one-chip line before.";
    }
    
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "Back for fun, " + playerName + "?",
            "No debt hanging over your head. Must be nice.",
            "Free man walks in. Let's see.",
            "You're not a debtor anymore. Just a player.",
            "The house remembers you, " + playerName + ". Fondly, even.",
            "What brings you back? Boredom?"
        };
        return pickPostVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "Let's see what you've got.",
        "Careful now, the deck is feeling cold.",
        "The house always has an ace up its sleeve.",
        "Feeling lucky today, kid?",
        "Don't go blowing all your chips at once.",
        "Another hand. Another chance to lose.",
        "You're still here? Good.",
        "The cards don't care about you. Neither do I.",
        "Place your bets. Or don't. Your call.",
        "Let's see if tonight's any different.",
        "Same table. Same rules. Same outcome, probably.",
        "You've got that look. The desperate kind.",
        "The house is patient. Very patient.",
        "Take your time. The debt isn't going anywhere.",
        "Alright. Show me what you've got."
    };
    return pickPreVictory(lines);
}

std::string getAllInWinLine(bool hasBeatenGame) {
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "All in. And you took it. Of course.",
            "Doubled again. Doesn't mean anything anymore, does it?",
            "Everything on the line and you still win. Showoff."
        };
        return pickPostVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "All in... and you won. Hm.",
        "The whole stack. Doubled. Don't let it go to your head.",
        "Bold. And lucky. Dangerous combination.",
        "You just doubled your entire worth. Remember that feeling.",
        "You bet everything. And got it back. Twice.",
        "That's the kind of move that ends lives. Yours, usually.",
        "Well. Look at that.",
        "Don't ever do that again."
    };
    return pickPreVictory(lines);
}

std::string getAllInLossLine(bool hasBeatenGame) {
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "All in and out. Happens to the best.",
            "Nothing on the line but pride. Still stings.",
            "You lost it all. And you're still smiling."
        };
        return pickPostVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "All in... and it's gone. All of it.",
        "The whole stack. Down the drain.",
        "That's the game, kid. Nothing left to bet.",
        "Back to the bottom. Or the Black Market.",
        "Everything. Gone. In one hand.",
        "You knew the risk. It didn't pay off.",
        "That's what happens. Remember that.",
        "Start over. The house will wait."
    };
    return pickPreVictory(lines);
}

std::string getDealerBustLine(bool hasBeatenGame) {
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "Busted. Still happens.",
            "Over. Yours again, " + playerName + ".",
            "The house breaks. Same as always."
        };
        return pickPostVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "Busted. The deck turned on me.",
        "Over. Take it. This round is yours.",
        "Well. That's embarrassing.",
        "The house breaks. Enjoy it while it lasts.",
        "Hm. First time for everything.",
        "Over. Just over.",
        "The dealer makes mistakes too. Occasionally.",
        "Take the chips. Don't gloat.",
        "You got lucky. I'll admit it.",
        "One for you. It happens."
    };
    return pickPreVictory(lines);
}

std::string getWinLine(bool firstWin, bool hugeBet, bool hasBeatenGame) {
    if (firstWin) {
        return "Beginner's luck. Enjoy it while it lasts.";
    }
    
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "Nice one, " + playerName + ".",
            "The house doesn't mind losing to you anymore.",
            "You're still good at this.",
            "Well played, " + playerName + ".",
            "Take it. You've earned the right."
        };
        return pickPostVictory(lines);
    }
    
    if (hugeBet) {
        std::vector<std::string> lines = {
            "...noted.",
            "Hm. That's a big one.",
            "The house remembers this.",
            "Don't get used to it.",
            "Big bet. Big win. Big trouble.",
            "That stings. For me.",
            "Alright, you've got my attention.",
            "Enjoy it. It won't last."
        };
        return pickPreVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "Got lucky.",
        "You beat me clean. Nice hand.",
        "Hmph. Enjoy it.",
        "The deck likes you tonight.",
        "Fine. Take it.",
        "Not bad. Don't get comfortable.",
        "A win. Miracles happen.",
        "Not bad. Not good either. But not bad.",
        "The house doesn't like giving those out.",
        "One down. Hundreds to go.",
        "You're learning. Slowly.",
        "Alright. Not bad."
    };
    return pickPreVictory(lines);
}

std::string getLossLine(bool firstLoss, bool hugeBet, bool hasBeatenGame) {
    if (firstLoss) {
        return "Welcome to the real game.";
    }
    
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "Tough break, " + playerName + ".",
            "The house wins, but you don't owe anyone.",
            "You'll get it back.",
            "A loss is a loss. You're still free, " + playerName + ".",
            "Better luck next round."
        };
        return pickPostVictory(lines);
    }
    
    if (hugeBet) {
        std::vector<std::string> lines = {
            "Ouch. That one hurt.",
            "Half your stack. Gone.",
            "That's a big loss. Even I felt that.",
            "You'll remember this one.",
            "The kind of loss that keeps people up at night."
        };
        return pickPreVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "House takes this round.",
        "Better luck next time.",
        "The deck is cold tonight.",
        "You'll get 'em next hand.",
        "Predictable.",
        "The house wins. As always.",
        "That's how it goes.",
        "Stack's looking thinner.",
        "Another one for the house.",
        "You're not making this easy on yourself.",
        "The cards don't love you back.",
        "Back to work."
    };
    return pickPreVictory(lines);
}

std::string getPushLine(bool hasBeatenGame) {
    if (hasBeatenGame) {
        std::vector<std::string> lines = {
            "Push. Still even.",
            "Tie. Happens.",
            "Nobody wins. Nobody loses. Same as always."
        };
        return pickPostVictory(lines);
    }
    
    std::vector<std::string> lines = {
        "A push. We trade even.",
        "Tie. Nobody wins, nobody loses.",
        "Dead even. How boring.",
        "A wash. Try again.",
        "Same score. The house shrugs.",
        "A tie. Almost as bad as losing.",
        "Even. You'll try again.",
        "Nobody wins. That's rare.",
        "Close, but no payout.",
        "The cards were kind to both of us."
    };
    return pickPreVictory(lines);
}
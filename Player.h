#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include "Card.h"
#include <string>
#include <vector>

class Player {
public:
    std::string id;
    std::string name;
    Hand hand;

    Player() = default;
    Player(const std::string& id, const std::string& name)
        : id(id), name(name) {}

    void receiveCards(const std::vector<Card>& cards) {
        for (const auto& c : cards) {
            hand.cards.push_back(c);
        }
        hand.sortAscending();
    }

    Card playLowest() {
        Card low = hand.lowest();
        hand.remove(low);
        return low;
    }

    Card discardLowestToStar() {
        return playLowest();
    }

    bool hasCards() const {
        return !hand.isEmpty();
    }

    Card peekLowest() const {
        return hand.lowest();
    }
};

#endif

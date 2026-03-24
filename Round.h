#ifndef ROUND_H
#define ROUND_H

#include "Card.h"
#include "Player.h"
#include <vector>

class Round {
public:
    std::vector<Card> playedPile;

    Round() = default;

    bool isComplete(const std::vector<Player>& players) const {
        for (const auto& p : players) {
            if (!p.hand.isEmpty()) return false;
        }
        return true;
    }

    Card top() const {
        if (playedPile.empty()) {
            return Card(0);  // Pila vacia, valor 0 como centinela
        }
        return playedPile.back();
    }

    void addCard(const Card& card) {
        playedPile.push_back(card);
    }

    void clear() {
        playedPile.clear();
    }
};

#endif

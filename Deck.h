#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck() = default;

    void shuffle(int seed) {
        std::mt19937 rng(seed);
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    std::vector<Card> draw(int n) {
        if (n > static_cast<int>(cards.size())) {
            throw std::runtime_error("No hay suficientes cartas en el mazo para repartir.");
        }
        std::vector<Card> drawn;
        for (int i = 0; i < n; i++) {
            drawn.push_back(cards.back());
            cards.pop_back();
        }
        return drawn;
    }

    void resetForLevel(int /*level*/) {
        cards.clear();
        for (int i = 1; i <= 100; i++) {
            cards.push_back(Card(i));
        }
    }

    int remaining() const {
        return static_cast<int>(cards.size());
    }
};

#endif

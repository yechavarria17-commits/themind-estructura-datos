#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

class Hand {
public:
    std::vector<Card> cards;

    Hand() = default;

    int size() const {
        return static_cast<int>(cards.size());
    }

    Card lowest() const {
        if (cards.empty()) {
            throw std::runtime_error("La mano esta vacia, no se puede obtener la carta mas baja.");
        }
        return *std::min_element(cards.begin(), cards.end());
    }

    bool remove(const Card& card) {
        auto it = std::find(cards.begin(), cards.end(), card);
        if (it != cards.end()) {
            cards.erase(it);
            return true;
        }
        return false;
    }

    bool isEmpty() const {
        return cards.empty();
    }

    void sortAscending() {
        std::sort(cards.begin(), cards.end());
    }

    bool hasCardLowerThan(const Card& card) const {
        for (const auto& c : cards) {
            if (c < card) return true;
        }
        return false;
    }

    std::vector<Card> removeAllLowerThan(const Card& card) {
        std::vector<Card> removed;
        auto it = cards.begin();
        while (it != cards.end()) {
            if (*it < card) {
                removed.push_back(*it);
                it = cards.erase(it);
            } else {
                ++it;
            }
        }
        return removed;
    }
};

#endif

#ifndef CARD_H
#define CARD_H

#include <iostream>

class Card {
public:
    int number;

    Card() : number(0) {}
    explicit Card(int n) : number(n) {}

    bool operator<(const Card& other) const { return number < other.number; }
    bool operator>(const Card& other) const { return number > other.number; }
    bool operator==(const Card& other) const { return number == other.number; }
    bool operator!=(const Card& other) const { return number != other.number; }
    bool operator<=(const Card& other) const { return number <= other.number; }
    bool operator>=(const Card& other) const { return number >= other.number; }

    friend std::ostream& operator<<(std::ostream& os, const Card& c) {
        os << c.number;
        return os;
    }
};

#endif

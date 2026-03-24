#ifndef LEVEL_H
#define LEVEL_H

#include <string>

// Recompensas posibles al completar un nivel
enum class RewardType { NONE, LIFE, STAR };

class Level {
public:
    int value;

    Level() : value(1) {}
    explicit Level(int v) : value(v) {}

    int handSize() const {
        return value;
    }

    // Recompensas segun las reglas oficiales del juego
    // Niveles 3, 5, 8: +1 vida
    // Niveles 2, 6, 9: +1 estrella
    RewardType getReward() const {
        if (value == 3 || value == 5 || value == 8) return RewardType::LIFE;
        if (value == 2 || value == 6 || value == 9) return RewardType::STAR;
        return RewardType::NONE;
    }

    std::string rewardDescription() const {
        switch (getReward()) {
            case RewardType::LIFE: return "+1 Vida";
            case RewardType::STAR: return "+1 Estrella";
            default: return "Ninguna";
        }
    }
};

#endif

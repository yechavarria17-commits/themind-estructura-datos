#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

enum class GameState {
    SETUP,
    DEAL,
    IN_ROUND,
    ROUND_COMPLETE,
    LEVEL_COMPLETE,
    GAME_OVER
};

inline std::string gameStateToString(GameState s) {
    switch (s) {
        case GameState::SETUP:          return "SETUP";
        case GameState::DEAL:           return "DEAL";
        case GameState::IN_ROUND:       return "IN_ROUND";
        case GameState::ROUND_COMPLETE: return "ROUND_COMPLETE";
        case GameState::LEVEL_COMPLETE: return "LEVEL_COMPLETE";
        case GameState::GAME_OVER:      return "GAME_OVER";
        default:                        return "UNKNOWN";
    }
}

#endif

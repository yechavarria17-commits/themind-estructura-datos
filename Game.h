#ifndef GAME_H
#define GAME_H

#include "Config.h"
#include "Player.h"
#include "Deck.h"
#include "Level.h"
#include "Lives.h"
#include "Stars.h"
#include "Round.h"
#include "RuleValidator.h"
#include "EventLog.h"
#include "GameState.h"
#include <string>
#include <vector>

class Game {
public:
    std::string id;

    // Componentes del juego (fieles al diagrama UML)
    Config config;
    std::vector<Player> players;      // relacion "Jugador"
    Stars stars;                       // relacion "stars"
    Round currentRound;                // relacion "currentRound"
    Level currentLevel;                // relacion "level"
    Lives lives;
    Deck deck;
    RuleValidator ruleValidator;
    EventLog eventLog;
    GameState state;

    // Semilla para aleatoriedad durante la simulacion
    int currentSeed;

    Game();
    Game(const Config& cfg);

    void start();
    void startLevel(int n);
    void dealHands();
    void playCard(Player& p, const Card& c);
    bool useStar();
    bool endRoundIfComplete();
    bool advanceLevel();

    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);

    // Metodos auxiliares para el flujo del juego
    void runInteractive();
    void runAutoSimulation();

private:
    void printGameState() const;
    void printPlayerHand(const Player& p) const;
    void printAllHands() const;
    int findPlayerWithGlobalLowest() const;
    bool anyPlayerHasCards() const;
    void applyLevelReward();
};

#endif

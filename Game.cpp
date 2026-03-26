#include "Game.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <sstream>
#include <limits>

// ===================== Constructores =====================

Game::Game() : state(GameState::SETUP), currentSeed(42) {}

Game::Game(const Config& cfg)
    : config(cfg),
      stars(cfg.startingStars),
      currentLevel(1),
      lives(cfg.startingLives),
      state(GameState::SETUP),
      currentSeed(cfg.seed)
{
    id = "game_1";
    // Crear los jugadores
    for (int i = 0; i < cfg.maxPlayers; i++) {
        std::string pid = "p" + std::to_string(i + 1);
        std::string pname = "Jugador " + std::to_string(i + 1);
        players.emplace_back(pid, pname);
    }
}

// ===================== Metodos del UML =====================

void Game::start() {
    eventLog.record("=== THE MIND - INICIO DEL JUEGO ===");
    eventLog.record("Jugadores: " + std::to_string(config.maxPlayers));
    eventLog.record("Vidas iniciales: " + std::to_string(lives.count));
    eventLog.record("Estrellas iniciales: " + std::to_string(stars.count));
    eventLog.record("Niveles a completar: " + std::to_string(config.maxLevel));
    state = GameState::DEAL;
    startLevel(1);
}

void Game::startLevel(int n) {
    currentLevel = Level(n);
    currentRound.clear();
    state = GameState::DEAL;

    std::cout << "\n============================================\n";
    std::cout << "         NIVEL " << n << " de " << config.maxLevel << "\n";
    std::cout << "   Cartas por jugador: " << currentLevel.handSize() << "\n";
    std::cout << "   Vidas: " << lives.count << " | Estrellas: " << stars.count << "\n";
    std::cout << "============================================\n";

    eventLog.record("--- Inicio del Nivel " + std::to_string(n) + " ---");
    dealHands();
    state = GameState::IN_ROUND;
}

void Game::dealHands() {
    // Resetear y barajar el mazo
    deck.resetForLevel(currentLevel.value);
    currentSeed++;
    deck.shuffle(currentSeed);

    // Limpiar las manos de todos los jugadores
    for (auto& p : players) {
        p.hand.cards.clear();
    }

    // Repartir cartas segun el nivel actual
    int cardsPerPlayer = currentLevel.handSize();
    for (auto& p : players) {
        std::vector<Card> drawn = deck.draw(cardsPerPlayer);
        p.receiveCards(drawn);

        std::string cardStr;
        p.hand.sortAscending();
        for (const auto& c : p.hand.cards) {
            if (!cardStr.empty()) cardStr += ", ";
            cardStr += std::to_string(c.number);
        }
        eventLog.record(p.name + " recibe: [" + cardStr + "]");
    }

    eventLog.record("Cartas repartidas. Comienza la ronda.");
}

void Game::playCard(Player& p, const Card& c) {
    bool legal = ruleValidator.isLegalPlay(currentRound, c);

    if (legal) {
        // Verificar si algun otro jugador tiene una carta menor (error de sincronizacion)
        bool someoneHadLower = false;
        for (auto& other : players) {
            if (other.id == p.id) continue;
            if (other.hasCards() && other.peekLowest() < c) {
                someoneHadLower = true;
                break;
            }
        }

        if (someoneHadLower) {
            // Jugada ilegal: alguien tenia una carta menor
            currentRound.addCard(c);
            eventLog.record(p.name + " juega la carta " + std::to_string(c.number)
                          + " -- ERROR: otro jugador tenia una carta menor.");
            ruleValidator.resolveIllegalPlay(*this, p, c);
        } else {
            // Jugada completamente legal
            currentRound.addCard(c);
            eventLog.record(p.name + " juega la carta " + std::to_string(c.number) + " -- Correcto.");
        }
    } else {
        // La carta es menor o igual al tope de la pila (no deberia pasar en flujo normal)
        currentRound.addCard(c);
        eventLog.record(p.name + " juega la carta " + std::to_string(c.number)
                      + " -- ERROR: menor al tope de la pila.");
        ruleValidator.resolveIllegalPlay(*this, p, c);
    }
}

bool Game::useStar() {
    if (ruleValidator.canUseStar(*this)) {
        ruleValidator.applyStar(*this);
        return true;
    }
    eventLog.record("No se puede usar una estrella (no hay disponibles o nadie tiene cartas).");
    return false;
}

bool Game::endRoundIfComplete() {
    if (currentRound.isComplete(players)) {
        state = GameState::ROUND_COMPLETE;
        eventLog.record("Ronda completada en el Nivel " + std::to_string(currentLevel.value) + ".");
        return true;
    }
    return false;
}

bool Game::advanceLevel() {
    // Otorgar recompensa del nivel completado
    applyLevelReward();

    if (currentLevel.value >= config.maxLevel) {
        state = GameState::GAME_OVER;
        eventLog.record("=== VICTORIA: El equipo completo todos los niveles. ===");
        return false;  // No hay mas niveles
    }

    state = GameState::LEVEL_COMPLETE;
    startLevel(currentLevel.value + 1);
    return true;
}

// ===================== Metodos auxiliares =====================

void Game::applyLevelReward() {
    RewardType reward = currentLevel.getReward();
    if (reward == RewardType::LIFE) {
        lives.gain();
        eventLog.record("RECOMPENSA del Nivel " + std::to_string(currentLevel.value)
                      + ": +1 Vida (Total: " + std::to_string(lives.count) + ")");
    } else if (reward == RewardType::STAR) {
        stars.gain();
        eventLog.record("RECOMPENSA del Nivel " + std::to_string(currentLevel.value)
                      + ": +1 Estrella (Total: " + std::to_string(stars.count) + ")");
    }
}

void Game::printGameState() const {
    std::cout << "\n--- Estado actual ---\n";
    std::cout << "  Nivel: " << currentLevel.value << "/" << config.maxLevel << "\n";
    std::cout << "  Vidas: " << lives.count << " | Estrellas: " << stars.count << "\n";
    std::cout << "  Tope de la pila: ";
    if (currentRound.playedPile.empty()) {
        std::cout << "(vacia)";
    } else {
        std::cout << currentRound.top().number;
    }
    std::cout << "\n";
    std::cout << "  Cartas jugadas: " << currentRound.playedPile.size() << "\n";
}

void Game::printPlayerHand(const Player& p) const {
    std::cout << "  " << p.name << " tiene " << p.hand.size() << " carta(s): [";
    bool first = true;
    for (const auto& c : p.hand.cards) {
        if (!first) std::cout << ", ";
        std::cout << c.number;
        first = false;
    }
    std::cout << "]\n";
}

void Game::printAllHands() const {
    for (const auto& p : players) {
        printPlayerHand(p);
    }
}

int Game::findPlayerWithGlobalLowest() const {
    int minIdx = -1;
    Card minCard(101);
    for (int i = 0; i < static_cast<int>(players.size()); i++) {
        if (players[i].hasCards() && players[i].peekLowest() < minCard) {
            minCard = players[i].peekLowest();
            minIdx = i;
        }
    }
    return minIdx;
}

bool Game::anyPlayerHasCards() const {
    for (const auto& p : players) {
        if (p.hasCards()) return true;
    }
    return false;
}

// ===================== Modo Interactivo =====================

void Game::runInteractive() {
    start();

    while (state != GameState::GAME_OVER) {
        if (lives.isDepleted()) {
            state = GameState::GAME_OVER;
            std::cout << "\n*** DERROTA: El equipo se quedo sin vidas. ***\n";
            eventLog.record("=== DERROTA: El equipo perdio todas sus vidas. ===");
            break;
        }

        if (endRoundIfComplete()) {
            std::cout << "\n*** Nivel " << currentLevel.value << " completado. ***\n";
            if (!advanceLevel()) {
                std::cout << "\n*********************************************\n";
                std::cout << "  VICTORIA: Completaron todos los niveles.\n";
                std::cout << "*********************************************\n";
                break;
            }
            continue;
        }

        printGameState();
        std::cout << "\n  Manos de los jugadores:\n";
        printAllHands();

        // Autoguardado del progreso actual para evitar pérdida
        saveGame("autosave.txt");

        std::cout << "\n  Opciones:\n";
        for (int i = 0; i < static_cast<int>(players.size()); i++) {
            if (players[i].hasCards()) {
                std::cout << "    " << (i + 1) << ". " << players[i].name
                          << " juega su carta mas baja (" << players[i].peekLowest().number << ")\n";
            }
        }
        if (stars.hasAny()) {
            std::cout << "    0. Usar una estrella (todos descartan su carta mas baja)\n";
        }
        std::cout << "   -1. Guardar partida y salir\n";

        std::cout << "\n  Ingrese su eleccion: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Entrada invalida. Intente de nuevo.\n";
            continue;
        }

        if (choice == -1) {
            std::cout << "  Ingrese nombre de archivo para guardar: ";
            std::string fname;
            std::cin >> fname;
            if (saveGame(fname)) {
                std::cout << "  Partida guardada exitosamente.\n";
            } else {
                std::cout << "  Error guardando partida.\n";
            }
            break;
        } else if (choice == 0) {
            if (stars.hasAny()) {
                useStar();
            } else {
                std::cout << "  No hay estrellas disponibles.\n";
            }
        } else if (choice >= 1 && choice <= static_cast<int>(players.size())) {
            int idx = choice - 1;
            if (players[idx].hasCards()) {
                Card c = players[idx].playLowest();
                playCard(players[idx], c);
            } else {
                std::cout << "  Ese jugador no tiene cartas.\n";
            }
        } else {
            std::cout << "  Opcion invalida.\n";
        }
    }

    std::cout << "\n";
    eventLog.printAll();
}

// ===================== Modo Simulacion Automatica =====================

void Game::runAutoSimulation() {
    start();
    std::mt19937 rng(config.seed);

    while (state != GameState::GAME_OVER) {
        if (lives.isDepleted()) {
            state = GameState::GAME_OVER;
            std::cout << "\n*** DERROTA: El equipo se quedo sin vidas. ***\n";
            eventLog.record("=== DERROTA: El equipo perdio todas sus vidas. ===");
            break;
        }

        if (endRoundIfComplete()) {
            std::cout << "\n*** Nivel " << currentLevel.value << " completado. ***\n";
            if (!advanceLevel()) {
                std::cout << "\n*********************************************\n";
                std::cout << "  VICTORIA: Completaron todos los niveles.\n";
                std::cout << "*********************************************\n";
                break;
            }
            continue;
        }

        if (!anyPlayerHasCards()) break;

        // Estrategia de la IA:
        // Generalmente el jugador con la carta globalmente mas baja juega.
        // Pero con cierta probabilidad (simulando desincronizacion),
        // otro jugador puede jugar primero, causando un error.

        // Decidir si usar estrella: cuando dos jugadores tienen cartas muy cercanas
        if (stars.hasAny()) {
            // Buscar si hay cartas peligrosamente cercanas entre jugadores
            std::vector<int> lowestCards;
            for (const auto& p : players) {
                if (p.hasCards()) {
                    lowestCards.push_back(p.peekLowest().number);
                }
            }
            if (lowestCards.size() >= 2) {
                std::sort(lowestCards.begin(), lowestCards.end());
                int gap = lowestCards[1] - lowestCards[0];
                // Si la diferencia entre las dos cartas mas bajas es <= 3, usar estrella
                if (gap <= 3 && gap > 0) {
                    std::uniform_int_distribution<int> dist(0, 100);
                    if (dist(rng) < 60) {  // 60% de chance de usar estrella en situacion peligrosa
                        eventLog.record("La IA decide usar una estrella (cartas cercanas detectadas).");
                        useStar();
                        continue;
                    }
                }
            }
        }

        int correctIdx = findPlayerWithGlobalLowest();
        if (correctIdx < 0) break;

        // Probabilidad de error: depende del nivel y la cercanía de las cartas
        int errorChance = 5 + currentLevel.value * 2;  // 7% a 21% dependiendo del nivel

        // Verificar si hay cartas cercanas entre jugadores
        std::vector<std::pair<int, int>> playersWithCards;
        for (int i = 0; i < static_cast<int>(players.size()); i++) {
            if (players[i].hasCards()) {
                playersWithCards.emplace_back(players[i].peekLowest().number, i);
            }
        }

        int chosenIdx = correctIdx;
        if (playersWithCards.size() >= 2) {
            std::sort(playersWithCards.begin(), playersWithCards.end());
            int gap = playersWithCards[1].first - playersWithCards[0].first;
            if (gap <= 5) {
                errorChance += (6 - gap) * 8;  // Cartas mas cercanas = mas error
            }

            std::uniform_int_distribution<int> dist(0, 100);
            if (dist(rng) < errorChance) {
                // Elegir aleatoriamente entre los jugadores con cartas (no el correcto)
                std::vector<int> candidates;
                for (const auto& pc : playersWithCards) {
                    if (pc.second != correctIdx) {
                        candidates.push_back(pc.second);
                    }
                }
                if (!candidates.empty()) {
                    std::uniform_int_distribution<int> cdist(0, static_cast<int>(candidates.size()) - 1);
                    chosenIdx = candidates[cdist(rng)];
                }
            }
        }

        // El jugador elegido juega su carta mas baja
        Player& chosen = players[chosenIdx];
        Card c = chosen.playLowest();

        std::cout << "\n  >> " << chosen.name << " juega la carta " << c.number << "\n";
        playCard(chosen, c);

        // Verificar si el equipo murio
        if (lives.isDepleted()) continue;
    }

    std::cout << "\n";
    eventLog.printAll();
}

// ===================== Persistencia de Datos =====================

bool Game::saveGame(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    out << id << "\n";
    out << currentSeed << " " << currentLevel.value << " " << lives.count << " " << stars.count << " " << static_cast<int>(state) << "\n";
    out << config.maxPlayers << " " << config.startingLives << " " << config.startingStars << " " << config.maxLevel << " " << config.seed << "\n";
    
    out << currentRound.playedPile.size() << "\n";
    for (const auto& c : currentRound.playedPile) {
        out << c.number << " ";
    }
    out << "\n";

    out << players.size() << "\n";
    for (const auto& p : players) {
        out << p.id << " " << p.name << "\n";
        out << p.hand.cards.size() << "\n";
        for (const auto& c : p.hand.cards) {
            out << c.number << " ";
        }
        out << "\n";
    }

    out.close();
    return true;
}

bool Game::loadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::getline(in, id); // Empty if first line was just \n but let's assume valid ID
    if (id.empty() || id == "\r") {
        std::getline(in, id);
    }
    
    int st;
    in >> currentSeed >> currentLevel.value >> lives.count >> stars.count >> st;
    state = static_cast<GameState>(st);

    in >> config.maxPlayers >> config.startingLives >> config.startingStars >> config.maxLevel >> config.seed;

    size_t pileSize;
    in >> pileSize;
    currentRound.playedPile.clear();
    for (size_t i = 0; i < pileSize; ++i) {
        int n;
        in >> n;
        currentRound.playedPile.push_back(Card(n));
    }

    size_t numPlayers;
    in >> numPlayers;
    players.clear();
    for (size_t i = 0; i < numPlayers; ++i) {
        std::string pid, pname;
        in >> pid;
        std::getline(in, pname); // consumimos resto y armamos string
        if (!pname.empty() && pname[0] == ' ') pname = pname.substr(1);
        Player p(pid, pname);

        size_t handSize;
        in >> handSize;
        for (size_t j = 0; j < handSize; ++j) {
            int n;
            in >> n;
            p.hand.cards.push_back(Card(n));
        }
        players.push_back(p);
    }

    in.close();
    eventLog.record("Partida cargada desde " + filename);
    return true;
}

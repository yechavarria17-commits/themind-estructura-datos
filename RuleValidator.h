#ifndef RULEVALIDATOR_H
#define RULEVALIDATOR_H

#include "Round.h"
#include "Card.h"
#include "Player.h"
#include <vector>

// Declaracion adelantada para evitar dependencia circular
class Game;

class RuleValidator {
public:
    // Verifica si una carta puede ser jugada legalmente
    // (debe ser mayor que la carta en el tope de la pila)
    bool isLegalPlay(const Round& round, const Card& card) const;

    // Resuelve una jugada ilegal: pierde vida y descarta cartas menores
    void resolveIllegalPlay(Game& game, Player& player, const Card& card) const;

    // Verifica si el equipo puede usar una estrella
    bool canUseStar(const Game& game) const;

    // Aplica el efecto de una estrella: cada jugador descarta su carta mas baja
    std::vector<Card> applyStar(Game& game) const;
};

#endif

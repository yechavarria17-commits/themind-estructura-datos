#include "RuleValidator.h"
#include "Game.h"

bool RuleValidator::isLegalPlay(const Round& round, const Card& card) const {
    // Una carta es legal si es mayor que el tope de la pila
    // Si la pila esta vacia, cualquier carta es legal
    return card > round.top();
}

void RuleValidator::resolveIllegalPlay(Game& game, Player& /*player*/, const Card& card) const {
    // 1. El equipo pierde una vida
    game.lives.lose();
    game.eventLog.record("PENALIZACION: El equipo pierde una vida. Vidas restantes: "
                         + std::to_string(game.lives.count));

    // 2. Todos los jugadores descartan cartas menores a la carta jugada
    for (auto& p : game.players) {
        std::vector<Card> discarded = p.hand.removeAllLowerThan(card);
        for (const auto& d : discarded) {
            game.eventLog.record("  " + p.name + " descarta la carta " + std::to_string(d.number));
        }
    }
}

bool RuleValidator::canUseStar(const Game& game) const {
    // Se puede usar una estrella si hay estrellas disponibles
    // y al menos un jugador tiene cartas
    if (!game.stars.hasAny()) return false;
    for (const auto& p : game.players) {
        if (p.hasCards()) return true;
    }
    return false;
}

std::vector<Card> RuleValidator::applyStar(Game& game) const {
    std::vector<Card> discarded;

    if (!game.stars.use()) return discarded;

    game.eventLog.record("ESTRELLA USADA: Cada jugador descarta su carta mas baja.");

    for (auto& p : game.players) {
        if (p.hasCards()) {
            Card lowest = p.discardLowestToStar();
            discarded.push_back(lowest);
            game.eventLog.record("  " + p.name + " descarta la carta " + std::to_string(lowest.number));
        }
    }

    game.eventLog.record("Estrellas restantes: " + std::to_string(game.stars.count));
    return discarded;
}

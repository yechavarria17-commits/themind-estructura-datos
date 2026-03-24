#ifndef CONFIG_H
#define CONFIG_H

#include <stdexcept>

class Config {
public:
    int maxPlayers = 4;
    int startingLives;
    int startingStars;
    int maxLevel;
    int seed;

    Config() : startingLives(2), startingStars(1), maxLevel(12), seed(42) {}

    Config(int numPlayers, int seed) : seed(seed) {
        maxPlayers = numPlayers;
        startingStars = 1;  // Siempre 1 estrella inicial

        // Configuracion segun numero de jugadores (reglas oficiales)
        switch (numPlayers) {
            case 2:
                startingLives = 2;
                maxLevel = 12;
                break;
            case 3:
                startingLives = 3;
                maxLevel = 10;
                break;
            case 4:
                startingLives = 4;
                maxLevel = 8;
                break;
            default:
                throw std::invalid_argument(
                    "Numero de jugadores invalido. Debe ser 2, 3 o 4.");
        }
    }
};

#endif

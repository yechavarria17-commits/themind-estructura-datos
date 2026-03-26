/*
 * Proyecto: THE MIND - Juego de Mesa Cooperativo
 * Estructura de Datos - Trabajo Universitario
 * Colaborador: [Yeison Lopez y Sebastian Galiano]
 * Fecha: 23/03/2026
 * Descripción: Punto de entrada principal para la lógica del juego.
 */
#include "Game.h"
#include "Config.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <string>

void printBanner() {
    std::cout << R"(
 ============================================================
 |                                                          |
 |                    T H E   M I N D                       |
 |                                                          |
 |          Juego de cartas colaborativo (C++)               |
 |                                                          |
 |   Reglas: Jueguen todas las cartas en orden ascendente   |
 |   sin comunicarse. Usen el sentido del tiempo.           |
 |                                                          |
 ============================================================
)" << std::endl;
}

void printRules() {
    std::cout << R"(
 ===================== REGLAS DEL JUEGO =====================

 THE MIND es un juego cooperativo donde todos los jugadores
 forman un solo equipo.

 OBJETIVO:
   Jugar todas las cartas numeradas (1-100) en orden ascendente
   sobre una pila central, sin comunicarse entre jugadores.

 SETUP POR NUMERO DE JUGADORES:
   2 jugadores: 2 vidas, 1 estrella, niveles 1-12
   3 jugadores: 3 vidas, 1 estrella, niveles 1-10
   4 jugadores: 4 vidas, 1 estrella, niveles 1-8

 NIVELES:
   En el nivel N, cada jugador recibe N cartas.
   Nivel 1 = 1 carta cada uno, Nivel 2 = 2 cartas, etc.

 COMO SE JUEGA:
   - No hay turnos fijos. Quien crea tener la carta mas baja
     la juega sobre la pila central.
   - Cada jugador DEBE jugar siempre su carta mas baja primero.
   - No se permite comunicacion de ningun tipo.

 ERRORES:
   Si alguien juega una carta y otro jugador tenia una carta
   menor en su mano, el equipo pierde 1 vida. Todos los
   jugadores descartan las cartas menores a la jugada.

 ESTRELLAS (Throwing Stars):
   Cualquier jugador puede proponer usar una estrella. Si todos
   aceptan, cada jugador descarta su carta mas baja boca arriba.

 RECOMPENSAS POR NIVEL COMPLETADO:
   Nivel 2: +1 Estrella
   Nivel 3: +1 Vida
   Nivel 5: +1 Vida
   Nivel 6: +1 Estrella
   Nivel 8: +1 Vida
   Nivel 9: +1 Estrella
   (Maximo: 5 vidas, 3 estrellas)

 VICTORIA: Completar todos los niveles con al menos 1 vida.
 DERROTA:  Perder todas las vidas.

 =============================================================
)" << std::endl;
}

int getValidInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Entrada invalida. Ingrese un numero entre "
                  << min << " y " << max << ".\n";
    }
}

int main() {
    printBanner();

    int mainChoice;
    do {
        std::cout << "\n  MENU PRINCIPAL:\n";
        std::cout << "    1. Ver reglas del juego\n";
        std::cout << "    2. Jugar en modo interactivo (tu decides quien juega)\n";
        std::cout << "    3. Ver simulacion automatica (la IA juega sola)\n";
        std::cout << "    4. Cargar partida (puedes escribir 'autosave.txt')\n";
        std::cout << "    5. Salir\n";

        mainChoice = getValidInt("\n  Seleccione una opcion: ", 1, 5);

        if (mainChoice == 1) {
            printRules();
            continue;
        }

        if (mainChoice == 5) {
            std::cout << "\n  Gracias por jugar The Mind.\n\n";
            break;
        }

        if (mainChoice == 4) {
            std::cout << "  Ingrese nombre de archivo a cargar: ";
            std::string fname;
            std::cin >> fname;
            
            Config dummyCfg(2, 0); // temp config
            Game game(dummyCfg);
            if (game.loadGame(fname)) {
                std::cout << "  Partida cargada correctamente.\n";
                game.runInteractive();
            } else {
                std::cout << "  Error al cargar la partida. El archivo no existe o es invalido.\n";
            }
            continue;
        }

        if (mainChoice == 2 || mainChoice == 3) {
            int numPlayers = getValidInt(
                "\n  Numero de jugadores (2, 3 o 4): ", 2, 4);

            std::cout << "  Ingrese una semilla para la aleatoriedad (0 = aleatoria): ";
            int seed;
            if (!(std::cin >> seed)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                seed = 0;
            }
            if (seed == 0) {
                seed = static_cast<int>(std::time(nullptr));
            }

            Config cfg(numPlayers, seed);
            Game game(cfg);

            if (mainChoice == 2) {
                game.runInteractive();
            } else {
                game.runAutoSimulation();
            }

            std::cout << "\n  Presione Enter para volver al menu...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }

    } while (mainChoice != 5);

    return 0;
}

# The Mind - Juego de Cartas Colaborativo en C++

## Descripcion

Implementacion en C++ del juego de cartas colaborativo **The Mind**, disenado por Wolfgang Warsch.
Todos los jugadores forman un equipo y deben jugar cartas numeradas del 1 al 100 en orden
ascendente sobre una pila central, sin comunicarse entre ellos.

## Como compilar

```bash
make
```

Esto genera el ejecutable en `build/TheMind`.

## Como ejecutar

```bash
make run
```

O directamente:

```bash
./build/TheMind
```

## Modos de juego

1. **Modo interactivo**: Tu decides que jugador juega en cada turno. Puedes ver las manos
   de todos los jugadores y elegir quien coloca su carta. Tambien puedes decidir cuando usar
   una estrella.

2. **Simulacion automatica**: Una IA juega el juego completo. La IA generalmente elige al
   jugador con la carta mas baja, pero con cierta probabilidad comete errores de
   sincronizacion (como pasaria en el juego real), especialmente cuando las cartas de
   distintos jugadores estan muy cerca unas de otras.

## Reglas del juego

- **2 jugadores**: 2 vidas, 1 estrella, niveles 1-12
- **3 jugadores**: 3 vidas, 1 estrella, niveles 1-10
- **4 jugadores**: 4 vidas, 1 estrella, niveles 1-8
- En el nivel N, cada jugador recibe N cartas
- Se deben jugar las cartas en orden ascendente global
- Si alguien juega una carta y otro jugador tenia una carta menor, el equipo pierde 1 vida
  y todos descartan sus cartas menores a la jugada
- Las estrellas permiten que todos descarten su carta mas baja (revelada)
- Recompensas por completar niveles: nivel 2 (+1 estrella), nivel 3 (+1 vida),
  nivel 5 (+1 vida), nivel 6 (+1 estrella), nivel 8 (+1 vida), nivel 9 (+1 estrella)
- Maximo: 5 vidas y 3 estrellas
- Victoria: completar todos los niveles. Derrota: perder todas las vidas.

## Estructura del proyecto y diagrama UML

```
TheMind/
├── Makefile
├── README.md
├── src/
│   ├── Card.h            -> Clase Card (numero de la carta)
│   ├── Hand.h            -> Clase Hand (coleccion de cartas de un jugador)
│   ├── Deck.h            -> Clase Deck (mazo de 100 cartas)
│   ├── Player.h          -> Clase Player (id, nombre, mano)
│   ├── Level.h           -> Clase Level (valor del nivel, tamano de mano, recompensas)
│   ├── Lives.h           -> Clase Lives (vidas del equipo)
│   ├── Stars.h           -> Clase Stars (estrellas del equipo)
│   ├── Round.h           -> Clase Round (pila de cartas jugadas)
│   ├── Config.h          -> Clase Config (configuracion del juego)
│   ├── RuleValidator.h   -> Clase RuleValidator (validacion de reglas)
│   ├── RuleValidator.cpp -> Implementacion de RuleValidator
│   ├── EventLog.h        -> Clase EventLog (registro de eventos)
│   ├── GameState.h       -> Enum GameState (estados del juego)
│   ├── Game.h            -> Clase Game (controlador principal)
│   ├── Game.cpp          -> Implementacion de Game
│   └── main.cpp          -> Punto de entrada con menu
└── build/
    └── TheMind           -> Ejecutable compilado
```

## Cambios realizados al diagrama UML original

El diagrama UML original fue respetado en su estructura general. Los cambios menores
realizados fueron:

1. **Hand::hasCardLowerThan() y Hand::removeAllLowerThan()**: Se agregaron dos metodos
   auxiliares a la clase Hand que no estaban en el diagrama, pero son necesarios para
   implementar la regla de descarte al cometer un error (cuando alguien juega una carta
   y otro jugador tenia una menor).

2. **Level::getReward() y Level::rewardDescription()**: Se agregaron metodos a Level para
   manejar las recompensas por nivel completado, que son parte esencial de las reglas
   oficiales del juego.

3. **Lives::gain() y Stars::gain()**: Se agregaron metodos para incrementar vidas y
   estrellas al recibir recompensas de nivel, respetando los maximos (5 vidas, 3 estrellas).

4. **Game::runInteractive() y Game::runAutoSimulation()**: Se agregaron los dos modos de
   ejecucion del juego como metodos publicos de Game, junto con metodos auxiliares privados
   para imprimir estado, buscar la carta mas baja global, etc.

5. **Player::peekLowest() y Player::hasCards()**: Metodos auxiliares para consultar el
   estado de la mano sin modificarla.

6. **Round::addCard() y Round::clear()**: Metodos para gestionar la pila de cartas jugadas
   durante una ronda.

Todos estos cambios son extensiones naturales del diagrama que no contradicen la estructura
original, sino que la complementan para que el juego sea funcional.

## Requisitos

- Compilador C++ con soporte para C++17 (g++, clang++)
- make

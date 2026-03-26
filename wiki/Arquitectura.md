# Arquitectura del Sistema

El proyecto sigue una estructura orientada a objetos basada en el diagrama UML.

## Clases Principales
- **Game**: Controlador principal que gestiona la máquina de estados.
- **Player**: Representa a cada jugador y su `Hand` de cartas.
- **Round**: Pila de cartas jugadas en la ronda actual.
- **Deck**, **Level**, **Lives**, **Stars**: Componentes individuales de las mecánicas.

El patrón arquitectónico centraliza toda la lógica de flujo y serialización en `Game`.

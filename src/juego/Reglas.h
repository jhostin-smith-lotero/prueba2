#ifndef REGLAS_H
#define REGLAS_H

#include "Banco.h"
#include "Estado.h"
#include "../modelo/Jugador.h"

/**
 * @class Reglas
 * @brief Procesa el efecto de caer en una casilla del tablero.
 */
class Reglas {
public:
    /**
     * @brief Evalúa la casilla actual del jugador.
     * @pre jugador.posicion debe estar en el rango [0,39].
     * @pre banco y estado deben estar correctamente inicializados.
     * @post El jugador puede recibir dinero, perder dinero, ir a la cárcel o no ocurrir nada.
     */
    void evaluarCasilla(modelo::Jugador& jugador, Banco& banco, Estado& estado);
};

#endif

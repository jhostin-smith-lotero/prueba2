#ifndef JUEGO_H
#define JUEGO_H

#include "Banco.h"
#include "Dado.h"
#include "Estado.h"
#include "Reglas.h"
#include "Turnos.h"

/**
 * @class Juego
 * @brief Controla la lógica principal del juego Monopoly.
 */
class Juego {
private:
    Banco banco;
    Dado dado1, dado2;
    Estado estado;
    Reglas reglas;
    Turnos turnos;

public:
    /**
     * @brief Constructor del juego.
     * @pre Ninguna.
     * @post Juego inicializado con todas las estructuras listas.
     */
    Juego();

    /**
     * @brief Inicializa los jugadores ingresados por el usuario.
     * @pre El usuario debe ingresar entre 2 y 4 nombres válidos.
     * @post Se crean cuentas y jugadores dentro del estado.
     */
    void inicializarJugadores();

    /**
     * @brief Ejecuta un turno del juego.
     * @pre Deben existir jugadores inicializados.
     * @post El jugador actual lanza dados, se mueve y se evalúa la casilla.
     */
    void jugarTurno();

    /**
     * @brief Muestra el estado general del juego.
     * @pre Jugadores inicializados.
     * @post No modifica estado.
     */
    void mostrarEstado();

    /**
     * @brief Determina si el juego ya terminó.
     * @pre Jugadores inicializados.
     * @post No modifica estado.
     * @return true si solo queda un jugador con saldo > 0.
     */
    bool haTerminado();
};

#endif

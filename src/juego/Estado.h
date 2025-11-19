#ifndef ESTADO_H
#define ESTADO_H

#include <string>
#include <vector>
#include "../modelo/Jugador.h"

/**
 * @class Estado
 * @brief Representa el estado dinámico del juego (jugadores y turno actual).
 */
class Estado {
private:
    std::vector<modelo::Jugador> jugadores;
    int turnoActual;

public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post turnoActual = 0, jugadores vacío.
     */
    Estado();

    /**
     * @brief Agrega un jugador nuevo.
     * @pre nombre no vacío.
     * @post Se añade un jugador con dinero en 0 y sin propiedades.
     */
    void agregarJugador(const std::string& nombre);

    /**
     * @brief Obtiene el jugador del turno actual.
     * @pre Debe existir al menos un jugador.
     * @post No modifica estado.
     */
    modelo::Jugador& getJugadorActual();

    const modelo::Jugador& getJugadorActualConst() const;

    /**
     * @brief Avanza al siguiente turno.
     * @pre Debe existir al menos un jugador.
     * @post turnoActual = (turnoActual + 1) % jugadores.size().
     */
    void siguienteTurno();

    /**
     * @brief Obtiene el índice del turno actual.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    int getTurnoActual() const;

    /**
     * @brief Devuelve copia de vector de jugadores.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    std::vector<modelo::Jugador> getJugadores() const;

    const std::vector<modelo::Jugador>& jugadoresRef() const { return jugadores; }
    std::vector<modelo::Jugador>& jugadoresRefMutable() { return jugadores; }
};

#endif

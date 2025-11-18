#ifndef TURNOS_H
#define TURNOS_H

/**
 * @class Turnos
 * @brief Lleva el conteo del número total de turnos ejecutados.
 */
class Turnos {
private:
    int numeroTurno;

public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post numeroTurno inicia en 1.
     */
    Turnos();

    /**
     * @brief Avanza un turno.
     * @pre Ninguna.
     * @post numeroTurno incrementa en 1.
     */
    void siguiente();

    /**
     * @brief Consulta el número actual de turno.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    int getTurno() const;
};

#endif

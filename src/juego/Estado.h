#ifndef ESTADO_H
#define ESTADO_H

#include <string>
#include <vector>
#include "../modelo/Jugador.h"

/**
 * @class Estado
 * @brief Contiene información dinámica del juego.
 */
class Estado {
private:
    std::vector<modelo::Jugador> jugadores;
    int turnoActual;

public:
    Estado();

    void agregarJugador(const std::string& nombre);
    modelo::Jugador& getJugadorActual();
    const modelo::Jugador& getJugadorActualConst() const;
    void siguienteTurno();
    int getTurnoActual() const;
    std::vector<modelo::Jugador> getJugadores() const;

    const std::vector<modelo::Jugador>& jugadoresRef() const { return jugadores; }
    std::vector<modelo::Jugador>& jugadoresRefMutable() { return jugadores; }

};

#endif

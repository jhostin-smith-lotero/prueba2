#include "Estado.h"

Estado::Estado() : turnoActual(0) {}

void Estado::agregarJugador(const std::string& nombre) {
    modelo::Jugador jugador(nombre);
    jugador.moverDirecto(0, 40);
    jugador.liberarDeCarcel();
    jugador.ajustarDinero(1500);
    jugadores.push_back(jugador);
}

modelo::Jugador& Estado::getJugadorActual() {
    return jugadores[turnoActual];
}

const modelo::Jugador& Estado::getJugadorActualConst() const {
    return jugadores[turnoActual];
}

void Estado::siguienteTurno() {
    turnoActual = (turnoActual + 1) % jugadores.size();
}

int Estado::getTurnoActual() const {
    return turnoActual;
}

std::vector<modelo::Jugador> Estado::getJugadores() const {
    return jugadores;
}


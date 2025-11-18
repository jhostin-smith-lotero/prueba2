#include "Estado.h"

Estado::Estado() : turnoActual(0) {}

void Estado::agregarJugador(const std::string& nombre) {
    jugadores.push_back({nombre, 0, false});
}

Jugador& Estado::getJugadorActual() {
    return jugadores[turnoActual];
}

void Estado::siguienteTurno() {
    turnoActual = (turnoActual + 1) % jugadores.size();
}

int Estado::getTurnoActual() const {
    return turnoActual;
}

std::vector<Jugador> Estado::getJugadores() const {
    return jugadores;
}

void Estado::asignarPropiedad(const std::string& propiedad, const std::string& jugador) {
    propiedades[propiedad] = jugador;
}

std::string Estado::getPropietario(const std::string& propiedad) {
    if (propiedades.find(propiedad) != propiedades.end())
        return propiedades[propiedad];
    return "";
}

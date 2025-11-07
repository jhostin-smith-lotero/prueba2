#include "Construccion.h"

#include "../juego/Banco.h"
#include "../modelo/Jugador.h"
#include "../modelo/Solar.h"

namespace reglas {

bool Construccion::construirCasa(modelo::Solar& solar,
                                 modelo::Jugador& jugador,
                                 juego::Banco& banco,
                                 std::string& mensaje) {
    if (solar.tieneHotel()) {
        mensaje = "La propiedad ya tiene un hotel";
        return false;
    }
    if (solar.casas() >= 4) {
        mensaje = "Debe construir un hotel en lugar de más casas";
        return false;
    }
    int costo = solar.escritura().costoCasa();
    if (!jugador.pagar(costo)) {
        mensaje = "Dinero insuficiente para construir";
        return false;
    }
    if (!banco.tomarCasas(1)) {
        jugador.cobrar(costo);
        mensaje = "El banco no tiene casas disponibles";
        return false;
    }
    banco.ajustarEfectivo(costo);
    solar.construirCasa();
    mensaje.clear();
    return true;
}

bool Construccion::construirHotel(modelo::Solar& solar,
                                  modelo::Jugador& jugador,
                                  juego::Banco& banco,
                                  std::string& mensaje) {
    if (solar.tieneHotel()) {
        mensaje = "La propiedad ya tiene un hotel";
        return false;
    }
    if (solar.casas() < 4) {
        mensaje = "Debe tener 4 casas antes de construir un hotel";
        return false;
    }
    int costo = solar.escritura().costoHotel();
    if (!jugador.pagar(costo)) {
        mensaje = "Dinero insuficiente para construir";
        return false;
    }
    if (!banco.tomarHoteles(1)) {
        jugador.cobrar(costo);
        mensaje = "El banco no tiene hoteles disponibles";
        return false;
    }
    banco.devolverCasas(4);
    banco.ajustarEfectivo(costo);
    solar.construirHotel();
    mensaje.clear();
    return true;
}

} // namespace reglas

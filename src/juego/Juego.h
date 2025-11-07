#pragma once

#include "Banco.h"
#include "Dado.h"
#include "Estado.h"
#include "Turnos.h"
#include "../edd/Pila.h"
#include "../edd/TablaHash.h"
#include "../modelo/Mazo.h"
#include "../modelo/Tablero.h"
#include "../reglas/Reglas.h"

#include <string>
#include <vector>

namespace modelo {
class Jugador;
class Propiedad;
class Solar;
class Servicio;
class Ferrocarril;
class Carta;
}

namespace juego {

class Juego {
public:
    Juego();

    void cargarDatos(const std::string& directorioDatos);
    bool iniciarPartida(const std::vector<std::string>& nombresJugadores);

    bool partidaActiva() const { return partidaActiva_; }
    const modelo::Jugador& jugadorActual() const;
    modelo::Jugador& jugadorActual();

    Turnos::Fase faseActual() const;
    bool puedeDeshacer() const;

    std::string lanzarDados();
    std::string comprarPropiedadActual();
    std::string pasarTurno();
    std::string construirCasa(const std::string& nombrePropiedad);
    std::string construirHotel(const std::string& nombrePropiedad);
    std::string hipotecarPropiedad(const std::string& nombrePropiedad);
    std::string deshipotecarPropiedad(const std::string& nombrePropiedad);
    std::string deshacer();
    std::string pagarMultaCarcel();
    std::string usarCartaSalirCarcel();

    std::string resumenEstado() const;

    void cobrarSalida(modelo::Jugador& jugador);
    void robarCartaArca(modelo::Jugador& jugador);
    void robarCartaCasualidad(modelo::Jugador& jugador);
    void cobrarImpuesto(modelo::Jugador& jugador, int monto);
    void visitarCarcel(modelo::Jugador& jugador);
    void enviarJugadorCarcel(modelo::Jugador& jugador);
    void estacionamientoLibre(modelo::Jugador& jugador);

    const reglas::Reglas& reglas() const { return reglas_; }
    const modelo::Tablero& tablero() const { return tablero_; }
    const juego::Banco& banco() const { return banco_; }

private:
    friend class modelo::Propiedad;

    void cargarTablero(const std::string& ruta);
    void cargarCartas(const std::string& ruta, modelo::Mazo& mazo);
    void guardarEstado();
    void restaurarEstado(const juego::Estado& estado);
    modelo::Propiedad* propiedadEnPosicion(int posicion);
    modelo::Propiedad* buscarPropiedad(const std::string& nombre);
    int contarPropiedadesColor(const modelo::Jugador& jugador, const modelo::Solar& solar);
    int contarServicios(const modelo::Jugador& jugador) const;
    int contarFerrocarriles(const modelo::Jugador& jugador) const;
    void procesarCasilla(modelo::Jugador& jugador, int tirada);
    void moverJugador(modelo::Jugador& jugador, int pasos, int tirada, bool fueDoble);
    void aplicarCarta(modelo::Carta carta, modelo::Jugador& jugador);
    void asignarPropiedad(modelo::Propiedad& propiedad, modelo::Jugador& jugador);
    void resolverPropiedad(modelo::Propiedad& propiedad, modelo::Jugador& jugador, int tirada);

    reglas::Reglas reglas_;
    modelo::Tablero tablero_;
    Banco banco_;
    modelo::Mazo mazoCasualidad_;
    modelo::Mazo mazoArca_;
    std::vector<modelo::Jugador> jugadores_;
    Turnos turnos_;
    edd::Pila<Estado> historial_;
    edd::TablaHash indice_;
    Dado dado_;
    bool partidaActiva_ = false;
    std::string directorioDatos_;
    int posicionCarcel_ = 10;
};

} // namespace juego

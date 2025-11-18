#ifndef JUEGO_H
#define JUEGO_H

#include "Banco.h"
#include "Dado.h"
#include "Estado.h"
#include "Reglas.h"
#include "Turnos.h"
#include "../modelo/Tablero.h"
#include "../edd/Pila.h"
#include "../edd/TablaHash.h"
#include "../reglas/Construccion.h"
#include "../reglas/Hipoteca.h"

class Juego {
private:
    Banco banco;
    Dado dado1, dado2;
    Estado estado;
    Reglas reglamento;
    Turnos turnos;
    modelo::Tablero tablero;
    edd::TablaHash indice;
    bool partidaIniciada;
    bool opcionCompra;
    int ultimoDado1;
    int ultimoDado2;

public:
    enum FaseTurno { DebeTirar, PostTirada };

private:
    FaseTurno fase;

    struct Snapshot {
        Banco banco;
        Estado estado;
        Turnos turnos;
        FaseTurno fase;
        bool opcionCompra;
        int ultimoDado1;
        int ultimoDado2;
        std::vector<modelo::Tablero::EstadoPropiedad> propiedades;
    };

    edd::Pila<Snapshot> historial;

public:
    Juego();

    void inicializarJugadores();

    void lanzarDados();
    void comprarPropiedadActual();
    void intentarConstruir();
    void intentarHipotecar();
    void intentarDeshipotecar();
    void usarCartaCarcel();
    void pagarMultaCarcel();
    void pasar();
    void undo();
    void mostrarEstado() const;

    bool haTerminado() const;

    bool puedeComprar() const;
    bool puedeConstruir() const;
    bool puedeHipotecar() const;
    bool puedeDeshipotecar() const;
    bool puedePagarMulta() const;
    bool tieneCartaSalir() const;
    bool hayUndo() const;
    bool puedeTirar() const;
    bool puedePasar() const;

    FaseTurno faseActual() const { return fase; }
    modelo::Jugador& jugadorActual();

    const Reglas& reglas() const { return reglamento; }
    Reglas& reglas() { return reglamento; }
    Banco& bancoJugador() { return banco; }
    modelo::Tablero& tableroJuego() { return tablero; }
    int posicionCarcel() const { return tablero.posicionCarcel(); }

private:
    void avanzarJugador(modelo::Jugador& jugador, int pasos);
    void prepararNuevoTurno();
    void evaluarPropiedad(modelo::Propiedad* propiedad, modelo::Jugador& jugador, int tirada);
    void guardarEstado();
    void restaurarEstado();
    bool grupoCompleto(const std::string& color, const modelo::Jugador& jugador) const;
    int valorEdificacion(const modelo::Solar* solar) const;
    bool cumpleSimetria(const modelo::Solar* solar, const modelo::Jugador& jugador) const;
    std::vector<modelo::Solar*> solaresConstruibles(const modelo::Jugador& jugador) const;
};

#endif

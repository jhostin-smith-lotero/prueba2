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
    modelo::Tablero tablero;
    edd::TablaHash indice;
    bool partidaIniciada;
    bool dadosLanzados;
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
        bool dadosLanzados;
        bool opcionCompra;
        int ultimoDado1;
        int ultimoDado2;
    };

    edd::Pila<Snapshot> historial;

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
    void lanzarDados();
    void comprarPropiedadActual();
    void intentarConstruir();
    void intentarHipotecar();
    void intentarDeshipotecar();
    void usarCartaCarcel();
    void pagarMultaCarcel();
    void pasar();
    void undo();

    /**
     * @brief Muestra el estado general del juego.
     * @pre Jugadores inicializados.
     * @post No modifica estado.
     */
    void mostrarEstado() const;

    /**
     * @brief Determina si el juego ya terminó.
     * @pre Jugadores inicializados.
     * @post No modifica estado.
     * @return true si solo queda un jugador con saldo > 0.
     */
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

private:
    void avanzarJugador(modelo::Jugador& jugador, int pasos);
    void prepararNuevoTurno();
    void evaluarPropiedad(modelo::Propiedad* propiedad, modelo::Jugador& jugador, int tirada);
    void guardarEstado();
    void restaurarEstado();
};

#endif

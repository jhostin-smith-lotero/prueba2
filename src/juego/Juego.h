#ifndef JUEGO_H
#define JUEGO_H

#include "Banco.h"
#include "Dado.h"
#include "Estado.h"
#include "Reglas.h"
#include "Turnos.h"
#include "../modelo/Tablero.h"
#include "../modelo/Mazo.h"
#include "../edd/Pila.h"
#include "../edd/TablaHash.h"
#include "../reglas/Construccion.h"
#include "../reglas/Hipoteca.h"
#include "../reglas/CalculoRenta.h"

class Juego {
private:
    Banco banco;
    Dado dado1, dado2;
    Estado estado;
    Reglas reglamento;
    CalculoRenta calculoRenta;
    Turnos turnos;
    modelo::Tablero tablero;
    modelo::Mazo mazoCasualidad;
    modelo::Mazo mazoArca;
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
        modelo::Mazo mazoCasualidad;
        modelo::Mazo mazoArca;
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
    void resolverEspecial(modelo::Especial* especial, modelo::Jugador& jugador, int tirada);

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
    void moverDirecto(modelo::Jugador& jugador, int destino, bool cobraGo);
    void aplicarCarta(const modelo::Carta& carta, modelo::Jugador& jugador);
    void cargarMazo(const std::string& ruta, modelo::Mazo& mazo, bool esArca);
    void prepararNuevoTurno();
    void evaluarPropiedad(modelo::Propiedad* propiedad, modelo::Jugador& jugador, int tirada);
    modelo::Propiedad* buscarPropiedad(const std::string& nombre) const;
    void guardarEstado();
    void restaurarEstado();
    bool grupoCompleto(const std::string& color, const modelo::Jugador& jugador) const;
    int valorEdificacion(const modelo::Solar* solar) const;
    bool cumpleSimetria(const modelo::Solar* solar, const modelo::Jugador& jugador) const;
    std::vector<modelo::Solar*> solaresConstruibles(const modelo::Jugador& jugador) const;
};

#endif

#include "Consola.h"
#include <iostream>
#include "../juego/Juego.h"

Consola::Consola() : activo(false), juegoActual(nullptr) {}

void Consola::inicializar(Juego* juego) {
    if (!juego) return;
    juegoActual = juego;
    activo = true;
}

void Consola::construirOpciones(Juego& juego) {
    comandos.limpiar();
    int numero = 1;

    if (juego.faseActual() == Juego::DebeTirar) {
        comandos.agregar({numero++, Comandos::Tirar, "Tirar dados"});
        if (juego.puedePagarMulta()) {
            comandos.agregar({numero++, Comandos::PagarMulta, "Pagar multa y salir"});
        }
        if (juego.tieneCartaSalir()) {
            comandos.agregar({numero++, Comandos::UsarCarta, "Usar carta 'Salir de la carcel'"});
        }
    } else {
        if (juego.puedeComprar()) {
            comandos.agregar({numero++, Comandos::Comprar, "Comprar esta propiedad"});
        }
        if (juego.puedeConstruir()) {
            comandos.agregar({numero++, Comandos::Construir, "Construir"});
        }
        if (juego.puedeHipotecar()) {
            comandos.agregar({numero++, Comandos::Hipotecar, "Hipotecar"});
        }
        if (juego.puedeDeshipotecar()) {
            comandos.agregar({numero++, Comandos::Deshipotecar, "Deshipotecar"});
        }
        comandos.agregar({numero++, Comandos::Pasar, "Pasar turno"});
    }

    comandos.agregar({numero++, Comandos::Estado, "Estado"});
    if (juego.hayUndo()) {
        comandos.agregar({numero++, Comandos::Undo, "Undo"});
    }
}

void Consola::mostrarOpciones() {
    const std::vector<Comandos::EntradaMenu>& ops = comandos.opciones();
    for (std::size_t i = 0; i < ops.size(); ++i) {
        std::cout << ops[i].numero << ". " << ops[i].texto << "\n";
    }
}

void Consola::run() {
    if (!activo || !juegoActual) {
        std::cerr << "Consola no inicializada.\n";
        return;
    }
    juegoActual->inicializarJugadores();
    std::cout << "Bienvenido al Monopoly en consola.\n";
    while (activo && !juegoActual->haTerminado()) {
        modelo::Jugador& j = juegoActual->jugadorActual();
        std::cout << "Turno: " << j.nombre() << " | Dinero $" << juegoActual->bancoJugador().getSaldo(j.nombre())
                  << " | Posicion " << j.posicion() << "\n";
        juegoActual->tableroJuego().describirCasilla(j.posicion());
        construirOpciones(*juegoActual);
        mostrarOpciones();
        std::cout << "Selecciona una opcion (0 para salir): ";
        int opcion = 0;
        std::cin >> opcion;
        if (!std::cin) break;
        if (opcion == 0) {
            activo = false;
            break;
        }
        if (!comandos.ejecutar(opcion, *juegoActual)) {
            std::cout << "Opcion inválida.\n";
        }
    }
    std::cout << "Juego terminado.\n";
}

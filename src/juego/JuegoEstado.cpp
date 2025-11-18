#include "Juego.h"
#include <iostream>

void Juego::prepararNuevoTurno() {
    opcionCompra = false;
    fase = DebeTirar;
}

void Juego::guardarEstado() {
    Snapshot snap;
    snap.banco = banco;
    snap.estado = estado;
    snap.turnos = turnos;
    snap.fase = fase;
    snap.opcionCompra = opcionCompra;
    snap.ultimoDado1 = ultimoDado1;
    snap.ultimoDado2 = ultimoDado2;
    snap.propiedades = tablero.capturarEstado(estado.jugadoresRef());
    snap.mazoCasualidad = mazoCasualidad;
    snap.mazoArca = mazoArca;
    historial.apilar(snap);
}

void Juego::restaurarEstado() {
    Snapshot snap;
    if (historial.desapilar(snap)) {
        banco = snap.banco;
        estado = snap.estado;
        turnos = snap.turnos;
        fase = snap.fase;
        opcionCompra = snap.opcionCompra;
        ultimoDado1 = snap.ultimoDado1;
        ultimoDado2 = snap.ultimoDado2;
        tablero.restaurarEstado(snap.propiedades, estado.jugadoresRefMutable());
        mazoCasualidad = snap.mazoCasualidad;
        mazoArca = snap.mazoArca;
    }
}

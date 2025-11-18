#include "Comandos.h"
#include "../juego/Juego.h"

Comandos::Comandos() {}

void Comandos::limpiar() {
    lista.clear();
}

void Comandos::agregar(const EntradaMenu& entrada) {
    lista.push_back(entrada);
}

bool Comandos::ejecutar(int numero, Juego& juego) {
    for (std::size_t i = 0; i < lista.size(); ++i) {
        if (lista[i].numero == numero) {
            Accion accion = lista[i].accion;
            if (accion == Tirar) juego.lanzarDados();
            else if (accion == Comprar) juego.comprarPropiedadActual();
            else if (accion == Construir) juego.intentarConstruir();
            else if (accion == Hipotecar) juego.intentarHipotecar();
            else if (accion == Deshipotecar) juego.intentarDeshipotecar();
            else if (accion == Estado) juego.mostrarEstado();
            else if (accion == Undo) juego.undo();
            else if (accion == Pasar) juego.pasar();
            else if (accion == UsarCarta) juego.usarCartaCarcel();
            else if (accion == PagarMulta) juego.pagarMultaCarcel();
            return true;
        }
    }
    return false;
}

const std::vector<Comandos::EntradaMenu>& Comandos::opciones() const {
    return lista;
}

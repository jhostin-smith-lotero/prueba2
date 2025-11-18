#include "Juego.h"
#include <iostream>

void Juego::avanzarJugador(modelo::Jugador& jugador, int pasos) {
    int posicionAnterior = jugador.posicion();
    int limite = tablero.totalCasillas();
    if (limite <= 0) {
        std::cout << "El tablero no tiene casillas cargadas.\n";
        return;
    }
    jugador.mover(pasos, limite);
    if (jugador.posicion() < posicionAnterior) {
        banco.recibirDelBanco(jugador.nombre(), reglamento.salarioGo());
        jugador.cobrar(reglamento.salarioGo());
        std::cout << "Cobras $" << reglamento.salarioGo() << " por pasar por GO.\n";
    }
}

void Juego::moverDirecto(modelo::Jugador& jugador, int destino, bool cobraGo) {
    int limite = tablero.totalCasillas();
    if (limite <= 0) return;
    int anterior = jugador.posicion();
    jugador.moverDirecto(destino, limite);
    if (cobraGo && destino < anterior) {
        banco.recibirDelBanco(jugador.nombre(), reglamento.salarioGo());
        jugador.cobrar(reglamento.salarioGo());
        std::cout << "Cobras $" << reglamento.salarioGo() << " por pasar por GO.\n";
    }
}

void Juego::evaluarPropiedad(modelo::Propiedad* propiedad, modelo::Jugador& jugador, int tirada) {
    opcionCompra = false;
    if (!propiedad) {
        return;
    }
    if (propiedad->disponible()) {
        opcionCompra = true;
        std::cout << "Propiedad sin dueño: " << propiedad->nombre() << " cuesta $" << propiedad->datos().precio() << "\n";
    } else if (propiedad->dueno() != &jugador && !propiedad->estaHipotecada()) {
        int renta = calculoRenta.calcular(propiedad, tirada);
        bool pago = banco.transferir(jugador.nombre(), propiedad->dueno()->nombre(), renta);
        if (pago && jugador.pagar(renta)) {
            propiedad->dueno()->cobrar(renta);
            std::cout << "Pagas renta de $" << renta << " a " << propiedad->dueno()->nombre() << "\n";
        } else {
            std::cout << "No tienes dinero suficiente para la renta.\n";
        }
    }
}

modelo::Propiedad* Juego::buscarPropiedad(const std::string& nombre) const {
    return indice.buscar(nombre);
}

bool Juego::grupoCompleto(const std::string& color, const modelo::Jugador& jugador) const {
    std::vector<modelo::Solar*> grupo = tablero.solaresPorColor(color);
    if (grupo.empty()) return false;
    for (std::size_t i = 0; i < grupo.size(); ++i) {
        if (grupo[i]->dueno() != &jugador) return false;
        if (grupo[i]->estaHipotecada()) return false;
    }
    return true;
}

int Juego::valorEdificacion(const modelo::Solar* solar) const {
    if (!solar) return 0;
    if (solar->hotel()) return 5;
    return solar->casas();
}

bool Juego::cumpleSimetria(const modelo::Solar* solar, const modelo::Jugador& jugador) const {
    if (!solar) return false;
    std::vector<modelo::Solar*> grupo = tablero.solaresPorColor(solar->color());
    if (grupo.empty()) return false;
    int minimo = 10;
    for (std::size_t i = 0; i < grupo.size(); ++i) {
        int valor = valorEdificacion(grupo[i]);
        if (valor < minimo) {
            minimo = valor;
        }
        if (grupo[i]->dueno() != &jugador) return false;
    }
    return valorEdificacion(solar) == minimo;
}

std::vector<modelo::Solar*> Juego::solaresConstruibles(const modelo::Jugador& jugador) const {
    std::vector<modelo::Solar*> lista;
    const std::vector<modelo::Propiedad*>& props = jugador.propiedades();
    for (std::size_t i = 0; i < props.size(); ++i) {
        modelo::Solar* solar = dynamic_cast<modelo::Solar*>(props[i]);
        if (!solar) continue;
        if (!grupoCompleto(solar->color(), jugador)) continue;
        if (solar->hotel()) continue;
        if (!cumpleSimetria(solar, jugador)) continue;
        lista.push_back(solar);
    }
    return lista;
}

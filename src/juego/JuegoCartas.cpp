#include "Juego.h"
#include <iostream>
#include <cstdio>

void Juego::resolverEspecial(modelo::Especial* especial, modelo::Jugador& jugador, int tirada) {
    if (!especial) return;
    modelo::Especial::TipoEspecial tipo = especial->tipoEspecial();
    if (tipo == modelo::Especial::GO) {
        std::cout << "Descansas en SALIDA y cobras $" << reglamento.salarioGo() << "\n";
        banco.recibirDelBanco(jugador.nombre(), reglamento.salarioGo());
        jugador.cobrar(reglamento.salarioGo());
    } else if (tipo == modelo::Especial::IMPUESTO) {
        int monto = especial->montoImpuesto();
        if (banco.pagarBanco(jugador.nombre(), monto) && jugador.pagar(monto)) {
            std::cout << "Pagas impuesto de $" << monto << "\n";
        } else {
            std::cout << "No tienes saldo para pagar impuesto de $" << monto << "\n";
        }
    } else if (tipo == modelo::Especial::IR_CARCEL) {
        std::cout << "Vas directo a la carcel.\n";
        jugador.encarcelar(posicionCarcel());
    } else if (tipo == modelo::Especial::CARCEL) {
        std::cout << "Estas de visita en la carcel.\n";
    } else if (tipo == modelo::Especial::PARQUE) {
        std::cout << "Parqueo libre, nada ocurre.\n";
    } else if (tipo == modelo::Especial::CASUALIDAD || tipo == modelo::Especial::ARCA) {
        modelo::Carta carta;
        bool tomada = false;
        if (tipo == modelo::Especial::CASUALIDAD) {
            tomada = mazoCasualidad.robar(carta);
        } else {
            tomada = mazoArca.robar(carta);
        }
        if (!tomada) {
            std::cout << "El mazo está vacio.\n";
            return;
        }
        std::cout << "Carta: " << carta.texto() << "\n";
        aplicarCarta(carta, jugador);
        if (!carta.conservar()) {
            if (tipo == modelo::Especial::CASUALIDAD) {
                mazoCasualidad.ponerAlFondo(carta);
            } else {
                mazoArca.ponerAlFondo(carta);
            }
        }
    }
    (void)tirada;
}

void Juego::aplicarCarta(const modelo::Carta& carta, modelo::Jugador& jugador) {
    if (carta.accion() == modelo::Carta::Accion::COBRAR) {
        banco.recibirDelBanco(jugador.nombre(), carta.valor());
        jugador.cobrar(carta.valor());
    } else if (carta.accion() == modelo::Carta::Accion::PAGAR) {
        if (!banco.pagarBanco(jugador.nombre(), carta.valor()) || !jugador.pagar(carta.valor())) {
            std::cout << "No puedes pagar $" << carta.valor() << "\n";
        }
    } else if (carta.accion() == modelo::Carta::Accion::MOVER) {
        moverDirecto(jugador, carta.destino(), true);
        tablero.describirCasilla(jugador.posicion());
        modelo::Casilla* c = tablero.obtener(jugador.posicion());
        if (c) { c->alCaer(*this, jugador, 0); }
        modelo::Propiedad* p = tablero.obtenerPropiedad(c);
        evaluarPropiedad(p, jugador, 0);
    } else if (carta.accion() == modelo::Carta::Accion::MOVER_RELATIVO) {
        avanzarJugador(jugador, carta.valor());
        tablero.describirCasilla(jugador.posicion());
        modelo::Casilla* c = tablero.obtener(jugador.posicion());
        if (c) { c->alCaer(*this, jugador, carta.valor()); }
        modelo::Propiedad* p = tablero.obtenerPropiedad(c);
        evaluarPropiedad(p, jugador, carta.valor());
    } else if (carta.accion() == modelo::Carta::Accion::SALIR_CARCEL) {
        jugador.recibirCartaSalirCarcel();
    } else if (carta.accion() == modelo::Carta::Accion::IR_CARCEL) {
        jugador.encarcelar(posicionCarcel());
    } else if (carta.accion() == modelo::Carta::Accion::REPARAR) {
        int costo = 0;
        const std::vector<modelo::Propiedad*>& props = jugador.propiedades();
        for (std::size_t i = 0; i < props.size(); ++i) {
            const modelo::Solar* s = dynamic_cast<const modelo::Solar*>(props[i]);
            if (!s) continue;
            costo += s->casas() * carta.valor();
            if (s->hotel()) costo += carta.destino();
        }
        if (costo > 0) {
            if (!banco.pagarBanco(jugador.nombre(), costo) || !jugador.pagar(costo)) {
                std::cout << "No puedes pagar reparaciones por $" << costo << "\n";
            }
        }
    }
}

void Juego::cargarMazo(const std::string& ruta, modelo::Mazo& mazo, bool esArca) {
    FILE* f = std::fopen(ruta.c_str(), "r");
    if (!f) {
        std::cout << "No se pudo abrir " << ruta << ", se usaran cartas básicas.\n";
        if (esArca) {
            mazo.agregar(modelo::Carta("Cobra $50", modelo::Carta::Accion::COBRAR, 50));
        } else {
            mazo.agregar(modelo::Carta("Paga $50", modelo::Carta::Accion::PAGAR, 50));
        }
        return;
    }
    char buffer[512];
    std::string texto;
    std::string tipo;
    int monto = 0;
    int destino = -1;
    int porCasa = 0;
    int porHotel = 0;
    while (std::fgets(buffer, sizeof(buffer), f)) {
        std::string linea(buffer);
        if (linea.find("\"descripcion\"") != std::string::npos) {
            std::size_t c1 = linea.find("\"");
            c1 = linea.find("\"", c1 + 1);
            std::size_t c2 = linea.find("\"", c1 + 1);
            std::size_t c3 = linea.find("\"", c2 + 1);
            if (c2 != std::string::npos && c3 != std::string::npos) {
                texto = linea.substr(c2 + 1, c3 - c2 - 1);
            }
        }
        if (linea.find("\"tipo\"") != std::string::npos) {
            std::size_t c1 = linea.find("\"", linea.find(":"));
            std::size_t c2 = linea.find("\"", c1 + 1);
            if (c1 != std::string::npos && c2 != std::string::npos) {
                tipo = linea.substr(c1 + 1, c2 - c1 - 1);
            }
        }
        if (linea.find("\"monto\"") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            monto = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { monto = monto * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("\"destino\"") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            destino = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { destino = destino * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("porCasa") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            porCasa = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { porCasa = porCasa * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("porHotel") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            porHotel = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { porHotel = porHotel * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("}") != std::string::npos) {
            if (tipo == "cobrar") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::COBRAR, monto));
            } else if (tipo == "pagar") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::PAGAR, monto));
            } else if (tipo == "mover_a") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::MOVER, monto, destino));
            } else if (tipo == "mover_rel") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::MOVER_RELATIVO, monto));
            } else if (tipo == "salir_carcel") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::SALIR_CARCEL, 0, 0, true));
            } else if (tipo == "ir_carcel") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::IR_CARCEL, 0));
            } else if (tipo == "reparar") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::REPARAR, porCasa, porHotel));
            }
            texto.clear();
            tipo.clear();
            monto = 0;
            destino = -1;
            porCasa = 0;
            porHotel = 0;
        }
    }
    std::fclose(f);
}

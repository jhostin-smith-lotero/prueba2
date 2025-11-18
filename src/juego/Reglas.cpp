#include "Reglas.h"
#include <iostream>
#include <cstdio>
#include <cctype>

Reglas::Reglas() : salarioGO(200), multaCarcelConfig(50), stockCasas(32), stockHoteles(12), interesHipotecaPorc(10) {}

static int leerEnteroSimple(const std::string& linea) {
    int numero = 0;
    bool enNumero = false;
    for (std::size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c >= '0' && c <= '9') {
            numero = numero * 10 + (c - '0');
            enNumero = true;
        } else if (c == '.') {
            continue;
        } else if (enNumero) {
            break;
        }
    }
    return numero;
}

void Reglas::cargarConfig(const std::string& ruta) {
    FILE* f = std::fopen(ruta.c_str(), "r");
    if (!f) {
        std::cout << "No se pudo abrir config, usando valores por defecto.\n";
        return;
    }
    char buffer[256];
    while (std::fgets(buffer, sizeof(buffer), f)) {
        std::string linea(buffer);
        if (linea.find("salario_go") != std::string::npos) {
            salarioGO = leerEnteroSimple(linea);
        } else if (linea.find("multa_carcel") != std::string::npos) {
            multaCarcelConfig = leerEnteroSimple(linea);
        } else if (linea.find("casas_stock_inicial") != std::string::npos) {
            stockCasas = leerEnteroSimple(linea);
        } else if (linea.find("hoteles_stock_inicial") != std::string::npos) {
            stockHoteles = leerEnteroSimple(linea);
        } else if (linea.find("interes_hipoteca") != std::string::npos) {
            interesHipotecaPorc = leerEnteroSimple(linea);
        }
    }
    std::fclose(f);
}

void Reglas::evaluarCasilla(modelo::Especial* especial, modelo::Jugador& jugador, Banco& banco, int posicionCarcel) const {
    if (!especial) return;
    modelo::Especial::TipoEspecial tipo = especial->tipoEspecial();
    if (tipo == modelo::Especial::GO) {
        std::cout << "Descansas en SALIDA y cobras $" << salarioGO << "\n";
        banco.recibirDelBanco(jugador.nombre(), salarioGO);
        jugador.cobrar(salarioGO);
    } else if (tipo == modelo::Especial::IMPUESTO) {
        int monto = especial->montoImpuesto();
        if (banco.pagarBanco(jugador.nombre(), monto) && jugador.pagar(monto)) {
            std::cout << "Pagas impuesto de $" << monto << "\n";
        } else {
            std::cout << "No tienes saldo para pagar impuesto de $" << monto << "\n";
        }
    } else if (tipo == modelo::Especial::IR_CARCEL) {
        std::cout << "Vas directo a la cárcel.\n";
        jugador.encarcelar(posicionCarcel);
    } else if (tipo == modelo::Especial::CARCEL) {
        std::cout << "Estás de visita en la cárcel.\n";
    } else if (tipo == modelo::Especial::PARQUE) {
        std::cout << "Parqueo libre, nada ocurre.\n";
    } else if (tipo == modelo::Especial::CASUALIDAD || tipo == modelo::Especial::ARCA) {
        std::cout << "Robas una carta: funcionalidad simplificada.\n";
    }
}


#include "Banco.h"
#include <iostream>

Banco::Banco() : stockCasas_(32), stockHoteles_(12) {}

void Banco::fijarStock(int casas, int hoteles) {
    stockCasas_ = casas;
    stockHoteles_ = hoteles;
}

void Banco::crearCuenta(const std::string& jugador, int montoInicial) {
    cuentas[jugador] = montoInicial;
}

bool Banco::transferir(const std::string& origen, const std::string& destino, int monto) {
    if (cuentas[origen] < monto) return false;
    cuentas[origen] -= monto;
    cuentas[destino] += monto;
    return true;
}

bool Banco::pagarBanco(const std::string& jugador, int monto) {
    if (cuentas[jugador] < monto) return false;
    cuentas[jugador] -= monto;
    return true;
}

bool Banco::recibirDelBanco(const std::string& jugador, int monto) {
    cuentas[jugador] += monto;
    return true;
}

int Banco::getSaldo(const std::string& jugador) const {
    std::map<std::string, int>::const_iterator it = cuentas.find(jugador);
    if (it != cuentas.end())
        return it->second;
    return 0;
}

bool Banco::tomarCasa() {
    if (stockCasas_ <= 0) return false;
    stockCasas_--;
    return true;
}

void Banco::devolverCasa() {
    stockCasas_++;
}

bool Banco::tomarHotel() {
    if (stockHoteles_ <= 0) return false;
    stockHoteles_--;
    return true;
}

void Banco::devolverHotel() {
    stockHoteles_++;
}


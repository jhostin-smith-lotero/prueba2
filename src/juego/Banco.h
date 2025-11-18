#ifndef BANCO_H
#define BANCO_H

#include <string>
#include <map>

/**
 * @class Banco
 * @brief Administra las cuentas de los jugadores del Monopoly.
 */
class Banco {
private:
    std::map<std::string, int> cuentas;
    int stockCasas_;
    int stockHoteles_;

public:
    Banco();

    void fijarStock(int casas, int hoteles);
    int stockCasas() const { return stockCasas_; }
    int stockHoteles() const { return stockHoteles_; }

    void crearCuenta(const std::string& jugador, int montoInicial = 1500);
    bool transferir(const std::string& origen, const std::string& destino, int monto);
    bool pagarBanco(const std::string& jugador, int monto);
    bool recibirDelBanco(const std::string& jugador, int monto);
    int getSaldo(const std::string& jugador) const;

    bool tomarCasa();
    void devolverCasa();
    bool tomarHotel();
    void devolverHotel();
};

#endif

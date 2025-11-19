#ifndef BANCO_H
#define BANCO_H

#include <string>
#include <map>

/**
 * @class Banco
 * @brief Administra el dinero y stock de edificaciones del juego.
 */
class Banco {
private:
    std::map<std::string, int> cuentas;
    int stockCasas_;
    int stockHoteles_;

public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Banco creado con stock en 0 y sin cuentas.
     */
    Banco();

    /**
     * @brief Asigna stock inicial.
     * @pre casas >= 0, hoteles >= 0.
     * @post El stock es actualizado.
     */
    void fijarStock(int casas, int hoteles);

    int stockCasas() const { return stockCasas_; }
    int stockHoteles() const { return stockHoteles_; }

    /**
     * @brief Crea una cuenta bancaria para un jugador.
     * @pre jugador no vacío.
     * @post Si no existía, se asigna montoInicial.
     */
    void crearCuenta(const std::string& jugador, int montoInicial = 1500);

    /**
     * @brief Transfiere dinero entre jugadores.
     * @pre cuentas deben existir y origen debe tener saldo suficiente.
     * @post saldoOrigen -= monto; saldoDestino += monto.
     * @return true si la transferencia se realiza.
     */
    bool transferir(const std::string& origen, const std::string& destino, int monto);

    /**
     * @brief Cobra dinero a un jugador (paga al banco).
     * @pre jugador debe existir y tener suficiente saldo.
     * @post saldoJugador -= monto.
     */
    bool pagarBanco(const std::string& jugador, int monto);

    /**
     * @brief El banco entrega dinero al jugador.
     * @pre jugador debe existir.
     * @post saldoJugador += monto.
     */
    bool recibirDelBanco(const std::string& jugador, int monto);

    /**
     * @brief Consulta el saldo de un jugador.
     * @pre jugador debe existir.
     * @post No modifica estado.
     */
    int getSaldo(const std::string& jugador) const;

    /**
     * @brief Reduce el stock de casas.
     * @pre stockCasas_ > 0.
     * @post stockCasas_--.
     */
    bool tomarCasa();

    /**
     * @brief Devuelve una casa al stock.
     * @pre Ninguna.
     * @post stockCasas_++.
     */
    void devolverCasa();

    /**
     * @brief Reduce el stock de hoteles.
     * @pre stockHoteles_ > 0.
     * @post stockHoteles_--.
     */
    bool tomarHotel();

    /**
     * @brief Devuelve un hotel al stock.
     * @pre Ninguna.
     * @post stockHoteles_++.
     */
    void devolverHotel();
};

#endif

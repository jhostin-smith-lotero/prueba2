#ifndef BANCO_H
#define BANCO_H

#include <string>
#include <unordered_map>

/**
 * @class Banco
 * @brief Administra las cuentas de los jugadores del Monopoly.
 */
class Banco {
private:
    std::unordered_map<std::string, int> cuentas;

public:
    /**
     * @brief Constructor del Banco.
     * @pre Ninguna.
     * @post Banco inicializado con mapa vacío de cuentas.
     */
    Banco();

    /**
     * @brief Crea una cuenta bancaria para un jugador.
     * @param jugador Nombre del jugador.
     * @param montoInicial Dinero inicial en su cuenta.
     * @pre jugador no debe estar vacío y no debe existir previamente.
     * @post Se registra una nueva entrada en el mapa con el saldo inicial.
     */
    void crearCuenta(const std::string& jugador, int montoInicial = 1500);

    /**
     * @brief Transfiere dinero entre dos jugadores.
     * @param origen Jugador que envía el dinero.
     * @param destino Jugador que recibe el dinero.
     * @param monto Cantidad a transferir.
     * @pre origen y destino deben existir en cuentas.
     * @pre El saldo de origen debe ser >= monto.
     * @post origen disminuye monto, destino lo incrementa.
     * @return true si la transferencia tuvo éxito, false si saldo insuficiente.
     */
    bool transferir(const std::string& origen, const std::string& destino, int monto);

    /**
     * @brief El jugador paga al banco.
     * @param jugador Nombre del jugador.
     * @param monto Cantidad a pagar.
     * @pre jugador debe existir en cuentas y tener saldo suficiente.
     * @post El saldo del jugador disminuye en monto.
     */
    bool pagarBanco(const std::string& jugador, int monto);

    /**
     * @brief El banco entrega dinero al jugador.
     * @param jugador Nombre del jugador.
     * @param monto Cantidad a entregar.
     * @pre jugador debe existir en cuentas.
     * @post El saldo del jugador aumenta en monto.
     */
    bool recibirDelBanco(const std::string& jugador, int monto);

    /**
     * @brief Consulta el saldo de un jugador.
     * @param jugador Nombre del jugador.
     * @pre jugador debe existir en cuentas.
     * @post No modifica estado.
     */
    int getSaldo(const std::string& jugador) const;
};

#endif

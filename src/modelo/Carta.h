#ifndef CARTA_H
#define CARTA_H
#include <string>

namespace modelo {

    /**
     * @class Carta
     * @brief Representa una carta de Arca o Casualidad.
     *
     * Una carta contiene un texto, una acción y valores asociados.
     * El efecto se aplica cuando un jugador la roba del mazo.
     */
    struct Carta {
    public:
        /**
         * @brief Tipos de acción que una carta puede ejecutar.
         */
        enum class Accion {
            COBRAR,         ///< Jugador recibe dinero.
            PAGAR,          ///< Jugador paga dinero.
            MOVER,          ///< Movimiento directo a casilla destino.
            MOVER_RELATIVO, ///< Movimiento relativo (ej. retroceder 3 casillas).
            SALIR_CARCEL,   ///< Jugador recibe carta para salir de la cárcel.
            IR_CARCEL,      ///< Jugador va directamente a la cárcel.
            REPARAR         ///< Pago por reparaciones (casas/hoteles).
        };

        /**
         * @brief Constructor por defecto.
         * @pre Ninguna.
         * @post Carta creada con valores por defecto.
         */
        Carta() = default;

        /**
         * @brief Constructor parametrizado.
         * @pre texto no vacío.
         * @post Todos los campos quedan inicializados.
         */
        Carta(const std::string& texto, Accion accion, int valor, int destino = -1, bool conservar = false);

        /// Accesores — no modifican estado
        const std::string& texto() const { return texto_; }
        Accion accion() const { return accion_; }
        int valor() const { return valor_; }
        int destino() const { return destino_; }
        bool conservar() const { return conservar_; }

    private:
        std::string texto_;   ///< Descripción de la carta
        Accion accion_ = Accion::COBRAR;
        int valor_ = 0;       ///< Monto o cantidad asociada
        int destino_ = -1;    ///< Casilla destino si aplica
        bool conservar_ = false; ///< Si se debe guardar (salir cárcel)
    };

}
#endif //CARTA_H

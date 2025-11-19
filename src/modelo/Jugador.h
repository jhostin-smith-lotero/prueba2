#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>
#include <vector>

namespace modelo {

    class Propiedad;

    /**
     * @class Jugador
     * @brief Representa un jugador del juego Monopoly.
     *
     * Maneja dinero, posición, propiedades, cárcel, cartas y movimientos.
     */
    struct Jugador {
    public:
        /**
         * @brief Constructor de jugador.
         * @pre nombre no vacío.
         * @post Jugador inicializado con dinero=0 y sin propiedades.
         */
        explicit Jugador(const std::string& nombre = "");

        const std::string& nombre() const { return nombre_; }
        int dinero() const { return dinero_; }
        int posicion() const { return posicion_; }
        bool enCarcel() const { return enCarcel_; }
        int turnosEnCarcel() const { return turnosEnCarcel_; }
        int cartasSalirCarcel() const { return cartasSalirCarcel_; }
        const std::vector<Propiedad*>& propiedades() const { return propiedades_; }

        /// —————————— MODIFICADORES ———————————

        /**
         * @brief Cambia el dinero del jugador.
         * @pre Ninguna.
         * @post dinero = nuevo valor.
         */
        void setDinero(int dinero) { dinero_ = dinero; }

        /**
         * @brief Mueve al jugador un número de pasos.
         * @pre limite > 0.
         * @post posición actualizada, respetando vuelta al tablero.
         */
        void mover(int pasos, int limite);

        /**
         * @brief Coloca al jugador directamente en una casilla.
         * @pre 0 <= posicion < limite.
         * @post posición = posicion.
         */
        void moverDirecto(int posicion, int limite);

        /**
         * @brief Ajusta el dinero sumando o restando.
         * @post dinero_ modificado.
         */
        void ajustarDinero(int delta);

        /**
         * @brief Intenta pagar un monto.
         * @pre monto >= 0.
         * @post dinero disminuye si es posible.
         * @return true si pudo pagar.
         */
        bool pagar(int monto);

        /**
         * @brief Añade dinero al jugador.
         * @pre monto >= 0.
         * @post dinero_ incrementado.
         */
        void cobrar(int monto);

        /**
         * @brief Encarcela al jugador.
         * @pre posicionCarcel >= 0.
         * @post enCarcel_ = true, posicion = carcel.
         */
        void encarcelar(int posicionCarcel);

        /**
         * @brief Libera de cárcel.
         * @post enCarcel_ = false, turnosEnCarcel_ = 0.
         */
        void liberarDeCarcel();

        /**
         * @pre Jugador está en cárcel.
         * @post turnosEnCarcel_ incrementado.
         */
        void incrementarTurnoCarcel();

        void reiniciarTurnoCarcel();

        /// Cartas
        void recibirCartaSalirCarcel();
        bool usarCartaSalirCarcel();

        /// Propiedades
        void agregarPropiedad(Propiedad* propiedad);
        void quitarPropiedad(Propiedad* propiedad);
        void limpiarPropiedades();

    private:
        std::string nombre_;
        int dinero_ = 0;
        int posicion_ = 0;
        bool enCarcel_ = false;
        int turnosEnCarcel_ = 0;
        int cartasSalirCarcel_ = 0;
        std::vector<Propiedad*> propiedades_;
    };

}
#endif //JUGADOR_H

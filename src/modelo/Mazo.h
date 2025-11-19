#ifndef MAZO_H
#define MAZO_H

#include "Carta.h"
#include "../edd/Cola.h"

namespace modelo {

    /**
     * @class Mazo
     * @brief Representa un mazo de cartas tipo cola (Arca o Casualidad).
     *
     * Internamente utiliza una cola FIFO para garantizar el orden clásico del mazo.
     */
    struct Mazo {
    public:
        /**
         * @brief Constructor por defecto.
         * @pre Ninguna.
         * @post Mazo vacío inicializado.
         */
        Mazo() {}

        /**
         * @brief Agrega una carta al fondo del mazo.
         * @pre carta es válida.
         * @post Carta añadida al final de la cola.
         */
        void agregar(const Carta& carta) { cartas_.enqueue(carta); }

        /**
         * @brief Roba la carta superior del mazo.
         * @param carta Referencia donde se guardará la carta robada.
         * @pre Mazo no vacío.
         * @post carta contiene el primer elemento; este se elimina del mazo.
         * @return true si se pudo robar, false si estaba vacío.
         */
        bool robar(Carta& carta) { return cartas_.dequeue(carta); }

        /**
         * @brief Devuelve la carta al fondo del mazo (ej. salir cárcel).
         * @pre Ninguna.
         * @post Carta agregada nuevamente al final.
         */
        void ponerAlFondo(const Carta& carta) { cartas_.enqueue(carta); }

        /**
         * @return true si no hay cartas en el mazo.
         */
        bool vacio() const { return cartas_.vacia(); }

        /**
         * @return Número de cartas en el mazo.
         */
        std::size_t tam() const { return cartas_.tam(); }

    private:
        edd::Cola<Carta> cartas_; ///< Cola que almacena las cartas del mazo.
    };

}

#endif //MAZO_H

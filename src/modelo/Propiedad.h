#ifndef PROPIEDAD_H
#define PROPIEDAD_H

#include "Casilla.h"
#include "Escritura.h"

namespace modelo {

    /**
     * @class Propiedad
     * @brief Clase base para todas las casillas adquiribles (solares, servicios y ferrocarriles).
     */
    struct Propiedad : public Casilla {
    public:
        /**
         * @brief Constructor.
         * @pre escritura válida.
         * @post Propiedad creada sin dueño y no hipotecada.
         */
        Propiedad(const std::string& nombre, const Escritura& escritura, Casilla::Tipo tipoPropiedad)
            : Casilla(nombre, tipoPropiedad), escritura_(escritura), dueno_(nullptr), hipotecada_(false) {}

        Jugador* dueno() const { return dueno_; }
        bool estaHipotecada() const { return hipotecada_; }
        const Escritura& datos() const { return escritura_; }

        /**
         * @brief Indica si no tiene dueño.
         * @return true si está disponible.
         */
        bool disponible() const { return dueno_ == nullptr; }

        /**
         * @brief Asigna dueño a la propiedad.
         * @pre jugador no nulo.
         * @post dueno_ = jugador.
         */
        void asignarDueno(Jugador* jugador) { dueno_ = jugador; }

        /**
         * @brief Hipoteca la propiedad.
         * @pre Propiedad no debe estar hipotecada.
         * @post hipotecada_ = true.
         */
        void hipotecar() { hipotecada_ = true; }

        /**
         * @brief Cancela la hipoteca.
         * @pre Propiedad debe estar hipotecada.
         * @post hipotecada_ = false.
         */
        void deshipotecar() { hipotecada_ = false; }

        /**
         * @brief Renta base sin modificaciones (solar redefine).
         * @return Renta estándar de escritura.
         */
        virtual int rentaBase(int tirada) const { (void)tirada; return escritura_.rentaBase(); }

        /**
         * @brief Lógica al caer en la propiedad.
         * @pre juego y jugador válidos.
         * @post Se ejecuta compra o pago de renta según dueño.
         */
        void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

    protected:
        Escritura escritura_;
        Jugador* dueno_;
        bool hipotecada_;
    };

}

#endif //PROPIEDAD_H

#ifndef CALCULORENTA_H
#define CALCULORENTA_H

/**
 * @file CalculoRenta.h
 * @brief Cálculo de renta para diferentes tipos de propiedad (Solar, Servicio, Ferrocarril).
 *
 * Reglas aplicadas:
 * - Solar: renta según casas u hotel.
 * - Servicio: renta = tirada * multiplicador (1 o 2 servicios).
 * - Ferrocarril: renta según cantidad de ferrocarriles del propietario.
 */

namespace modelo { struct Propiedad; }

/**
 * @class CalculoRenta
 * @brief Determina la renta que debe pagarse al caer en una propiedad.
 */
class CalculoRenta {
public:

    /**
     * @brief Calcula la renta de una propiedad.
     *
     * @param propiedad Puntero a la propiedad evaluada.
     *
     * @pre propiedad != nullptr.
     * @pre propiedad debe tener un dueño distinto del jugador que cae
     *      (esta verificación ocurre en la lógica superior).
     *
     * @post No modifica el estado de la propiedad.
     * @post Retorna un entero mayor o igual a 0.
     *
     * @return Monto de dinero que debe pagarse al propietario.
     */
    int calcular(const modelo::Propiedad* propiedad) const;
};

#endif // CALCULORENTA_H

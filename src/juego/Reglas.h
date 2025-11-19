#ifndef REGLAS_H
#define REGLAS_H

#include "Banco.h"
#include "Estado.h"
#include "../modelo/Jugador.h"
#include "../modelo/Casilla.h"
#include "../modelo/Especial.h"
#include <string>

/**
 * @class Reglas
 * @brief Procesa reglas del juego y parámetros configurables.
 *
 * Contiene la lógica para evaluar casillas especiales (GO, cárcel, impuestos,
 * y el envío a casilla especial) y parámetros como salarios y multas.
 */
class Reglas {
public:
    /**
     * @brief Constructor por defecto.
     * @pre Ninguna.
     * @post Valores por defecto cargados para salarios y multas.
     */
    Reglas();

    /**
     * @brief Carga configuración desde un archivo de texto (ruta).
     * @param ruta Ruta del archivo de configuración.
     * @pre El archivo debe existir y tener formato correcto.
     * @post Los parámetros internos (salarioGO, multas, stock) quedan actualizados.
     */
    void cargarConfig(const std::string& ruta);

    /**
     * @brief Evalúa el efecto de una casilla especial sobre un jugador.
     * @param especial Puntero a la casilla Especial invocada.
     * @param jugador Referencia al jugador que cayó.
     * @param banco Referencia al banco para movimientos monetarios.
     * @param posicionCarcel Posición numérica de la cárcel en el tablero.
     * @pre especial != nullptr; jugador válido; banco inicializado.
     * @post Se aplican efectos (cobro/go, encarcelar, multas, robar carta, etc.).
     */
    void evaluarCasilla(modelo::Especial* especial, modelo::Jugador& jugador, Banco& banco, int posicionCarcel) const;

    /**
     * @brief Obtiene salario al pasar por GO.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    int salarioGo() const { return salarioGO; }

    int multaCarcel() const { return multaCarcelConfig; }
    int stockCasasInicial() const { return stockCasas; }
    int stockHotelesInicial() const { return stockHoteles; }
    int interesHipoteca() const { return interesHipotecaPorc; }

private:
    int salarioGO;
    int multaCarcelConfig;
    int stockCasas;
    int stockHoteles;
    int interesHipotecaPorc;
};

#endif

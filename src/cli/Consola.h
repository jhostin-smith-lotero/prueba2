#ifndef CONSOLA_H
#define CONSOLA_H

#include "Comandos.h"
#include <string>

/**
 * @file Consola.h
 * @brief Interfaz de línea de comandos que utiliza la clase Comandos.
 *
 * Interactúa con la clase Juego por medio de callbacks registrados.
 */

class Juego; // forward

/**
 * @class Consola
 * @brief Bucle principal de entrada/salida (CLI).
 */
class Consola {
public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Consola creada sin callbacks registrados.
     */
    Consola();

    /**
     * @brief Inicializa la consola y registra comandos básicos interactuando con el juego.
     * @param juego Puntero al objeto Juego (no nulo).
     * @pre juego debe estar inicializado.
     * @post Se registran comandos básicos que llaman a métodos de 'juego'.
     */
    void inicializar(Juego* juego);

    /**
     * @brief Ejecuta el bucle interactivo hasta recibir el comando 'salir'.
     * @pre inicializar() debió haberse llamado previamente.
     * @post El programa sale cuando el usuario escribe 'salir' o similar.
     */
    void run();

private:
    Comandos comandos;
    bool activo;
    Juego* juegoActual;

    void construirOpciones(Juego& juego);
    void mostrarOpciones();
};

#endif // CONSOLA_H

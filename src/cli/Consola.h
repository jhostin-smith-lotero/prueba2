#ifndef CONSOLA_H
#define CONSOLA_H

#include "Comandos.h"
#include <string>

/**
 * @file Consola.h
 * @brief Interfaz de texto (CLI) que usa la clase Comandos para ejecutar acciones del juego.
 */

class Juego;

/**
 * @class Consola
 * @brief Se encarga de recibir entrada del usuario y ejecutar comandos.
 */
class Consola {
public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Crea una consola inactiva y sin juego asociado.
     */
    Consola();

    /**
     * @brief Inicializa la consola y prepara la lista de comandos.
     * @param juego Puntero al juego.
     * @pre juego != nullptr.
     * @post El menú queda configurado y la consola lista para ejecutarse.
     */
    void inicializar(Juego* juego);

    /**
     * @brief Ejecuta el bucle principal de interacción por texto.
     * @pre inicializar() ya fue llamado.
     * @post El bucle termina únicamente cuando el usuario ingresa el comando "salir".
     */
    void run();

private:
    Comandos comandos;
    bool activo;
    Juego* juegoActual;

    /**
     * @brief Construye el menú dinámico según el estado actual del juego.
     * @pre juego debe estar inicializado.
     * @post La lista de comandos queda actualizada.
     */
    void construirOpciones(Juego& juego);

    /**
     * @brief Muestra las opciones del menú en pantalla.
     * @pre Ninguna.
     * @post No modifica estado del juego ni la consola.
     */
    void mostrarOpciones();
};

#endif // CONSOLA_H

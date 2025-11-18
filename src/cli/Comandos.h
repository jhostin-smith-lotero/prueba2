#ifndef COMANDOS_H
#define COMANDOS_H

#include <string>
#include <functional>
#include <map>

/**
 * @file Comandos.h
 * @brief Registro y ejecución de comandos de texto vinculados a funciones.
 *
 * @note Este módulo es independiente de la CLI y guarda punteros a funciones que
 * operan sobre el juego (por ejemplo mediante callbacks que usan la clase Juego).
 */

class Juego; // forward-declare

/**
 * @class Comandos
 * @brief Administra el mapeo entre nombre de comando y su acción.
 */
class Comandos {
public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Objeto de comandos inicializado sin comandos registrados.
     */
    Comandos();

    /**
     * @brief Registra un comando.
     * @param nombre Nombre del comando.
     * @param accion Función a ejecutar (callback).
     * @pre nombre no debe estar vacío; accion debe ser válida.
     * @post El comando queda registrado y puede ejecutarse por su nombre.
     */
    void registrar(const std::string& nombre, std::function<void(const std::string&)> accion);

    /**
     * @brief Ejecuta un comando si existe.
     * @param linea Completa de entrada que contiene comando y argumentos.
     * @pre linea no debe estar vacía.
     * @post Si existe el comando, se ejecuta la función asociada.
     * @return true si se encontró y ejecutó el comando; false si no existe.
     */
    bool ejecutar(const std::string& linea) const;

    /**
     * @brief Lista los comandos registrados.
     * @pre Ninguna.
     * @post No modifica el estado.
     */
    std::map<std::string, std::function<void(const std::string&)>> listar() const;

private:
    std::map<std::string, std::function<void(const std::string&)>> tabla;
};

#endif // COMANDOS_H

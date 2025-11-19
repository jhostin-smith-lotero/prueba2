#ifndef COMANDOS_H
#define COMANDOS_H

#include <string>
#include <vector>

class Juego;

/**
 * @class Comandos
 * @brief Administra un conjunto de acciones disponibles para el jugador.
 *
 * Cada comando se asocia a un número del menú y a una acción concreta
 * que luego será ejecutada sobre el juego.
 */
class Comandos {
public:
    /**
     * @enum Accion
     * @brief Conjunto de acciones disponibles dentro del menú.
     */
    enum Accion {
        Tirar,
        Comprar,
        Construir,
        Hipotecar,
        Deshipotecar,
        Estado,
        Undo,
        Pasar,
        UsarCarta,
        PagarMulta
    };

    /**
     * @struct EntradaMenu
     * @brief Representa una opción del menú mostrada al jugador.
     */
    struct EntradaMenu {
        int numero;          ///< Número de opción del menú.
        Accion accion;       ///< Acción a ejecutar.
        std::string texto;   ///< Texto mostrado al usuario.
    };

    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Crea una lista vacía de comandos.
     */
    Comandos();

    /**
     * @brief Limpia la lista de opciones disponibles.
     * @pre Ninguna.
     * @post La lista queda vacía.
     */
    void limpiar();

    /**
     * @brief Agrega una nueva entrada al menú.
     * @param entrada Entrada a añadir.
     * @pre Ninguna.
     * @post La entrada es agregada al vector interno.
     */
    void agregar(const EntradaMenu& entrada);

    /**
     * @brief Ejecuta la acción asociada a un número de menú.
     * @param numero Número de la opción elegida.
     * @param juego Referencia al juego sobre el cual se ejecuta la acción.
     * @pre El número debe existir en la lista.
     * @pre El juego debe estar correctamente inicializado.
     * @post Se llama al método correspondiente en `juego`.
     * @return true si se ejecutó la acción; false si la opción no existe.
     */
    bool ejecutar(int numero, Juego& juego);

    /**
     * @brief Obtiene la lista de opciones disponibles.
     * @pre Ninguna.
     * @post No modifica el estado interno.
     * @return Referencia constante al vector interno.
     */
    const std::vector<EntradaMenu>& opciones() const;

private:
    std::vector<EntradaMenu> lista; ///< Lista de entradas de menú.
};

#endif // COMANDOS_H

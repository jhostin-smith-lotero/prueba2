#ifndef ESTADO_H
#define ESTADO_H

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @struct Jugador
 * @brief Representa la información básica de un jugador.
 */
struct Jugador {
    std::string nombre;
    int posicion;
    bool enCarcel;
};

/**
 * @class Estado
 * @brief Contiene información dinámica del juego.
 */
class Estado {
private:
    std::vector<Jugador> jugadores;
    int turnoActual;
    std::unordered_map<std::string, std::string> propiedades;

public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Estado inicializado sin jugadores y turnoActual = 0.
     */
    Estado();

    /**
     * @brief Agrega un jugador al estado del juego.
     * @pre nombre no debe estar vacío.
     * @post Se añade un jugador con posición 0 y enCarcel=false.
     */
    void agregarJugador(const std::string& nombre);

    /**
     * @brief Obtiene el jugador del turno actual.
     * @pre Debe existir al menos un jugador agregado.
     * @post No modifica estado.
     */
    Jugador& getJugadorActual();

    /**
     * @brief Avanza al siguiente turno.
     * @pre Debe haber al menos un jugador registrado.
     * @post turnoActual incrementa circularmente.
     */
    void siguienteTurno();

    /**
     * @brief Obtiene el número del turno actual.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    int getTurnoActual() const;

    /**
     * @brief Retorna la lista completa de jugadores.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    std::vector<Jugador> getJugadores() const;

    /**
     * @brief Asigna una propiedad a un jugador.
     * @pre propiedad no debe ser vacía; jugador debe existir.
     * @post Se actualiza el dueño de la propiedad en el mapa.
     */
    void asignarPropiedad(const std::string& propiedad, const std::string& jugador);

    /**
     * @brief Obtiene el propietario de una propiedad.
     * @pre propiedad debe existir en el mapa o haber sido registrada.
     * @post No modifica estado.
     */
    std::string getPropietario(const std::string& propiedad);
};

#endif

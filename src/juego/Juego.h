#ifndef JUEGO_H
#define JUEGO_H

#include "Banco.h"
#include "Dado.h"
#include "Estado.h"
#include "Reglas.h"
#include "Turnos.h"
#include "../modelo/Tablero.h"
#include "../modelo/Mazo.h"
#include "../edd/Pila.h"
#include "../edd/TablaHash.h"
#include "../reglas/Construccion.h"
#include "../reglas/Hipoteca.h"
#include <vector>

/**
 * @class Juego
 * @brief Coordinador principal del juego Monopoly.
 *
 * Integra todos los módulos: Banco, Estado, Reglas, Turnos, Tablero, Mazo,
 * además de mantener un historial (pila) para deshacer movimientos (undo).
 */
class Juego {
private:
    Banco banco;
    Dado dado1, dado2;
    Estado estado;
    Reglas reglamento;
    Turnos turnos;
    modelo::Tablero tablero;
    modelo::Mazo mazoCasualidad;
    modelo::Mazo mazoArca;
    edd::TablaHash indice;
    bool partidaIniciada;
    bool opcionCompra;
    int ultimoDado1;
    int ultimoDado2;

public:
    /**
     * @brief Fases posibles dentro del turno de un jugador.
     */
    enum FaseTurno { DebeTirar, PostTirada };

private:
    FaseTurno fase;

    /**
     * @struct Snapshot
     * @brief Estructura que guarda el estado necesario para poder hacer "undo".
     *
     * Contiene copias (o estados serializables) de las estructuras que cambian
     * durante el juego. La implementación asume que el constructor por copia
     * o los métodos capturar/restaurar están disponibles para los tipos usados.
     */
    struct Snapshot {
        Banco banco;
        Estado estado;
        Turnos turnos;
        FaseTurno fase;
        bool opcionCompra;
        int ultimoDado1;
        int ultimoDado2;
        std::vector<modelo::Tablero::EstadoPropiedad> propiedades;
        modelo::Mazo mazoCasualidad;
        modelo::Mazo mazoArca;
    };

    edd::Pila<Snapshot> historial;

public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Se crean las estructuras internas con valores por defecto;
     *       partidaIniciada == false.
     */
    Juego();

    /**
     * @brief Inicializa jugadores pidiendo nombres por consola (o desde UI).
     * @pre Se debe llamar antes de iniciar turnos.
     * @post Se agregan N jugadores en estado y se crean cuentas en Banco.
     */
    void inicializarJugadores();

    /**
     * @brief Lanza ambos dados y guarda la tirada en ultimoDado1/2.
     * @pre La partida debe estar iniciada.
     * @post ultimoDado1 y ultimoDado2 contienen valores 1..6 y fase se actualiza.
     */
    void lanzarDados();

    /**
     * @brief Compra la propiedad en la casilla actual del jugador (si procede).
     * @pre Debe existir una propiedad disponible en la posición actual del jugador.
     * @pre jugador actual debe tener saldo suficiente.
     * @post Si se compra, se transfiere dinero y la propiedad queda asignada.
     */
    void comprarPropiedadActual();

    /**
     * @brief Intenta construir (casa/hotel) en las propiedades del jugador.
     * @pre Debe existir al menos una propiedad construible.
     * @post Si la construcción es posible, se actualizan propiedades y stock del banco.
     */
    void intentarConstruir();

    /**
     * @brief Intenta hipotecar la propiedad seleccionada del jugador.
     * @pre Propiedad debe pertenecer al jugador y no estar hipotecada.
     * @post Si es exitosa, propiedad queda hipotecada y jugador recibe dinero.
     */
    void intentarHipotecar();

    /**
     * @brief Intenta deshipotecar una propiedad del jugador.
     * @pre Propiedad debe estar hipotecada y pertenecer al jugador.
     * @post Si es exitosa, propiedad deja de estar hipotecada y se descuenta pago.
     */
    void intentarDeshipotecar();

    /**
     * @brief Usar carta "salir de la cárcel" si la tiene.
     * @pre Jugador actual debe existir.
     * @post Si el jugador posee la carta, la consume y lo libera de la cárcel.
     */
    void usarCartaCarcel();

    /**
     * @brief Paga la multa por salir de la cárcel.
     * @pre Jugador en cárcel y saldo suficiente.
     * @post El jugador paga la multa y se libera de la cárcel.
     */
    void pagarMultaCarcel();

    /**
     * @brief Pasar el turno sin ejecutar otras acciones (cuando aplica).
     * @pre Turno en estado PostTirada o acción permitida.
     * @post Se finaliza el turno actual y se avanza al siguiente jugador.
     */
    void pasar();

    /**
     * @brief Deshace el último snapshot guardado (undo).
     * @pre Debe existir al menos un snapshot en historial (hayUndo()==true).
     * @post El juego se restaura al estado del snapshot extraído.
     */
    void undo();

    /**
     * @brief Muestra en consola (o UI) el estado actual completo del juego.
     * @pre jugadores inicializados.
     * @post No modifica estado.
     */
    void mostrarEstado() const;

    /**
     * @brief Resuelve el efecto de una casilla especial (por ejemplo Arca/Casualidad).
     * @param especial puntero a la casilla especial invocada.
     * @param jugador referencia al jugador que cayó.
     * @param tirada suma de dados (si aplica).
     * @pre especial != nullptr, jugador válido.
     * @post Efecto de la casilla aplicado (movimiento, pago, carta, etc.).
     */
    void resolverEspecial(modelo::Especial* especial, modelo::Jugador& jugador, int tirada);

    /**
     * @brief Indica si el juego ha terminado.
     * @pre jugadores inicializados.
     * @post No modifica estado.
     * @return true si queda <= 1 jugador con saldo > 0.
     */
    bool haTerminado() const;

    /* ---------- Consultas de permisos para la UI / CLI ---------- */

    /**
     * @brief Indica si el jugador actual puede comprar la propiedad en la que está.
     * @pre Jugador inicializado.
     * @post No modifica estado.
     */
    bool puedeComprar() const;

    bool puedeConstruir() const;
    bool puedeHipotecar() const;
    bool puedeDeshipotecar() const;
    bool puedePagarMulta() const;
    bool tieneCartaSalir() const;
    bool hayUndo() const;
    bool puedeTirar() const;
    bool puedePasar() const;

    /**
     * @brief Devuelve la fase actual del turno.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    FaseTurno faseActual() const { return fase; }

    /**
     * @brief Devuelve el jugador actual (referencia).
     * @pre Debe existir al menos un jugador.
     * @post No modifica estado.
     */
    modelo::Jugador& jugadorActual();

    /**
     * @brief Acceso a reglas y banco para uso interno o pruebas.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    const Reglas& reglas() const { return reglamento; }
    Reglas& reglas() { return reglamento; }
    Banco& bancoJugador() { return banco; }
    modelo::Tablero& tableroJuego() { return tablero; }

    /**
     * @brief Devuelve la posición de la cárcel según el tablero.
     * @pre Tablero inicializado.
     * @post No modifica estado.
     */
    int posicionCarcel() const { return tablero.posicionCarcel(); }

private:
    /* ---- Métodos auxiliares internos; documentados por contrato aquí ---- */

    /**
     * @brief Avanza un jugador N pasos y aplica GO si corresponde.
     * @pre jugador existe; pasos >= 0.
     * @post jugador.posicion se actualiza y se aplica efecto GO si pasó por la casilla 0.
     */
    void avanzarJugador(modelo::Jugador& jugador, int pasos);

    /**
     * @brief Mueve directamente un jugador a una casilla destino.
     * @pre destino válido dentro de tablero.
     * @post jugador.posicion = destino; puede cobrar GO si cobraGo==true.
     */
    void moverDirecto(modelo::Jugador& jugador, int destino, bool cobraGo);

    /**
     * @brief Aplica el efecto de una carta robada al jugador.
     * @pre carta válida; jugador válido.
     * @post Se aplican movimientos, cobros o cambios indicados en la carta.
     */
    void aplicarCarta(const modelo::Carta& carta, modelo::Jugador& jugador);

    /**
     * @brief Carga un mazo desde un archivo simple de texto.
     * @pre ruta válida y archivo legible.
     * @post mazo tiene cartas cargadas en orden.
     */
    void cargarMazo(const std::string& ruta, modelo::Mazo& mazo, bool esArca);

    /**
     * @brief Preparar variables para el inicio de un nuevo turno.
     * @pre Estado del juego válido.
     * @post Se actualizan FaseTurno, opcionCompra y demás flags.
     */
    void prepararNuevoTurno();

    /**
     * @brief Evalúa lógica al caer en una propiedad (cobro de renta, compra, etc.)
     * @pre propiedad != nullptr y jugador válido.
     * @post Se aplican pagos, compras o asignaciones de dueño si corresponde.
     */
    void evaluarPropiedad(modelo::Propiedad* propiedad, modelo::Jugador& jugador, int tirada);

    /**
     * @brief Busca una propiedad por nombre (usa indice).
     * @pre nombre no vacío.
     * @post Retorna puntero a Propiedad o nullptr si no existe.
     */
    modelo::Propiedad* buscarPropiedad(const std::string& nombre) const;

    /**
     * @brief Guarda un snapshot del estado actual en la pila para posible undo.
     * @pre Estado válido.
     * @post Se empuja un Snapshot nuevo en 'historial'.
     */
    void guardarEstado();

    /**
     * @brief Restaura el último snapshot guardado (invocado por undo()).
     * @pre hayUndo() == true.
     * @post Estado del juego queda exactamente como en el snapshot restaurado.
     */
    void restaurarEstado();

    /**
     * @brief Determina si un jugador posee todas las propiedades de un color (grupo).
     * @pre color no vacío; jugador válido.
     * @post No modifica estado.
     * @return true si el jugador posee todas las casillas del grupo.
     */
    bool grupoCompleto(const std::string& color, const modelo::Jugador& jugador) const;

    /**
     * @brief Calcula el valor de edificación (costo) para un solar.
     * @pre solar != nullptr.
     * @post No modifica estado.
     */
    int valorEdificacion(const modelo::Solar* solar) const;

    /**
     * @brief Comprueba simetría de construcción entre todas las casillas de un grupo.
     * @pre solar != nullptr y jugador válido.
     * @post No modifica estado.
     */
    bool cumpleSimetria(const modelo::Solar* solar, const modelo::Jugador& jugador) const;

    /**
     * @brief Retorna lista de solares donde el jugador puede construir.
     * @pre jugador válido.
     * @post No modifica estado.
     */
    std::vector<modelo::Solar*> solaresConstruibles(const modelo::Jugador& jugador) const;
};

#endif // JUEGO_H

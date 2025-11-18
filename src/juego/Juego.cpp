#include "Juego.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

Juego::Juego() : partidaIniciada(false), dadosLanzados(false), opcionCompra(false), ultimoDado1(0), ultimoDado2(0), fase(DebeTirar) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    indice.cargarDesdeTablero(tablero);
}

void Juego::inicializarJugadores() {
    if (partidaIniciada) {
        return;
    }
    int n = 0;
    std::cout << "Ingrese la cantidad de jugadores (2-4): ";
    std::cin >> n;
    if (!std::cin) return;
    std::cin.ignore();

    if (n < 2) n = 2;
    if (n > 4) n = 4;

    for (int i = 0; i < n; ++i) {
        std::string nombre;
        std::cout << "Nombre del jugador " << i + 1 << ": ";
        std::getline(std::cin, nombre);
        estado.agregarJugador(nombre);
        banco.crearCuenta(nombre);
    }
    partidaIniciada = true;
    prepararNuevoTurno();
}

void Juego::prepararNuevoTurno() {
    dadosLanzados = false;
    opcionCompra = false;
    fase = DebeTirar;
}

void Juego::guardarEstado() {
    Snapshot snap;
    snap.banco = banco;
    snap.estado = estado;
    snap.turnos = turnos;
    snap.fase = fase;
    snap.dadosLanzados = dadosLanzados;
    snap.opcionCompra = opcionCompra;
    snap.ultimoDado1 = ultimoDado1;
    snap.ultimoDado2 = ultimoDado2;
    historial.apilar(snap);
}

void Juego::restaurarEstado() {
    Snapshot snap;
    if (historial.desapilar(snap)) {
        banco = snap.banco;
        estado = snap.estado;
        turnos = snap.turnos;
        fase = snap.fase;
        dadosLanzados = snap.dadosLanzados;
        opcionCompra = snap.opcionCompra;
        ultimoDado1 = snap.ultimoDado1;
        ultimoDado2 = snap.ultimoDado2;
    }
}

void Juego::avanzarJugador(modelo::Jugador& jugador, int pasos) {
    int posicionAnterior = jugador.posicion();
    int limite = tablero.totalCasillas();
    jugador.mover(pasos, limite);
    if (jugador.posicion() < posicionAnterior) {
        banco.recibirDelBanco(jugador.nombre(), 200);
        jugador.cobrar(200);
        std::cout << "Cobras $200 por pasar por GO.\n";
    }
}

void Juego::evaluarPropiedad(modelo::Propiedad* propiedad, modelo::Jugador& jugador, int tirada) {
    opcionCompra = false;
    if (!propiedad) {
        return;
    }
    if (propiedad->disponible()) {
        opcionCompra = true;
        std::cout << "Propiedad sin dueño: " << propiedad->nombre() << " cuesta $" << propiedad->datos().precio() << "\n";
    } else if (propiedad->dueno() != &jugador && !propiedad->estaHipotecada()) {
        int renta = propiedad->rentaBase(tirada);
        bool pago = banco.transferir(jugador.nombre(), propiedad->dueno()->nombre(), renta);
        if (pago) {
            jugador.pagar(renta);
            propiedad->dueno()->cobrar(renta);
            std::cout << "Pagas renta de $" << renta << " a " << propiedad->dueno()->nombre() << "\n";
        } else {
            std::cout << "No tienes dinero suficiente para la renta.\n";
        }
    }
}

void Juego::lanzarDados() {
    if (!puedeTirar()) {
        std::cout << "No puedes tirar ahora.\n";
        return;
    }
    guardarEstado();
    ultimoDado1 = dado1.lanzar();
    ultimoDado2 = dado2.lanzar();
    int avance = ultimoDado1 + ultimoDado2;
    modelo::Jugador& actual = estado.getJugadorActual();

    if (actual.enCarcel()) {
        if (ultimoDado1 == ultimoDado2) {
            actual.liberarDeCarcel();
            std::cout << "Dobles! sales de la cárcel.\n";
        } else {
            actual.incrementarTurnoCarcel();
            std::cout << "Sigues en la cárcel.\n";
            fase = PostTirada;
            dadosLanzados = true;
            return;
        }
    }

    avanzarJugador(actual, avance);
    modelo::Casilla* casilla = tablero.obtener(actual.posicion());
    modelo::Propiedad* propiedad = tablero.obtenerPropiedad(casilla);
    evaluarPropiedad(propiedad, actual, avance);
    reglas.evaluarCasilla(actual, banco, estado);
    dadosLanzados = true;
    fase = PostTirada;
}

void Juego::comprarPropiedadActual() {
    if (!puedeComprar()) {
        std::cout << "No es posible comprar en este momento.\n";
        return;
    }
    modelo::Jugador& jugador = estado.getJugadorActual();
    modelo::Casilla* casilla = tablero.obtener(jugador.posicion());
    modelo::Propiedad* propiedad = tablero.obtenerPropiedad(casilla);
    if (!propiedad) return;
    int costo = propiedad->datos().precio();
    if (!banco.pagarBanco(jugador.nombre(), costo)) {
        std::cout << "Saldo insuficiente.\n";
        return;
    }
    if (!jugador.pagar(costo)) {
        return;
    }
    propiedad->asignarDueno(&jugador);
    jugador.agregarPropiedad(propiedad);
    opcionCompra = false;
    std::cout << "Compraste " << propiedad->nombre() << " por $" << costo << "\n";
}

void Juego::intentarConstruir() {
    std::cout << "Función de construcción simplificada aún no disponible.\n";
}

void Juego::intentarHipotecar() {
    std::cout << "Función de hipotecar simplificada.\n";
}

void Juego::intentarDeshipotecar() {
    std::cout << "Función de deshipotecar simplificada.\n";
}

void Juego::usarCartaCarcel() {
    modelo::Jugador& jugador = estado.getJugadorActual();
    if (jugador.usarCartaSalirCarcel()) {
        std::cout << "Sales de la cárcel usando carta.\n";
        fase = DebeTirar;
    } else {
        std::cout << "No tienes carta.\n";
    }
}

void Juego::pagarMultaCarcel() {
    modelo::Jugador& jugador = estado.getJugadorActual();
    if (!jugador.enCarcel()) {
        std::cout << "No estás en la cárcel.\n";
        return;
    }
    int multa = 50;
    if (banco.pagarBanco(jugador.nombre(), multa) && jugador.pagar(multa)) {
        jugador.liberarDeCarcel();
        std::cout << "Pagas $" << multa << " y sales.\n";
        fase = DebeTirar;
    } else {
        std::cout << "No tienes suficiente dinero.\n";
    }
}

void Juego::pasar() {
    if (!puedePasar()) {
        std::cout << "Debes tirar antes de pasar.\n";
        return;
    }
    estado.siguienteTurno();
    turnos.siguiente();
    prepararNuevoTurno();
}

void Juego::mostrarEstado() const {
    std::cout << "\n=== ESTADO DEL JUEGO ===\n";
    std::vector<modelo::Jugador> jugadores = estado.getJugadores();
    for (std::size_t i = 0; i < jugadores.size(); ++i) {
        const modelo::Jugador& j = jugadores[i];
        std::cout << j.nombre() << " en " << j.posicion()
                  << " | Dinero: $" << banco.getSaldo(j.nombre())
                  << " | Cárcel: " << (j.enCarcel() ? "Sí" : "No") << "\n";
    }
    std::cout << "Turno #" << turnos.getTurno() << "\n";
}

bool Juego::haTerminado() const {
    int activos = 0;
    std::vector<modelo::Jugador> jugadores = estado.getJugadores();
    for (std::size_t i = 0; i < jugadores.size(); ++i) {
        if (banco.getSaldo(jugadores[i].nombre()) > 0) {
            activos++;
        }
    }
    return activos <= 1;
}

bool Juego::puedeComprar() const {
    if (fase != PostTirada) return false;
    if (!opcionCompra) return false;
    const modelo::Jugador& jugador = estado.getJugadorActualConst();
    modelo::Casilla* casilla = tablero.obtener(jugador.posicion());
    modelo::Propiedad* propiedad = tablero.obtenerPropiedad(casilla);
    if (!propiedad) return false;
    return banco.getSaldo(jugador.nombre()) >= propiedad->datos().precio();
}

bool Juego::puedeConstruir() const {
    return false;
}

bool Juego::puedeHipotecar() const {
    return false;
}

bool Juego::puedeDeshipotecar() const {
    return false;
}

bool Juego::puedePagarMulta() const {
    const modelo::Jugador& jugador = estado.getJugadorActualConst();
    if (!jugador.enCarcel()) return false;
    return banco.getSaldo(jugador.nombre()) >= 50;
}

bool Juego::tieneCartaSalir() const {
    return estado.getJugadorActualConst().cartasSalirCarcel() > 0;
}

bool Juego::hayUndo() const {
    return !historial.vacia();
}

bool Juego::puedeTirar() const {
    return fase == DebeTirar;
}

bool Juego::puedePasar() const {
    return fase == PostTirada;
}

modelo::Jugador& Juego::jugadorActual() {
    return estado.getJugadorActual();
}

void Juego::undo() {
    restaurarEstado();
}

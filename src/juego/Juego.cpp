#include "Juego.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

Juego::Juego() : partidaIniciada(false), opcionCompra(false), ultimoDado1(0), ultimoDado2(0), fase(DebeTirar) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    reglamento.cargarConfig("data/config.txt");
    banco.fijarStock(reglamento.stockCasasInicial(), reglamento.stockHotelesInicial());
    cargarMazo("data/cartas_casualidad.txt", mazoCasualidad, false);
    cargarMazo("data/cartas_arca.txt", mazoArca, true);
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

void Juego::lanzarDados() {
    if (!puedeTirar()) {
        std::cout << "No puedes tirar ahora.\n";
        return;
    }
    guardarEstado();
    ultimoDado1 = dado1.lanzar();
    ultimoDado2 = dado2.lanzar();
    int avance = ultimoDado1 + ultimoDado2;
    std::cout << "Dados: " << ultimoDado1 << " y " << ultimoDado2 << " (" << avance << ")\n";
    modelo::Jugador& actual = estado.getJugadorActual();

    if (actual.enCarcel()) {
        if (ultimoDado1 == ultimoDado2) {
            actual.liberarDeCarcel();
            std::cout << "Dobles! sales de la cárcel.\n";
        } else {
            actual.incrementarTurnoCarcel();
            std::cout << "Sigues en la cárcel.\n";
            fase = PostTirada;
            return;
        }
    }

    avanzarJugador(actual, avance);
    tablero.describirCasilla(actual.posicion());
    modelo::Casilla* casilla = tablero.obtener(actual.posicion());
    if (casilla) {
        casilla->alCaer(*this, actual, avance);
    }
    modelo::Propiedad* propiedad = tablero.obtenerPropiedad(casilla);
    evaluarPropiedad(propiedad, actual, avance);
    fase = PostTirada;
}

void Juego::comprarPropiedadActual() {
    if (!puedeComprar()) {
        std::cout << "No es posible comprar en este momento.\n";
        return;
    }
    guardarEstado();
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
    if (!puedeConstruir()) {
        std::cout << "No puedes construir ahora.\n";
        return;
    }
    modelo::Jugador& jugador = estado.getJugadorActual();
    std::vector<modelo::Solar*> opciones = solaresConstruibles(jugador);
    if (opciones.empty()) {
        std::cout << "No hay solares válidos.\n";
        return;
    }
    std::cout << "¿Dónde quieres construir?\n";
    for (std::size_t i = 0; i < opciones.size(); ++i) {
        std::cout << i + 1 << ". " << opciones[i]->nombre() << " (casas=" << opciones[i]->casas() << (opciones[i]->hotel() ? ",hotel" : "") << ")\n";
    }
    int eleccion = 0;
    std::cin >> eleccion;
    if (!std::cin || eleccion <= 0 || static_cast<std::size_t>(eleccion) > opciones.size()) return;
    modelo::Propiedad* encontrada = buscarPropiedad(opciones[static_cast<std::size_t>(eleccion - 1)]->nombre());
    modelo::Solar* solar = dynamic_cast<modelo::Solar*>(encontrada);
    if (!solar) { std::cout << "No se encontró el solar en el índice.\n"; return; }
    int pasos = 1;
    std::cout << "¿Cuántos pasos de edificación? (1.." << (solar->casas() == 4 ? 1 : 4 - solar->casas()) << ")\n";
    std::cin >> pasos;
    if (!std::cin) return;
    if (pasos < 1) pasos = 1;
    guardarEstado();
    Construccion reglaConstruccion;
    for (int p = 0; p < pasos; ++p) {
        if (!cumpleSimetria(solar, jugador)) {
            std::cout << "Debes construir de manera equilibrada en el grupo.\n";
            break;
        }
        if (solar->hotel()) break;
        if (solar->casas() < 4) {
            if (!banco.tomarCasa()) { std::cout << "Sin stock de casas.\n"; break; }
            if (!jugador.pagar(solar->datos().costoCasa()) || !banco.pagarBanco(jugador.nombre(), solar->datos().costoCasa())) { banco.devolverCasa(); break; }
            solar->construirCasa();
            std::cout << "Construyes casa en " << solar->nombre() << "\n";
        } else {
            if (!banco.tomarHotel()) { std::cout << "Sin hoteles disponibles.\n"; break; }
            if (!jugador.pagar(solar->datos().costoHotel()) || !banco.pagarBanco(jugador.nombre(), solar->datos().costoHotel())) { banco.devolverHotel(); break; }
            solar->construirHotel();
            for (int i = 0; i < 4; ++i) banco.devolverCasa();
            std::cout << "Construyes hotel en " << solar->nombre() << "\n";
        }
    }
}

void Juego::intentarHipotecar() {
    if (!puedeHipotecar()) {
        std::cout << "No puedes hipotecar ahora.\n";
        return;
    }
    modelo::Jugador& jugador = estado.getJugadorActual();
    std::vector<modelo::Propiedad*> propias = jugador.propiedades();
    std::vector<modelo::Propiedad*> candidatas;
    for (std::size_t i = 0; i < propias.size(); ++i) {
        modelo::Solar* s = dynamic_cast<modelo::Solar*>(propias[i]);
        if (s && (s->casas() > 0 || s->hotel())) continue;
        if (!propias[i]->estaHipotecada()) candidatas.push_back(propias[i]);
    }
    if (candidatas.empty()) { std::cout << "No hay propiedades hipotecables.\n"; return; }
    std::cout << "Elige propiedad a hipotecar:\n";
    for (std::size_t i = 0; i < candidatas.size(); ++i) {
        std::cout << i + 1 << ". " << candidatas[i]->nombre() << "\n";
    }
    int opt = 0; std::cin >> opt; if (!std::cin) return;
    if (opt <= 0 || static_cast<std::size_t>(opt) > candidatas.size()) return;
    modelo::Propiedad* elegida = buscarPropiedad(candidatas[static_cast<std::size_t>(opt - 1)]->nombre());
    if (!elegida) { std::cout << "La propiedad no está en el índice.\n"; return; }
    guardarEstado();
    Hipoteca regla;
    regla.hipotecar(elegida, &jugador);
}

void Juego::intentarDeshipotecar() {
    if (!puedeDeshipotecar()) {
        std::cout << "No hay hipotecas para levantar.\n";
        return;
    }
    modelo::Jugador& jugador = estado.getJugadorActual();
    std::vector<modelo::Propiedad*> props = jugador.propiedades();
    std::vector<modelo::Propiedad*> hip;
    for (std::size_t i = 0; i < props.size(); ++i) {
        if (props[i]->estaHipotecada()) hip.push_back(props[i]);
    }
    if (hip.empty()) return;
    std::cout << "¿Qué hipoteca quieres cancelar?\n";
    for (std::size_t i = 0; i < hip.size(); ++i) {
        std::cout << i + 1 << ". " << hip[i]->nombre() << "\n";
    }
    int opt = 0; std::cin >> opt; if (!std::cin) return;
    if (opt <= 0 || static_cast<std::size_t>(opt) > hip.size()) return;
    modelo::Propiedad* elegida = buscarPropiedad(hip[static_cast<std::size_t>(opt - 1)]->nombre());
    if (!elegida) { std::cout << "No se encontró esa hipoteca en el índice.\n"; return; }
    guardarEstado();
    int deuda = elegida->datos().precio() / 2;
    int interes = (deuda * reglamento.interesHipoteca()) / 100;
    int total = deuda + interes;
    if (!banco.pagarBanco(jugador.nombre(), total) || !jugador.pagar(total)) {
        std::cout << "Saldo insuficiente.\n";
        return;
    }
    Hipoteca regla;
    regla.cancelarHipoteca(elegida, &jugador);
}

void Juego::usarCartaCarcel() {
    modelo::Jugador& jugador = estado.getJugadorActual();
    if (jugador.cartasSalirCarcel() > 0) {
        guardarEstado();
        jugador.usarCartaSalirCarcel();
        std::cout << "Sales de la cárcel usando carta.\n";
        fase = DebeTirar;
    } else {
        std::cout << "No tienes carta.\n";
    }
}

void Juego::pagarMultaCarcel() {
    modelo::Jugador& jugador = estado.getJugadorActual();
    if (!jugador.enCarcel()) return;
    int multa = reglamento.multaCarcel();
    guardarEstado();
    if (!banco.pagarBanco(jugador.nombre(), multa) || !jugador.pagar(multa)) {
        std::cout << "No tienes suficiente dinero.\n";
        return;
    }
    jugador.liberarDeCarcel();
    std::cout << "Pagas $" << multa << " y sales.\n";
    fase = DebeTirar;
}

void Juego::pasar() {
    if (!puedePasar()) {
        std::cout << "Debes tirar antes de pasar.\n";
        return;
    }
    guardarEstado();
    estado.siguienteTurno();
    turnos.siguiente();
    prepararNuevoTurno();
}

void Juego::mostrarEstado() const {
    std::cout << "\n=== ESTADO DEL JUEGO ===\n";
    std::vector<modelo::Jugador> jugadores = estado.getJugadores();
    for (std::size_t i = 0; i < jugadores.size(); ++i) {
        const modelo::Jugador& j = jugadores[i];
        std::cout << j.nombre() << " en pos " << j.posicion()
                  << " | Dinero: $" << banco.getSaldo(j.nombre())
                  << " | Cárcel: " << (j.enCarcel() ? "Sí" : "No") << "\n";
    }
    std::cout << "Stock banco -> Casas: " << banco.stockCasas() << " Hoteles: " << banco.stockHoteles() << "\n";
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
    if (fase != PostTirada) return false;
    const modelo::Jugador& jugador = estado.getJugadorActualConst();
    std::vector<modelo::Solar*> opciones = solaresConstruibles(jugador);
    if (opciones.empty()) return false;
    if (banco.stockCasas() <= 0 && banco.stockHoteles() <= 0) return false;
    return true;
}

bool Juego::puedeHipotecar() const {
    const modelo::Jugador& jugador = estado.getJugadorActualConst();
    const std::vector<modelo::Propiedad*>& props = jugador.propiedades();
    for (std::size_t i = 0; i < props.size(); ++i) {
        if (!props[i]->estaHipotecada()) {
            const modelo::Solar* s = dynamic_cast<const modelo::Solar*>(props[i]);
            if (s && (s->casas() > 0 || s->hotel())) continue;
            return true;
        }
    }
    return false;
}

bool Juego::puedeDeshipotecar() const {
    const modelo::Jugador& jugador = estado.getJugadorActualConst();
    const std::vector<modelo::Propiedad*>& props = jugador.propiedades();
    for (std::size_t i = 0; i < props.size(); ++i) {
        if (props[i]->estaHipotecada()) {
            int deuda = props[i]->datos().precio() / 2;
            int interes = (deuda * reglamento.interesHipoteca()) / 100;
            if (banco.getSaldo(jugador.nombre()) >= deuda + interes) {
                return true;
            }
        }
    }
    return false;
}

bool Juego::puedePagarMulta() const {
    const modelo::Jugador& jugador = estado.getJugadorActualConst();
    if (!jugador.enCarcel()) return false;
    return banco.getSaldo(jugador.nombre()) >= reglamento.multaCarcel();
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

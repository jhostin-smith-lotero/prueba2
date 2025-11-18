#include "Juego.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cstdio>

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

void Juego::prepararNuevoTurno() {
    opcionCompra = false;
    fase = DebeTirar;
}

void Juego::guardarEstado() {
    Snapshot snap;
    snap.banco = banco;
    snap.estado = estado;
    snap.turnos = turnos;
    snap.fase = fase;
    snap.opcionCompra = opcionCompra;
    snap.ultimoDado1 = ultimoDado1;
    snap.ultimoDado2 = ultimoDado2;
    snap.propiedades = tablero.capturarEstado(estado.jugadoresRef());
    snap.mazoCasualidad = mazoCasualidad;
    snap.mazoArca = mazoArca;
    historial.apilar(snap);
}

void Juego::restaurarEstado() {
    Snapshot snap;
    if (historial.desapilar(snap)) {
        banco = snap.banco;
        estado = snap.estado;
        turnos = snap.turnos;
        fase = snap.fase;
        opcionCompra = snap.opcionCompra;
        ultimoDado1 = snap.ultimoDado1;
        ultimoDado2 = snap.ultimoDado2;
        tablero.restaurarEstado(snap.propiedades, estado.jugadoresRefMutable());
        mazoCasualidad = snap.mazoCasualidad;
        mazoArca = snap.mazoArca;
    }
}

void Juego::avanzarJugador(modelo::Jugador& jugador, int pasos) {
    int posicionAnterior = jugador.posicion();
    int limite = tablero.totalCasillas();
    if (limite <= 0) {
        std::cout << "El tablero no tiene casillas cargadas.\n";
        return;
    }
    jugador.mover(pasos, limite);
    if (jugador.posicion() < posicionAnterior) {
        banco.recibirDelBanco(jugador.nombre(), reglamento.salarioGo());
        jugador.cobrar(reglamento.salarioGo());
        std::cout << "Cobras $" << reglamento.salarioGo() << " por pasar por GO.\n";
    }
}

void Juego::resolverEspecial(modelo::Especial* especial, modelo::Jugador& jugador, int tirada) {
    if (!especial) return;
    modelo::Especial::TipoEspecial tipo = especial->tipoEspecial();
    if (tipo == modelo::Especial::GO) {
        std::cout << "Descansas en SALIDA y cobras $" << reglamento.salarioGo() << "\n";
        banco.recibirDelBanco(jugador.nombre(), reglamento.salarioGo());
        jugador.cobrar(reglamento.salarioGo());
    } else if (tipo == modelo::Especial::IMPUESTO) {
        int monto = especial->montoImpuesto();
        if (banco.pagarBanco(jugador.nombre(), monto) && jugador.pagar(monto)) {
            std::cout << "Pagas impuesto de $" << monto << "\n";
        } else {
            std::cout << "No tienes saldo para pagar impuesto de $" << monto << "\n";
        }
    } else if (tipo == modelo::Especial::IR_CARCEL) {
        std::cout << "Vas directo a la cárcel.\n";
        jugador.encarcelar(posicionCarcel());
    } else if (tipo == modelo::Especial::CARCEL) {
        std::cout << "Estás de visita en la cárcel.\n";
    } else if (tipo == modelo::Especial::PARQUE) {
        std::cout << "Parqueo libre, nada ocurre.\n";
    } else if (tipo == modelo::Especial::CASUALIDAD || tipo == modelo::Especial::ARCA) {
        modelo::Carta carta;
        bool tomada = false;
        if (tipo == modelo::Especial::CASUALIDAD) {
            tomada = mazoCasualidad.robar(carta);
        } else {
            tomada = mazoArca.robar(carta);
        }
        if (!tomada) {
            std::cout << "El mazo está vacío.\n";
            return;
        }
        std::cout << "Carta: " << carta.texto() << "\n";
        aplicarCarta(carta, jugador);
        if (!carta.conservar()) {
            if (tipo == modelo::Especial::CASUALIDAD) {
                mazoCasualidad.ponerAlFondo(carta);
            } else {
                mazoArca.ponerAlFondo(carta);
            }
        }
    }
    (void)tirada;
}

void Juego::aplicarCarta(const modelo::Carta& carta, modelo::Jugador& jugador) {
    if (carta.accion() == modelo::Carta::Accion::COBRAR) {
        banco.recibirDelBanco(jugador.nombre(), carta.valor());
        jugador.cobrar(carta.valor());
    } else if (carta.accion() == modelo::Carta::Accion::PAGAR) {
        if (!banco.pagarBanco(jugador.nombre(), carta.valor()) || !jugador.pagar(carta.valor())) {
            std::cout << "No puedes pagar $" << carta.valor() << "\n";
        }
    } else if (carta.accion() == modelo::Carta::Accion::MOVER) {
        moverDirecto(jugador, carta.destino(), true);
        tablero.describirCasilla(jugador.posicion());
        modelo::Casilla* c = tablero.obtener(jugador.posicion());
        if (c) { c->alCaer(*this, jugador, 0); }
        modelo::Propiedad* p = tablero.obtenerPropiedad(c);
        evaluarPropiedad(p, jugador, 0);
    } else if (carta.accion() == modelo::Carta::Accion::MOVER_RELATIVO) {
        avanzarJugador(jugador, carta.valor());
        tablero.describirCasilla(jugador.posicion());
        modelo::Casilla* c = tablero.obtener(jugador.posicion());
        if (c) { c->alCaer(*this, jugador, carta.valor()); }
        modelo::Propiedad* p = tablero.obtenerPropiedad(c);
        evaluarPropiedad(p, jugador, carta.valor());
    } else if (carta.accion() == modelo::Carta::Accion::SALIR_CARCEL) {
        jugador.recibirCartaSalirCarcel();
    } else if (carta.accion() == modelo::Carta::Accion::IR_CARCEL) {
        jugador.encarcelar(posicionCarcel());
    } else if (carta.accion() == modelo::Carta::Accion::REPARAR) {
        int costo = 0;
        const std::vector<modelo::Propiedad*>& props = jugador.propiedades();
        for (std::size_t i = 0; i < props.size(); ++i) {
            const modelo::Solar* s = dynamic_cast<const modelo::Solar*>(props[i]);
            if (!s) continue;
            costo += s->casas() * carta.valor();
            if (s->hotel()) costo += carta.destino();
        }
        if (costo > 0) {
            if (!banco.pagarBanco(jugador.nombre(), costo) || !jugador.pagar(costo)) {
                std::cout << "No puedes pagar reparaciones por $" << costo << "\n";
            }
        }
    }
}

void Juego::cargarMazo(const std::string& ruta, modelo::Mazo& mazo, bool esArca) {
    FILE* f = std::fopen(ruta.c_str(), "r");
    if (!f) {
        std::cout << "No se pudo abrir " << ruta << ", se usarán cartas básicas.\n";
        if (esArca) {
            mazo.agregar(modelo::Carta("Cobra $50", modelo::Carta::Accion::COBRAR, 50));
        } else {
            mazo.agregar(modelo::Carta("Paga $50", modelo::Carta::Accion::PAGAR, 50));
        }
        return;
    }
    char buffer[512];
    std::string texto;
    std::string tipo;
    int monto = 0;
    int destino = -1;
    int porCasa = 0;
    int porHotel = 0;
    while (std::fgets(buffer, sizeof(buffer), f)) {
        std::string linea(buffer);
        if (linea.find("\"descripcion\"") != std::string::npos) {
            std::size_t c1 = linea.find("\"");
            c1 = linea.find("\"", c1 + 1);
            std::size_t c2 = linea.find("\"", c1 + 1);
            std::size_t c3 = linea.find("\"", c2 + 1);
            if (c2 != std::string::npos && c3 != std::string::npos) {
                texto = linea.substr(c2 + 1, c3 - c2 - 1);
            }
        }
        if (linea.find("\"tipo\"") != std::string::npos) {
            std::size_t c1 = linea.find("\"", linea.find(":"));
            std::size_t c2 = linea.find("\"", c1 + 1);
            if (c1 != std::string::npos && c2 != std::string::npos) {
                tipo = linea.substr(c1 + 1, c2 - c1 - 1);
            }
        }
        if (linea.find("\"monto\"") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            monto = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { monto = monto * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("\"destino\"") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            destino = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { destino = destino * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("porCasa") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            porCasa = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { porCasa = porCasa * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("porHotel") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            porHotel = 0;
            while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) pos++;
            while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) { porHotel = porHotel * 10 + (linea[pos]-'0'); pos++; }
        }
        if (linea.find("}") != std::string::npos) {
            if (tipo == "cobrar") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::COBRAR, monto));
            } else if (tipo == "pagar") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::PAGAR, monto));
            } else if (tipo == "mover_a") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::MOVER, monto, destino));
            } else if (tipo == "mover_rel") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::MOVER_RELATIVO, monto));
            } else if (tipo == "salir_carcel") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::SALIR_CARCEL, 0, 0, true));
            } else if (tipo == "ir_carcel") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::IR_CARCEL, 0));
            } else if (tipo == "reparar") {
                mazo.agregar(modelo::Carta(texto, modelo::Carta::Accion::REPARAR, porCasa, porHotel));
            }
            texto.clear();
            tipo.clear();
            monto = 0;
            destino = -1;
            porCasa = 0;
            porHotel = 0;
        }
    }
    std::fclose(f);
}

void Juego::moverDirecto(modelo::Jugador& jugador, int destino, bool cobraGo) {
    int limite = tablero.totalCasillas();
    if (limite <= 0) return;
    int anterior = jugador.posicion();
    jugador.moverDirecto(destino, limite);
    if (cobraGo && destino < anterior) {
        banco.recibirDelBanco(jugador.nombre(), reglamento.salarioGo());
        jugador.cobrar(reglamento.salarioGo());
        std::cout << "Cobras $" << reglamento.salarioGo() << " por pasar por GO.\n";
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
        if (pago && jugador.pagar(renta)) {
            propiedad->dueno()->cobrar(renta);
            std::cout << "Pagas renta de $" << renta << " a " << propiedad->dueno()->nombre() << "\n";
        } else {
            std::cout << "No tienes dinero suficiente para la renta.\n";
        }
    }
}

bool Juego::grupoCompleto(const std::string& color, const modelo::Jugador& jugador) const {
    std::vector<modelo::Solar*> grupo = tablero.solaresPorColor(color);
    if (grupo.empty()) return false;
    for (std::size_t i = 0; i < grupo.size(); ++i) {
        if (grupo[i]->dueno() != &jugador) return false;
        if (grupo[i]->estaHipotecada()) return false;
    }
    return true;
}

int Juego::valorEdificacion(const modelo::Solar* solar) const {
    if (!solar) return 0;
    if (solar->hotel()) return 5;
    return solar->casas();
}

bool Juego::cumpleSimetria(const modelo::Solar* solar, const modelo::Jugador& jugador) const {
    if (!solar) return false;
    std::vector<modelo::Solar*> grupo = tablero.solaresPorColor(solar->color());
    if (grupo.empty()) return false;
    int minimo = 10;
    for (std::size_t i = 0; i < grupo.size(); ++i) {
        int valor = valorEdificacion(grupo[i]);
        if (valor < minimo) {
            minimo = valor;
        }
        if (grupo[i]->dueno() != &jugador) return false;
    }
    return valorEdificacion(solar) == minimo;
}

std::vector<modelo::Solar*> Juego::solaresConstruibles(const modelo::Jugador& jugador) const {
    std::vector<modelo::Solar*> lista;
    const std::vector<modelo::Propiedad*>& props = jugador.propiedades();
    for (std::size_t i = 0; i < props.size(); ++i) {
        modelo::Solar* solar = dynamic_cast<modelo::Solar*>(props[i]);
        if (!solar) continue;
        if (!grupoCompleto(solar->color(), jugador)) continue;
        if (solar->hotel()) continue;
        if (!cumpleSimetria(solar, jugador)) continue;
        lista.push_back(solar);
    }
    return lista;
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
        std::cout << i + 1 << ". " << opciones[i]->nombre() << " (casas=" << opciones[i]->casas() << (opciones[i]->hotel() ? ", hotel" : "") << ")\n";
    }
    int eleccion = 0;
    std::cin >> eleccion;
    if (!std::cin || eleccion <= 0 || static_cast<std::size_t>(eleccion) > opciones.size()) return;
    modelo::Solar* solar = opciones[static_cast<std::size_t>(eleccion - 1)];
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
    guardarEstado();
    Hipoteca regla;
    regla.hipotecar(candidatas[static_cast<std::size_t>(opt - 1)], &jugador);
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
    guardarEstado();
    int deuda = hip[static_cast<std::size_t>(opt - 1)]->datos().precio() / 2;
    int interes = (deuda * reglamento.interesHipoteca()) / 100;
    int total = deuda + interes;
    if (!banco.pagarBanco(jugador.nombre(), total) || !jugador.pagar(total)) {
        std::cout << "Saldo insuficiente.\n";
        return;
    }
    Hipoteca regla;
    regla.cancelarHipoteca(hip[static_cast<std::size_t>(opt - 1)], &jugador);
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
    if (!jugador.enCarcel()) {
        std::cout << "No estás en la cárcel.\n";
        return;
    }
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


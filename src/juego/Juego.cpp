#include "Juego.h"

#include "../modelo/Carta.h"
#include "../modelo/Especial.h"
#include "../modelo/Ferrocarril.h"
#include "../modelo/Jugador.h"
#include "../modelo/Mazo.h"
#include "../modelo/Propiedad.h"
#include "../modelo/Servicio.h"
#include "../modelo/Solar.h"
#include "../modelo/Tablero.h"
#include "../reglas/CalculoRenta.h"
#include "../reglas/Construccion.h"
#include "../reglas/Hipoteca.h"
#include "../util/Json.h"
#include "../util/StringUtils.h"

#include <array>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace juego {

namespace {
std::string formarRuta(const std::string& base, const std::string& archivo) {
    if (base.empty()) {
        return archivo;
    }
    if (base.back() == '/') {
        return base + archivo;
    }
    return base + "/" + archivo;
}

modelo::Especial::Categoria categoriaDesdeTexto(const std::string& texto) {
    std::string upper = util::toUpper(texto);
    if (upper == "GO" || upper == "SALIDA") return modelo::Especial::Categoria::SALIDA;
    if (upper == "ARCA" || upper == "ARCA COMUNAL") return modelo::Especial::Categoria::ARCA;
    if (upper == "CASUALIDAD") return modelo::Especial::Categoria::CASUALIDAD;
    if (upper == "IMPUESTO") return modelo::Especial::Categoria::IMPUESTO;
    if (upper == "IMPUESTO_LUJO" || upper == "IMPUESTO DE LUJO") return modelo::Especial::Categoria::IMPUESTO_LUJO;
    if (upper == "CARCEL" || upper == "CÁRCEL") return modelo::Especial::Categoria::CARCEL;
    if (upper == "IR_CARCEL" || upper == "IR A LA CÁRCEL") return modelo::Especial::Categoria::IR_CARCEL;
    if (upper == "ESTACIONAMIENTO" || upper == "PARQUE LIBRE") return modelo::Especial::Categoria::ESTACIONAMIENTO;
    return modelo::Especial::Categoria::NADA;
}

modelo::Carta::Accion accionDesdeTexto(const std::string& texto) {
    std::string upper = util::toUpper(texto);
    if (upper == "COBRAR") return modelo::Carta::Accion::COBRAR;
    if (upper == "PAGAR") return modelo::Carta::Accion::PAGAR;
    if (upper == "MOVER") return modelo::Carta::Accion::MOVER;
    if (upper == "MOVER_RELATIVO") return modelo::Carta::Accion::MOVER_RELATIVO;
    if (upper == "SALIR_CARCEL") return modelo::Carta::Accion::SALIR_CARCEL;
    if (upper == "IR_CARCEL") return modelo::Carta::Accion::IR_CARCEL;
    if (upper == "REPARAR") return modelo::Carta::Accion::REPARAR;
    return modelo::Carta::Accion::COBRAR;
}
}

Juego::Juego() = default;

void Juego::cargarDatos(const std::string& directorioDatos) {
    directorioDatos_ = directorioDatos;
    reglas_.cargar(formarRuta(directorioDatos_, "config.json"));
    cargarTablero(formarRuta(directorioDatos_, "tablero.json"));
    cargarCartas(formarRuta(directorioDatos_, "cartas_casualidad.json"), mazoCasualidad_);
    cargarCartas(formarRuta(directorioDatos_, "cartas_arca.json"), mazoArca_);
    indice_.cargarDesdeTablero(tablero_);
}

bool Juego::iniciarPartida(const std::vector<std::string>& nombresJugadores) {
    if (nombresJugadores.size() < 2) {
        return false;
    }
    jugadores_.clear();
    for (const auto& nombre : nombresJugadores) {
        modelo::Jugador jugador(nombre);
        jugador.setDinero(reglas_.dineroInicialJugadores());
        jugador.moverDirecto(0, static_cast<int>(tablero_.tamanio()));
        jugadores_.push_back(jugador);
    }
    banco_.inicializar(reglas_.dineroBanco(), reglas_.casasBanco(), reglas_.hotelesBanco());
    turnos_.iniciar(jugadores_.size());
    historial_.limpiar();
    partidaActiva_ = true;
    return true;
}

const modelo::Jugador& Juego::jugadorActual() const {
    return jugadores_.at(turnos_.actual());
}

modelo::Jugador& Juego::jugadorActual() {
    return jugadores_.at(turnos_.actual());
}

Turnos::Fase Juego::faseActual() const {
    return turnos_.fase();
}

bool Juego::puedeDeshacer() const {
    return !historial_.vacia();
}

std::string Juego::lanzarDados() {
    if (!partidaActiva_) {
        return "No hay partida en curso";
    }
    guardarEstado();
    auto& jugador = jugadorActual();
    auto tirada = dado_.lanzar();
    int suma = tirada.first + tirada.second;
    bool doble = tirada.first == tirada.second;
    std::ostringstream mensaje;
    mensaje << jugador.nombre() << " lanzó " << tirada.first << "+" << tirada.second << " = " << suma;

    if (jugador.enCarcel()) {
        if (doble) {
            jugador.liberarDeCarcel();
            mensaje << ". Sale de la cárcel y se mueve.";
        } else {
            jugador.incrementarTurnoCarcel();
            mensaje << ". No salió de la cárcel.";
            turnos_.pasarTurno();
            return mensaje.str();
        }
    }

    moverJugador(jugador, suma, suma, doble);
    turnos_.registrarLanzamiento(doble);
    if (doble && turnos_.doblesConsecutivos() >= 3) {
        mensaje << " Sacó tres dobles seguidos. Va a la cárcel.";
        enviarJugadorCarcel(jugador);
        turnos_.pasarTurno();
        return mensaje.str();
    }
    procesarCasilla(jugador, suma);
    if (!doble || jugador.enCarcel()) {
        turnos_.setFase(Turnos::Fase::Acciones);
    } else {
        mensaje << " Puede volver a lanzar.";
    }
    return mensaje.str();
}

std::string Juego::comprarPropiedadActual() {
    if (!partidaActiva_) {
        return "No hay partida activa";
    }
    auto* propiedad = propiedadEnPosicion(jugadorActual().posicion());
    if (!propiedad) {
        return "La casilla actual no es una propiedad";
    }
    if (propiedad->dueno() != nullptr) {
        return "La propiedad ya tiene dueño";
    }
    int precio = propiedad->escritura().precio();
    auto& jugador = jugadorActual();
    if (jugador.dinero() < precio) {
        return "Dinero insuficiente";
    }
    guardarEstado();
    jugador.pagar(precio);
    banco_.ajustarEfectivo(precio);
    asignarPropiedad(*propiedad, jugador);
    std::ostringstream mensaje;
    mensaje << jugador.nombre() << " compra " << propiedad->nombre() << " por $" << precio;
    return mensaje.str();
}

std::string Juego::pasarTurno() {
    if (!partidaActiva_) {
        return "No hay partida activa";
    }
    turnos_.pasarTurno();
    return "Turno finalizado";
}

std::string Juego::construirCasa(const std::string& nombrePropiedad) {
    auto* propiedad = buscarPropiedad(nombrePropiedad);
    if (!propiedad) {
        return "Propiedad no encontrada";
    }
    auto* solar = dynamic_cast<modelo::Solar*>(propiedad);
    if (!solar) {
        return "Solo se puede construir en solares";
    }
    if (propiedad->dueno() != &jugadorActual()) {
        return "Solo el dueño puede construir";
    }
    guardarEstado();
    std::string mensaje;
    if (!reglas::Construccion::construirCasa(*solar, jugadorActual(), banco_, mensaje)) {
        Estado anterior;
        historial_.desapilar(anterior);
        return mensaje;
    }
    return "Casa construida";
}

std::string Juego::construirHotel(const std::string& nombrePropiedad) {
    auto* propiedad = buscarPropiedad(nombrePropiedad);
    if (!propiedad) {
        return "Propiedad no encontrada";
    }
    auto* solar = dynamic_cast<modelo::Solar*>(propiedad);
    if (!solar) {
        return "Solo se puede construir en solares";
    }
    if (propiedad->dueno() != &jugadorActual()) {
        return "Solo el dueño puede construir";
    }
    guardarEstado();
    std::string mensaje;
    if (!reglas::Construccion::construirHotel(*solar, jugadorActual(), banco_, mensaje)) {
        Estado anterior;
        historial_.desapilar(anterior);
        return mensaje;
    }
    return "Hotel construido";
}

std::string Juego::hipotecarPropiedad(const std::string& nombrePropiedad) {
    auto* propiedad = buscarPropiedad(nombrePropiedad);
    if (!propiedad) {
        return "Propiedad no encontrada";
    }
    if (propiedad->dueno() != &jugadorActual()) {
        return "Solo el dueño puede hipotecar";
    }
    guardarEstado();
    std::string mensaje;
    if (!reglas::Hipoteca::hipotecar(*propiedad, jugadorActual(), banco_, reglas_.interesHipoteca(), mensaje)) {
        Estado anterior;
        historial_.desapilar(anterior);
        return mensaje;
    }
    return "Propiedad hipotecada";
}

std::string Juego::deshipotecarPropiedad(const std::string& nombrePropiedad) {
    auto* propiedad = buscarPropiedad(nombrePropiedad);
    if (!propiedad) {
        return "Propiedad no encontrada";
    }
    if (propiedad->dueno() != &jugadorActual()) {
        return "Solo el dueño puede deshipotecar";
    }
    guardarEstado();
    std::string mensaje;
    if (!reglas::Hipoteca::deshipotecar(*propiedad, jugadorActual(), banco_, reglas_.interesHipoteca(), mensaje)) {
        Estado anterior;
        historial_.desapilar(anterior);
        return mensaje;
    }
    return "Hipoteca cancelada";
}

std::string Juego::deshacer() {
    if (historial_.vacia()) {
        return "No hay acciones para deshacer";
    }
    Estado estado;
    historial_.desapilar(estado);
    restaurarEstado(estado);
    return "Acción deshecha";
}

std::string Juego::pagarMultaCarcel() {
    if (!partidaActiva_) {
        return "No hay partida activa";
    }
    auto& jugador = jugadorActual();
    if (!jugador.enCarcel()) {
        return "No estás en la cárcel";
    }
    int multa = reglas_.multaCarcel();
    if (jugador.dinero() < multa) {
        return "Dinero insuficiente para pagar la multa";
    }
    guardarEstado();
    if (!banco_.cobrarAJugador(jugador, multa)) {
        Estado estado;
        historial_.desapilar(estado);
        return "No fue posible pagar la multa";
    }
    jugador.liberarDeCarcel();
    turnos_.setFase(Turnos::Fase::DebeLanzar);
    turnos_.reiniciarDobles();
    return "Has pagado la multa y quedas en libertad";
}

std::string Juego::usarCartaSalirCarcel() {
    if (!partidaActiva_) {
        return "No hay partida activa";
    }
    auto& jugador = jugadorActual();
    if (!jugador.enCarcel()) {
        return "No estás en la cárcel";
    }
    if (jugador.cartasSalirCarcel() <= 0) {
        return "No tienes cartas para salir de la cárcel";
    }
    guardarEstado();
    if (!jugador.usarCartaSalirCarcel()) {
        Estado estado;
        historial_.desapilar(estado);
        return "No fue posible usar la carta";
    }
    turnos_.setFase(Turnos::Fase::DebeLanzar);
    turnos_.reiniciarDobles();
    return "Has usado una carta y sales de la cárcel";
}

std::string Juego::resumenEstado() const {
    std::ostringstream oss;
    oss << "Turno de " << jugadorActual().nombre() << "\n";
    for (const auto& jugador : jugadores_) {
        oss << "- " << std::setw(12) << jugador.nombre() << ": $" << jugador.dinero()
            << " | Posición: " << jugador.posicion();
        if (jugador.enCarcel()) {
            oss << " (en cárcel)";
        }
        oss << "\n";
    }
    return oss.str();
}

void Juego::cobrarSalida(modelo::Jugador& jugador) {
    banco_.pagarAJugador(jugador, reglas_.salarioSalida());
}

void Juego::robarCartaArca(modelo::Jugador& jugador) {
    if (mazoArca_.vacio()) {
        return;
    }
    auto carta = mazoArca_.robar();
    aplicarCarta(carta, jugador);
    if (!carta.conservar()) {
        mazoArca_.ponerAlFondo(carta);
    }
}

void Juego::robarCartaCasualidad(modelo::Jugador& jugador) {
    if (mazoCasualidad_.vacio()) {
        return;
    }
    auto carta = mazoCasualidad_.robar();
    aplicarCarta(carta, jugador);
    if (!carta.conservar()) {
        mazoCasualidad_.ponerAlFondo(carta);
    }
}

void Juego::cobrarImpuesto(modelo::Jugador& jugador, int monto) {
    banco_.cobrarAJugador(jugador, monto);
}

void Juego::visitarCarcel(modelo::Jugador&) {
    // No ocurre nada especial.
}

void Juego::enviarJugadorCarcel(modelo::Jugador& jugador) {
    jugador.encarcelar(posicionCarcel_);
    turnos_.setFase(Turnos::Fase::DebeLanzar);
}

void Juego::estacionamientoLibre(modelo::Jugador&) {
    // Punto de descanso, sin efecto.
}

void Juego::cargarTablero(const std::string& ruta) {
    auto json = util::JsonValue::parseFile(ruta);
    tablero_.limpiar();
    if (!json.isArray()) {
        throw std::runtime_error("El tablero debe ser un arreglo de casillas");
    }
    for (const auto& casilla : json.asArray()) {
        std::string tipo = util::toUpper(casilla.at("tipo").asString());
        std::string nombre = casilla.at("nombre").asString();
        if (tipo == "ESPECIAL") {
            auto categoria = categoriaDesdeTexto(casilla.at("categoria").asString());
            int monto = casilla.at("monto").asInt(0);
            tablero_.agregarCasilla(std::make_unique<modelo::Especial>(nombre, categoria, monto));
        } else if (tipo == "SOLAR") {
            std::string color = casilla.at("color").asString();
            const auto& escrituraJson = casilla.at("escritura");
            std::array<int, 6> rentas{0, 0, 0, 0, 0, 0};
            const auto& rentasJson = escrituraJson.at("rentas").asArray();
            for (std::size_t i = 0; i < rentasJson.size() && i < rentas.size(); ++i) {
                rentas[i] = rentasJson[i].asInt();
            }
            modelo::Escritura escritura(
                escrituraJson.at("precio").asInt(),
                escrituraJson.at("hipoteca").asInt(),
                escrituraJson.at("casa").asInt(),
                escrituraJson.at("hotel").asInt(),
                rentas);
            tablero_.agregarCasilla(std::make_unique<modelo::Solar>(nombre, color, escritura));
        } else if (tipo == "FERROCARRIL") {
            const auto& escrituraJson = casilla.at("escritura");
            std::array<int, 6> rentas{0, 0, 0, 0, 0, 0};
            const auto& rentasJson = escrituraJson.at("rentas").asArray();
            for (std::size_t i = 0; i < rentasJson.size() && i < rentas.size(); ++i) {
                rentas[i] = rentasJson[i].asInt();
            }
            modelo::Escritura escritura(
                escrituraJson.at("precio").asInt(),
                escrituraJson.at("hipoteca").asInt(),
                escrituraJson.at("casa").asInt(0),
                escrituraJson.at("hotel").asInt(0),
                rentas);
            tablero_.agregarCasilla(std::make_unique<modelo::Ferrocarril>(nombre, escritura));
        } else if (tipo == "SERVICIO") {
            const auto& escrituraJson = casilla.at("escritura");
            std::array<int, 6> rentas{0, 0, 0, 0, 0, 0};
            const auto& rentasJson = escrituraJson.at("rentas").asArray();
            for (std::size_t i = 0; i < rentasJson.size() && i < rentas.size(); ++i) {
                rentas[i] = rentasJson[i].asInt();
            }
            modelo::Escritura escritura(
                escrituraJson.at("precio").asInt(),
                escrituraJson.at("hipoteca").asInt(),
                escrituraJson.at("casa").asInt(0),
                escrituraJson.at("hotel").asInt(0),
                rentas);
            int mulUno = casilla.at("multiplicador_uno").asInt(4);
            int mulDos = casilla.at("multiplicador_dos").asInt(10);
            tablero_.agregarCasilla(std::make_unique<modelo::Servicio>(nombre, escritura, mulUno, mulDos));
        }
    }
}

void Juego::cargarCartas(const std::string& ruta, modelo::Mazo& mazo) {
    auto json = util::JsonValue::parseFile(ruta);
    if (!json.isArray()) {
        throw std::runtime_error("Las cartas deben representarse como un arreglo");
    }
    std::vector<modelo::Carta> cartas;
    for (const auto& cartaJson : json.asArray()) {
        std::string texto = cartaJson.at("texto").asString();
        auto accion = accionDesdeTexto(cartaJson.at("accion").asString());
        int valor = cartaJson.at("valor").asInt();
        int destino = cartaJson.at("destino").asInt(-1);
        bool conservar = cartaJson.at("conservar").asBool(false);
        cartas.emplace_back(texto, accion, valor, destino, conservar);
    }
    mazo.cargar(cartas);
}

void Juego::guardarEstado() {
    historial_.apilar(Estado(jugadores_, banco_, turnos_, mazoCasualidad_, mazoArca_));
}

void Juego::restaurarEstado(const juego::Estado& estado) {
    jugadores_ = estado.jugadores();
    banco_ = estado.banco();
    turnos_ = estado.turnos();
    mazoCasualidad_ = estado.mazoCasualidad();
    mazoArca_ = estado.mazoArca();
}

modelo::Propiedad* Juego::propiedadEnPosicion(int posicion) {
    if (posicion < 0 || posicion >= static_cast<int>(tablero_.tamanio())) {
        return nullptr;
    }
    return dynamic_cast<modelo::Propiedad*>(&tablero_.casilla(posicion));
}

modelo::Propiedad* Juego::buscarPropiedad(const std::string& nombre) {
    return indice_.buscar(nombre);
}

int Juego::contarPropiedadesColor(const modelo::Jugador& jugador, const modelo::Solar& solar) {
    int contador = 0;
    for (auto* propiedad : jugador.propiedades()) {
        auto* s = dynamic_cast<modelo::Solar*>(propiedad);
        if (s && util::toUpper(s->color()) == util::toUpper(solar.color())) {
            ++contador;
        }
    }
    return contador;
}

int Juego::contarServicios(const modelo::Jugador& jugador) const {
    int contador = 0;
    for (auto* propiedad : jugador.propiedades()) {
        if (dynamic_cast<modelo::Servicio*>(propiedad) != nullptr) {
            ++contador;
        }
    }
    return contador;
}

int Juego::contarFerrocarriles(const modelo::Jugador& jugador) const {
    int contador = 0;
    for (auto* propiedad : jugador.propiedades()) {
        if (dynamic_cast<modelo::Ferrocarril*>(propiedad) != nullptr) {
            ++contador;
        }
    }
    return contador;
}

void Juego::procesarCasilla(modelo::Jugador& jugador, int tirada) {
    auto& casilla = tablero_.casilla(jugador.posicion());
    casilla.alCaer(*this, jugador, tirada);
}

void Juego::moverJugador(modelo::Jugador& jugador, int pasos, int tirada, bool) {
    if (tablero_.pasoPorSalida(jugador.posicion(), pasos)) {
        banco_.pagarAJugador(jugador, reglas_.salarioSalida());
    }
    jugador.mover(pasos, static_cast<int>(tablero_.tamanio()));
    (void)tirada;
}

void Juego::aplicarCarta(modelo::Carta carta, modelo::Jugador& jugador) {
    switch (carta.accion()) {
        case modelo::Carta::Accion::COBRAR:
            banco_.pagarAJugador(jugador, carta.valor());
            break;
        case modelo::Carta::Accion::PAGAR:
            banco_.cobrarAJugador(jugador, carta.valor());
            break;
        case modelo::Carta::Accion::MOVER: {
            int destino = carta.destino();
            if (destino >= 0) {
                if (destino < jugador.posicion()) {
                    banco_.pagarAJugador(jugador, reglas_.salarioSalida());
                }
                jugador.moverDirecto(destino, static_cast<int>(tablero_.tamanio()));
                procesarCasilla(jugador, carta.valor());
            }
            break;
        }
        case modelo::Carta::Accion::MOVER_RELATIVO:
            moverJugador(jugador, carta.valor(), carta.valor(), false);
            procesarCasilla(jugador, carta.valor());
            break;
        case modelo::Carta::Accion::SALIR_CARCEL:
            jugador.recibirCartaSalirCarcel();
            break;
        case modelo::Carta::Accion::IR_CARCEL:
            enviarJugadorCarcel(jugador);
            break;
        case modelo::Carta::Accion::REPARAR:
            banco_.cobrarAJugador(jugador, carta.valor());
            break;
    }
}

void Juego::asignarPropiedad(modelo::Propiedad& propiedad, modelo::Jugador& jugador) {
    propiedad.asignarDueno(&jugador);
    jugador.agregarPropiedad(&propiedad);
}

void Juego::resolverPropiedad(modelo::Propiedad& propiedad, modelo::Jugador& jugador, int tirada) {
    if (propiedad.dueno() == nullptr) {
        return;
    }
    if (propiedad.dueno() == &jugador) {
        return;
    }
    if (propiedad.estaHipotecada()) {
        return;
    }
    int grupo = 0;
    if (auto* solar = dynamic_cast<modelo::Solar*>(&propiedad)) {
        grupo = contarPropiedadesColor(*propiedad.dueno(), *solar);
    }
    int servicios = contarServicios(*propiedad.dueno());
    int ferrocarriles = contarFerrocarriles(*propiedad.dueno());
    int renta = reglas::CalculoRenta::rentaPropiedad(propiedad, tirada, grupo, servicios, ferrocarriles);
    if (renta <= 0) {
        return;
    }
    if (!jugador.pagar(renta)) {
        // Si no puede pagar, pierde la partida
        jugador.setDinero(0);
        return;
    }
    propiedad.dueno()->cobrar(renta);
}

} // namespace juego

#include "Comandos.h"

#include "Consola.h"
#include "../juego/Juego.h"
#include "../juego/Turnos.h"
#include "../modelo/Casilla.h"
#include "../modelo/Jugador.h"
#include "../modelo/Propiedad.h"
#include "../modelo/Solar.h"
#include "../util/StringUtils.h"

#include <algorithm>
#include <sstream>
#include <vector>

namespace cli {
namespace {

std::string descripcionPosicion(const juego::Juego& juego) {
    if (!juego.partidaActiva()) {
        return "Menú principal";
    }
    const auto& jugador = juego.jugadorActual();
    const auto& casilla = juego.tablero().casilla(jugador.posicion());
    std::ostringstream titulo;
    titulo << "Turno: " << jugador.nombre() << " (dinero: $" << jugador.dinero() << ")";
    titulo << " | Posición: " << casilla.nombre();
    if (const auto* propiedad = dynamic_cast<const modelo::Propiedad*>(&casilla)) {
        if (propiedad->dueno() == nullptr) {
            titulo << " (sin dueño)";
        } else {
            titulo << " (dueño: " << propiedad->dueno()->nombre() << ")";
        }
    }
    return titulo.str();
}

bool esColorIgual(const std::string& a, const std::string& b) {
    return util::toUpper(a) == util::toUpper(b);
}

int pasosDisponibles(const juego::Juego& juego, const modelo::Solar& solar) {
    if (solar.tieneHotel()) {
        return 0;
    }
    int casasBanco = juego.banco().casasDisponibles();
    int hotelesBanco = juego.banco().hotelesDisponibles();
    int dinero = juego.jugadorActual().dinero();

    int casasActuales = solar.casas();
    bool hotel = solar.tieneHotel();
    int pasos = 0;

    while (!hotel) {
        if (casasActuales < 4) {
            if (casasBanco <= 0) {
                break;
            }
            int costo = solar.escritura().costoCasa();
            if (dinero < costo) {
                break;
            }
            dinero -= costo;
            --casasBanco;
            ++casasActuales;
            ++pasos;
            continue;
        }
        if (hotelesBanco <= 0) {
            break;
        }
        int costoHotel = solar.escritura().costoHotel();
        if (dinero < costoHotel) {
            break;
        }
        dinero -= costoHotel;
        --hotelesBanco;
        hotel = true;
        ++pasos;
    }
    return pasos;
}

std::vector<std::string> descripciones(const std::vector<Comandos::Opcion>& opciones) {
    std::vector<std::string> etiquetas;
    etiquetas.reserve(opciones.size());
    for (const auto& opcion : opciones) {
        etiquetas.push_back(opcion.descripcion);
    }
    return etiquetas;
}

} // namespace

bool Comandos::procesar(juego::Juego& juego, Consola& consola) {
    Menu menu = construirMenu(juego);
    if (menu.opciones.empty()) {
        consola.mostrarMensaje("No hay acciones disponibles.");
        return false;
    }
    auto etiquetas = descripciones(menu.opciones);
    int indice = consola.mostrarMenu(menu.titulo, etiquetas);
    return ejecutarAccion(menu.opciones[indice].accion, juego, consola);
}

Comandos::Menu Comandos::construirMenu(const juego::Juego& juego) const {
    Menu menu;
    if (!juego.partidaActiva()) {
        menu.titulo = "No hay partida activa";
        menu.opciones.push_back({Accion::IniciarPartida, "Iniciar nueva partida"});
        menu.opciones.push_back({Accion::Salir, "Salir"});
        return menu;
    }

    const auto& jugador = juego.jugadorActual();
    const bool enCarcel = jugador.enCarcel();
    const auto fase = juego.faseActual();

    if (enCarcel && fase == juego::Turnos::Fase::DebeLanzar) {
        menu.titulo = "Estás en la CÁRCEL. Opciones:";
        menu.opciones.push_back({Accion::TirarDados, "Tirar e intentar dobles"});
        if (jugador.dinero() >= juego.reglas().multaCarcel()) {
            menu.opciones.push_back({Accion::PagarMultaCarcel, "Pagar $" + std::to_string(juego.reglas().multaCarcel()) + " y salir"});
        }
        if (jugador.cartasSalirCarcel() > 0) {
            menu.opciones.push_back({Accion::UsarCartaSalirCarcel, "Usar carta 'Salir de la cárcel' (tienes " + std::to_string(jugador.cartasSalirCarcel()) + ")"});
        }
        menu.opciones.push_back({Accion::MostrarEstado, "Estado"});
        if (juego.puedeDeshacer()) {
            menu.opciones.push_back({Accion::Deshacer, "Undo"});
        }
        menu.opciones.push_back({Accion::Salir, "Salir"});
        return menu;
    }

    menu.titulo = descripcionPosicion(juego);

    if (fase == juego::Turnos::Fase::DebeLanzar) {
        menu.opciones.push_back({Accion::TirarDados, "Tirar dados"});
        menu.opciones.push_back({Accion::MostrarEstado, "Estado"});
        if (juego.puedeDeshacer()) {
            menu.opciones.push_back({Accion::Deshacer, "Undo"});
        }
        menu.opciones.push_back({Accion::Salir, "Salir"});
        return menu;
    }

    const auto& casilla = juego.tablero().casilla(jugador.posicion());
    if (const auto* propiedad = dynamic_cast<const modelo::Propiedad*>(&casilla)) {
        if (propiedad->dueno() == nullptr && jugador.dinero() >= propiedad->escritura().precio()) {
            menu.opciones.push_back({Accion::Comprar, "Comprar esta propiedad ($" + std::to_string(propiedad->escritura().precio()) + ")"});
        }
    }

    if (!solaresConstruibles(juego).empty()) {
        menu.opciones.push_back({Accion::Construir, "Construir"});
    }

    if (!propiedadesDisponibles(juego, false).empty()) {
        menu.opciones.push_back({Accion::Hipotecar, "Hipotecar"});
    }

    if (!propiedadesDisponibles(juego, true).empty()) {
        menu.opciones.push_back({Accion::Deshipotecar, "Deshipotecar"});
    }

    menu.opciones.push_back({Accion::MostrarEstado, "Estado"});
    if (juego.puedeDeshacer()) {
        menu.opciones.push_back({Accion::Deshacer, "Undo"});
    }
    menu.opciones.push_back({Accion::Pasar, "Pasar"});
    menu.opciones.push_back({Accion::Salir, "Salir"});
    return menu;
}

bool Comandos::ejecutarAccion(Accion accion, juego::Juego& juego, Consola& consola) {
    switch (accion) {
        case Accion::Salir:
            return false;
        case Accion::IniciarPartida:
            return manejarInicio(juego, consola);
        case Accion::TirarDados: {
            auto mensaje = juego.lanzarDados();
            consola.mostrarMensaje(mensaje);
            return true;
        }
        case Accion::MostrarEstado:
            consola.mostrarEstado(juego);
            return true;
        case Accion::Comprar: {
            auto mensaje = juego.comprarPropiedadActual();
            consola.mostrarMensaje(mensaje);
            return true;
        }
        case Accion::Pasar: {
            auto mensaje = juego.pasarTurno();
            consola.mostrarMensaje(mensaje);
            consola.mostrarEstado(juego);
            return true;
        }
        case Accion::Construir:
            return manejarConstruccion(juego, consola);
        case Accion::Hipotecar:
            return manejarHipoteca(juego, consola, true);
        case Accion::Deshipotecar:
            return manejarHipoteca(juego, consola, false);
        case Accion::Deshacer: {
            auto mensaje = juego.deshacer();
            consola.mostrarMensaje(mensaje);
            consola.mostrarEstado(juego);
            return true;
        }
        case Accion::UsarCartaSalirCarcel:
            return manejarUsarCarta(juego, consola);
        case Accion::PagarMultaCarcel:
            return manejarPagarMulta(juego, consola);
    }
    return true;
}

bool Comandos::manejarInicio(juego::Juego& juego, Consola& consola) {
    auto nombres = consola.solicitarNombresJugadores();
    if (nombres.size() < 2) {
        consola.mostrarMensaje("Debe ingresar al menos dos nombres.");
        return true;
    }
    if (!juego.iniciarPartida(nombres)) {
        consola.mostrarMensaje("No se pudo iniciar la partida.");
    } else {
        consola.mostrarMensaje("Partida iniciada.");
        consola.mostrarEstado(juego);
    }
    return true;
}

bool Comandos::manejarConstruccion(juego::Juego& juego, Consola& consola) {
    auto disponibles = solaresConstruibles(juego);
    if (disponibles.empty()) {
        consola.mostrarMensaje("No hay propiedades disponibles para construir.");
        return true;
    }
    std::vector<std::string> etiquetas;
    etiquetas.reserve(disponibles.size());
    for (const auto* solar : disponibles) {
        std::ostringstream etiqueta;
        etiqueta << solar->nombre() << " (casas=" << solar->casas();
        if (solar->tieneHotel()) {
            etiqueta << ", hotel";
        }
        etiqueta << ")";
        etiquetas.push_back(etiqueta.str());
    }
    int indice = consola.mostrarMenu("¿Dónde quieres construir?", etiquetas);
    auto* seleccionado = disponibles[indice];
    int maxPasos = pasosDisponibles(juego, *seleccionado);
    if (maxPasos <= 0) {
        consola.mostrarMensaje("No es posible construir en esta propiedad ahora mismo.");
        return true;
    }
    int pasos = consola.solicitarEntero(
        "¿Cuántos pasos de edificación? (1.." + std::to_string(maxPasos) + "): ", 1, maxPasos);

    std::string nombre = seleccionado->nombre();
    for (int i = 0; i < pasos; ++i) {
        std::string mensaje;
        if (!seleccionado->tieneHotel() && seleccionado->casas() < 4) {
            mensaje = juego.construirCasa(nombre);
        } else if (!seleccionado->tieneHotel()) {
            mensaje = juego.construirHotel(nombre);
        } else {
            mensaje = "La propiedad ya tiene un hotel";
        }
        consola.mostrarMensaje(mensaje);
        if (mensaje != "Casa construida" && mensaje != "Hotel construido") {
            break;
        }
    }
    return true;
}

bool Comandos::manejarHipoteca(juego::Juego& juego, Consola& consola, bool hipotecar) {
    auto lista = propiedadesDisponibles(juego, !hipotecar);
    if (lista.empty()) {
        consola.mostrarMensaje(hipotecar ? "No tienes propiedades para hipotecar." : "No tienes hipotecas para cancelar.");
        return true;
    }
    std::vector<std::string> etiquetas;
    etiquetas.reserve(lista.size());
    for (const auto* propiedad : lista) {
        etiquetas.push_back(propiedad->nombre());
    }
    std::string titulo = hipotecar ? "¿Qué propiedad deseas hipotecar?" : "¿Qué hipoteca deseas cancelar?";
    int indice = consola.mostrarMenu(titulo, etiquetas);
    auto* seleccionada = lista[indice];
    std::string resultado = hipotecar ? juego.hipotecarPropiedad(seleccionada->nombre())
                                      : juego.deshipotecarPropiedad(seleccionada->nombre());
    consola.mostrarMensaje(resultado);
    return true;
}

bool Comandos::manejarUsarCarta(juego::Juego& juego, Consola& consola) {
    auto mensaje = juego.usarCartaSalirCarcel();
    consola.mostrarMensaje(mensaje);
    return true;
}

bool Comandos::manejarPagarMulta(juego::Juego& juego, Consola& consola) {
    auto mensaje = juego.pagarMultaCarcel();
    consola.mostrarMensaje(mensaje);
    return true;
}

std::vector<modelo::Solar*> Comandos::solaresConstruibles(const juego::Juego& juego) const {
    std::vector<modelo::Solar*> resultado;
    if (!juego.partidaActiva()) {
        return resultado;
    }
    const auto& jugador = juego.jugadorActual();
    for (auto* propiedad : jugador.propiedades()) {
        auto* solar = dynamic_cast<modelo::Solar*>(propiedad);
        if (!solar) {
            continue;
        }
        if (propiedad->estaHipotecada()) {
            continue;
        }
        if (!grupoCompleto(juego, *solar, jugador)) {
            continue;
        }
        if (pasosDisponibles(juego, *solar) > 0) {
            resultado.push_back(solar);
        }
    }
    return resultado;
}

std::vector<modelo::Propiedad*> Comandos::propiedadesDisponibles(const juego::Juego& juego, bool hipotecadas) const {
    std::vector<modelo::Propiedad*> resultado;
    if (!juego.partidaActiva()) {
        return resultado;
    }
    const auto& jugador = juego.jugadorActual();
    int interes = juego.reglas().interesHipoteca();
    for (auto* propiedad : jugador.propiedades()) {
        if (propiedad->estaHipotecada() != hipotecadas) {
            continue;
        }
        if (hipotecadas) {
            int monto = propiedad->escritura().hipoteca();
            int total = monto + (monto * interes) / 100;
            if (jugador.dinero() < total) {
                continue;
            }
        }
        resultado.push_back(propiedad);
    }
    return resultado;
}

bool Comandos::grupoCompleto(const juego::Juego& juego, const modelo::Solar& solar, const modelo::Jugador& jugador) const {
    int total = 0;
    int propias = 0;
    for (std::size_t i = 0; i < juego.tablero().tamanio(); ++i) {
        auto* casilla = dynamic_cast<modelo::Solar*>(&juego.tablero().casilla(i));
        if (!casilla) {
            continue;
        }
        if (!esColorIgual(casilla->color(), solar.color())) {
            continue;
        }
        ++total;
        if (casilla->dueno() == &jugador && !casilla->estaHipotecada()) {
            ++propias;
        }
    }
    return total > 0 && propias == total;
}

} // namespace cli

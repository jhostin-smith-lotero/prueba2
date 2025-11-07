#pragma once

#include <string>
#include <vector>

namespace juego {
class Juego;
}

namespace modelo {
class Jugador;
class Propiedad;
class Solar;
}

namespace cli {

class Consola;

class Comandos {
public:
    enum class Accion {
        IniciarPartida,
        TirarDados,
        MostrarEstado,
        Comprar,
        Pasar,
        Construir,
        Hipotecar,
        Deshipotecar,
        Deshacer,
        Salir,
        UsarCartaSalirCarcel,
        PagarMultaCarcel
    };

    struct Opcion {
        Accion accion;
        std::string descripcion;
    };

    struct Menu {
        std::string titulo;
        std::vector<Opcion> opciones;
    };

    bool procesar(juego::Juego& juego, Consola& consola);

private:
    Menu construirMenu(const juego::Juego& juego) const;
    bool ejecutarAccion(Accion accion, juego::Juego& juego, Consola& consola);
    bool manejarInicio(juego::Juego& juego, Consola& consola);
    bool manejarConstruccion(juego::Juego& juego, Consola& consola);
    bool manejarHipoteca(juego::Juego& juego, Consola& consola, bool hipotecar);
    bool manejarUsarCarta(juego::Juego& juego, Consola& consola);
    bool manejarPagarMulta(juego::Juego& juego, Consola& consola);

    std::vector<modelo::Solar*> solaresConstruibles(const juego::Juego& juego) const;
    std::vector<modelo::Propiedad*> propiedadesDisponibles(const juego::Juego& juego, bool hipotecadas) const;
    bool grupoCompleto(const juego::Juego& juego, const modelo::Solar& solar, const modelo::Jugador& jugador) const;
};

} // namespace cli

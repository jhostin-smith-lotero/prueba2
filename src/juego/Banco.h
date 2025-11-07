#pragma once

namespace modelo {
class Jugador;
class Propiedad;
class Solar;
}

namespace juego {

class Banco {
public:
    void inicializar(int efectivo, int casasDisponibles, int hotelesDisponibles);

    int efectivo() const { return efectivo_; }
    int casasDisponibles() const { return casasDisponibles_; }
    int hotelesDisponibles() const { return hotelesDisponibles_; }

    void ajustarEfectivo(int delta);

    bool pagarAJugador(modelo::Jugador& jugador, int monto);
    bool cobrarAJugador(modelo::Jugador& jugador, int monto);

    bool tomarCasas(int cantidad);
    void devolverCasas(int cantidad);
    bool tomarHoteles(int cantidad);
    void devolverHoteles(int cantidad);

private:
    int efectivo_ = 0;
    int casasDisponibles_ = 32;
    int hotelesDisponibles_ = 12;
};

} // namespace juego

#pragma once

#include <cstddef>

namespace juego {

class Turnos {
public:
    enum class Fase { DebeLanzar, Acciones };

    void iniciar(std::size_t cantidadJugadores);
    std::size_t actual() const { return actual_; }
    Fase fase() const { return fase_; }
    int doblesConsecutivos() const { return doblesConsecutivos_; }
    bool partidaActiva() const { return cantidadJugadores_ > 0; }

    void registrarLanzamiento(bool doble);
    void pasarTurno();
    void forzarTurno(std::size_t jugador);
    void setFase(Fase fase) { fase_ = fase; }
    void reiniciarDobles() { doblesConsecutivos_ = 0; }

private:
    std::size_t cantidadJugadores_ = 0;
    std::size_t actual_ = 0;
    Fase fase_ = Fase::DebeLanzar;
    int doblesConsecutivos_ = 0;
};

} // namespace juego

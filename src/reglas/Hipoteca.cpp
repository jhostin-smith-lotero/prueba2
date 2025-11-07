#include "Hipoteca.h"

#include "../juego/Banco.h"
#include "../modelo/Jugador.h"
#include "../modelo/Propiedad.h"

namespace reglas {

bool Hipoteca::hipotecar(modelo::Propiedad& propiedad,
                          modelo::Jugador& jugador,
                          juego::Banco& banco,
                          int,
                          std::string& mensaje) {
    if (propiedad.estaHipotecada()) {
        mensaje = "La propiedad ya está hipotecada";
        return false;
    }
    propiedad.hipotecar();
    int monto = propiedad.escritura().hipoteca();
    jugador.cobrar(monto);
    banco.ajustarEfectivo(-monto);
    mensaje.clear();
    return true;
}

bool Hipoteca::deshipotecar(modelo::Propiedad& propiedad,
                             modelo::Jugador& jugador,
                             juego::Banco& banco,
                             int interes,
                             std::string& mensaje) {
    if (!propiedad.estaHipotecada()) {
        mensaje = "La propiedad no está hipotecada";
        return false;
    }
    int monto = propiedad.escritura().hipoteca();
    int total = monto + (monto * interes) / 100;
    if (!jugador.pagar(total)) {
        mensaje = "Dinero insuficiente para liberar la hipoteca";
        return false;
    }
    banco.ajustarEfectivo(total);
    propiedad.deshipotecar();
    mensaje.clear();
    return true;
}

} // namespace reglas

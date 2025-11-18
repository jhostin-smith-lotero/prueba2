#ifndef CARTA_H
#define CARTA_H
#include <string>

namespace modelo {

    struct Carta {
    public:
        enum class Accion {
            COBRAR,
            PAGAR,
            MOVER,
            MOVER_RELATIVO,
            SALIR_CARCEL,
            IR_CARCEL,
            REPARAR
        };

        Carta() = default;
        Carta(const std::string& texto, Accion accion, int valor, int destino = -1, bool conservar = false);

        const std::string& texto() const { return texto_; }
        Accion accion() const { return accion_; }
        int valor() const { return valor_; }
        int destino() const { return destino_; }
        bool conservar() const { return conservar_; }

    private:
        std::string texto_;
        Accion accion_ = Accion::COBRAR;
        int valor_ = 0;
        int destino_ = -1;
        bool conservar_ = false;
    };

}
#endif //CARTA_H

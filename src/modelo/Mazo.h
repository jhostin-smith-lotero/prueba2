#ifndef MAZO_H
#define MAZO_H

#include "Carta.h"
#include "../edd/Cola.h"

namespace modelo {

struct Mazo {
public:
    Mazo() {}

    void agregar(const Carta& carta) { cartas_.enqueue(carta); }

    bool robar(Carta& carta) { return cartas_.dequeue(carta); }

    void ponerAlFondo(const Carta& carta) { cartas_.enqueue(carta); }

    bool vacio() const { return cartas_.vacia(); }

    std::size_t tam() const { return cartas_.tam(); }

private:
    edd::Cola<Carta> cartas_;
};

}

#endif //MAZO_H

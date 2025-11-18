#ifndef ESCRITURA_H
#define ESCRITURA_H
#include <cstddef>

namespace modelo {

struct Escritura {
public:
    Escritura() { inicializarRentas(); }

    Escritura(int precio, int hipoteca, int costoCasa, int costoHotel, const int rentas[6])
        : precio_(precio), hipoteca_(hipoteca), costoCasa_(costoCasa), costoHotel_(costoHotel) {
        for (std::size_t i = 0; i < 6; ++i) {
            rentas_[i] = rentas[i];
        }
    }

    int precio() const { return precio_; }
    int hipoteca() const { return hipoteca_; }
    int costoCasa() const { return costoCasa_; }
    int costoHotel() const { return costoHotel_; }
    int rentaBase() const { return rentas_[0]; }
    int rentaConCasas(int casas) const { return rentas_[casas]; }
    int rentaConHotel() const { return rentas_[5]; }

private:
    void inicializarRentas() {
        for (std::size_t i = 0; i < 6; ++i) {
            rentas_[i] = 0;
        }
    }

    int precio_ = 0;
    int hipoteca_ = 0;
    int costoCasa_ = 0;
    int costoHotel_ = 0;
    int rentas_[6];
};

}
#endif //ESCRITURA_H

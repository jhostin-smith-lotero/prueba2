#pragma once

#include <string>
#include <vector>

namespace modelo {

class Propiedad;

class Jugador {
public:
    explicit Jugador(std::string nombre = "");

    const std::string& nombre() const { return nombre_; }
    int dinero() const { return dinero_; }
    int posicion() const { return posicion_; }
    bool enCarcel() const { return enCarcel_; }
    int turnosEnCarcel() const { return turnosEnCarcel_; }
    int cartasSalirCarcel() const { return cartasSalirCarcel_; }
    const std::vector<Propiedad*>& propiedades() const { return propiedades_; }

    void setDinero(int dinero) { dinero_ = dinero; }
    void mover(int pasos, int limite);
    void moverDirecto(int posicion, int limite);
    void ajustarDinero(int delta);
    bool pagar(int monto);
    void cobrar(int monto);
    void encarcelar(int posicionCarcel);
    void liberarDeCarcel();
    void incrementarTurnoCarcel();
    void reiniciarTurnoCarcel();
    void recibirCartaSalirCarcel();
    bool usarCartaSalirCarcel();

    void agregarPropiedad(Propiedad* propiedad);
    void quitarPropiedad(Propiedad* propiedad);

private:
    std::string nombre_;
    int dinero_ = 0;
    int posicion_ = 0;
    bool enCarcel_ = false;
    int turnosEnCarcel_ = 0;
    int cartasSalirCarcel_ = 0;
    std::vector<Propiedad*> propiedades_;
};

} // namespace modelo

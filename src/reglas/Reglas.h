#pragma once

#include <string>

namespace util {
class JsonValue;
}

namespace reglas {

class Reglas {
public:
    void cargar(const std::string& rutaConfig);

    int salarioSalida() const { return salarioSalida_; }
    int multaCarcel() const { return multaCarcel_; }
    int interesHipoteca() const { return interesHipoteca_; }
    int casasBanco() const { return casasBanco_; }
    int hotelesBanco() const { return hotelesBanco_; }
    int dineroInicialJugadores() const { return dineroInicial_; }
    int dineroBanco() const { return dineroBanco_; }

private:
    void cargarDesdeJson(const util::JsonValue& json);

    int salarioSalida_ = 200;
    int multaCarcel_ = 50;
    int interesHipoteca_ = 10;
    int casasBanco_ = 32;
    int hotelesBanco_ = 12;
    int dineroInicial_ = 1500;
    int dineroBanco_ = 20580;
};

} // namespace reglas

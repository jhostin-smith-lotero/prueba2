#include "Reglas.h"

#include "../util/Json.h"

#include <stdexcept>

namespace reglas {

void Reglas::cargar(const std::string& rutaConfig) {
    auto json = util::JsonValue::parseFile(rutaConfig);
    cargarDesdeJson(json);
}

void Reglas::cargarDesdeJson(const util::JsonValue& json) {
    if (json.isObject()) {
        salarioSalida_ = json.at("salario_go").asInt(salarioSalida_);
        multaCarcel_ = json.at("multa_carcel").asInt(multaCarcel_);
        interesHipoteca_ = json.at("interes_hipoteca").asInt(interesHipoteca_);
        casasBanco_ = json.at("casas_banco").asInt(casasBanco_);
        hotelesBanco_ = json.at("hoteles_banco").asInt(hotelesBanco_);
        dineroInicial_ = json.at("dinero_inicial").asInt(dineroInicial_);
        dineroBanco_ = json.at("dinero_banco").asInt(dineroBanco_);
    } else {
        throw std::runtime_error("El archivo de configuración no contiene un objeto JSON");
    }
}

} // namespace reglas

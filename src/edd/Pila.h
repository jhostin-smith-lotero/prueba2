#pragma once

#include <utility>
#include <vector>

namespace edd {

template <typename T>
class Pila {
public:
    void apilar(const T& valor) {
        datos_.push_back(valor);
    }

    void apilar(T&& valor) {
        datos_.push_back(std::move(valor));
    }

    bool desapilar(T& out) {
        if (datos_.empty()) {
            return false;
        }
        out = std::move(datos_.back());
        datos_.pop_back();
        return true;
    }

    bool vacia() const {
        return datos_.empty();
    }

    void limpiar() {
        datos_.clear();
    }

    std::size_t tam() const {
        return datos_.size();
    }

private:
    std::vector<T> datos_;
};

} // namespace edd

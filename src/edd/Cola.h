#pragma once

#include <deque>

namespace edd {

template <typename T>
class Cola {
public:
    void enqueue(const T& valor) {
        datos_.push_back(valor);
    }

    void enqueue(T&& valor) {
        datos_.push_back(std::move(valor));
    }

    bool dequeue(T& out) {
        if (datos_.empty()) {
            return false;
        }
        out = std::move(datos_.front());
        datos_.pop_front();
        return true;
    }

    bool vacia() const {
        return datos_.empty();
    }

    std::size_t tam() const {
        return datos_.size();
    }

    void clear() {
        datos_.clear();
    }

private:
    std::deque<T> datos_;
};

} // namespace edd

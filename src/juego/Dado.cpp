#include "Dado.h"
#include <cstdlib>
#include <ctime>

int Dado::lanzar() {
    return (std::rand() % 6) + 1;
}

# Monopoly de Terminal en C++

Este proyecto implementa una versión de Monopoly jugable desde la terminal y desarrollada en C++.

## Compilación

1. Desde la raíz del repositorio, crea el directorio de salida si no existe:
   ```bash
   mkdir -p build
   ```
2. Compila todas las fuentes y genera el ejecutable en `build/`:
   ```bash
   g++ src/*.cpp src/cli/*.cpp src/juego/*.cpp src/modelo/*.cpp src/reglas/*.cpp -o build/juego.exe
   ```

## Ejecución

Ejecuta el juego desde la raíz del proyecto:
```bash
./build/juego.exe
```

cmake_minimum_required(VERSION 3.20)
project(matching_engine VERSION 1.0.0 LANGUAGES CXX)

# 1. Estándar C++20 estricto
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. Configuración de banderas de optimización de alto rendimiento
if(MSVC)
    add_compile_options(/W4 /O2 /arch:AVX2)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3 -march=native)
endif()

# 3. Directorio de cabeceras públicas
include_directories(include)

# 4. Crear la librería C++ estática (Core Engine)
add_library(matching_core STATIC
    src/engine.cpp
)

target_include_directories(matching_core PUBLIC include)

# 5. Buscar Pybind11 e integrar el módulo para Python
find_package(pybind11 CONFIG REQUIRED)

# Define el módulo de Python 'matching_engine' basado en src/bindings.cpp
pybind11_add_module(matching_engine
    src/bindings.cpp
)

# Enlazar la librería core de C++ con el módulo de Pybind11
target_link_libraries(matching_engine PRIVATE matching_core)

# 6. Habilitar la suite de pruebas unitarias en C++
enable_testing()
add_subdirectory(tests)
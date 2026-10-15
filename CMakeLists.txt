cmake_minimum_required(VERSION 3.20)
project(matching_engine VERSION 1.0.0 LANGUAGES CXX)

# 1. Estándar C++20 estricto
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. Configuración de banderas de optimización de alto rendimiento
if(MSVC)
    add_compile_options(/W4 /O2 /arch:AVX2)
elseif(APPLE)
    # macOS Clang: Usamos -O3 para máxima optimización sin -march=native
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
else()
    # Linux / GCC: Conservamos -march=native para exprimir la arquitectura
    add_compile_options(-Wall -Wextra -Wpedantic -O3 -march=native)
endif()

# 3. Directorio de cabeceras públicas
include_directories(include)

# 4. Crear la librería C++ estática (Core Engine)
add_library(matching_core STATIC
    src/engine.cpp
)

# Activar -fPIC para permitir enlazar esta librería estática con módulos .so de Python
set_property(TARGET matching_core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(matching_core PUBLIC include)

# 5. Descargar e integrar Pybind11 automáticamente
include(FetchContent)

FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)

FetchContent_MakeAvailable(pybind11)

# Define el módulo de Python 'matching_engine' basado en src/bindings.cpp
pybind11_add_module(matching_engine
    src/bindings.cpp
)

# Enlazar la librería core de C++ con el módulo de Pybind11
target_link_libraries(matching_engine PRIVATE matching_core)

# 6. Habilitar la suite de pruebas unitarias en C++
enable_testing()
add_subdirectory(tests)
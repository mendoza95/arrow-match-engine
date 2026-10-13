# Descargar o buscar FetchContent para Catch2 (Framework de Pruebas)
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

# Crear el ejecutable de pruebas
add_executable(unit_tests test_matching.cpp)

# Enlazar con el core del motor y Catch2
target_link_libraries(unit_tests PRIVATE matching_core Catch2::Catch2WithMain)

# Registrar la prueba en CTest
include(Catch)
catch_discover_tests(unit_tests)
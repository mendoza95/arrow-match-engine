# arrow-match-engine

```text
matching-engine/
├── .gitignore
├── README.md
├── CMakeLists.txt
├── include/
│   └── matching/
│       ├── engine.hpp          # Declaración del motor de matching y estructuras
│       └── timer.hpp           # Medidor de rendimiento
├── src/
│   ├── engine.cpp              # Lógica multihilo y cálculos vectoriales
│   └── bindings.cpp            # Módulo de Pybind11 para exponer la memoria a Python
├── tests/
│   ├── CMakeLists.txt
│   └── test_matching.cpp       # Pruebas unitarias en C++ (Catch2 o GoogleTest)
├── python/
│   ├── benchmark.py            # Script para medir velocidad y zero-copy
│   └── requirements.txt        # Dependencias (pyarrow, numpy, pybind11)
└── cmake/
    └── PreventInSourceBuilds.cmake
```
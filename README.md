# arrow-match-engine

matching-engine/
├── .gitignore
├── README.md
├── CMakeLists.txt
├── include/
│   └── matching/
│       ├── engine.hpp          # Matching engine and structure declaration
│       └── timer.hpp           # Performance measurament
├── src/
│   ├── engine.cpp              # Vectorial computation and multithread logic
│   └── bindings.cpp            # Pybind11 module to expose memory to Python
├── tests/
│   ├── CMakeLists.txt
│   └── test_matching.cpp       # Unit tests in C++ (Catch2 o GoogleTest)
├── python/
│   ├── benchmark.py            # Script for measuring speed and zero-copy
│   └── requirements.txt        # Dependencies (pyarrow, numpy, pybind11)
└── cmake/
    └── PreventInSourceBuilds.cmake
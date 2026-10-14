# arrow-match-engine

A high-performance, low-latency C++/Python vector and entity matching engine designed for high-throughput data processing systems. Built with **C++20**, **Pybind11**, and **Apache Arrow** integration in mind, this engine performs multi-threaded cosine similarity search and top-$K$ vector matching over contiguous memory buffers without copying data between runtime environments.

---

## Key Features

* **Zero-Copy Interoperability:** Operates directly on contiguous memory buffers (`const float*`) exposed by Python (`NumPy` / `PyArrow`) via `Pybind11` buffer protocol, eliminating allocation overhead and memory churn.
* **Multi-Threaded Execution:** Parallelizes vector batch queries across threads using `std::jthread` / `std::thread` worker partitioning with zero mutex contention on read-only target buffers.
* **Memory & Hardware Efficiency:** Designed around CPU cache locality, SIMD vectorization friendliness (`-O3 -march=native`), and data alignment for modern architectures.
* **Low-Level Instrumentation:** Includes custom high-resolution timers (`std::chrono`) for precise micro-benchmarking, profiling with `perf` / `Instruments`, and memory safety verification using AddressSanitizer (`ASan`) and ThreadSanitizer (`TSan`).
* **Robust Testing Framework:** Integrated with `Catch2` for automated C++ unit testing and memory safety regression testing.

---

## Architecture Overview

The engine compares a batch of **Query Vectors** ($Q \in \mathbb{R}^{N \times D}$) against a dataset of **Target Vectors** ($T \in \mathbb{R}^{M \times D}$) using cosine similarity:

$$\text{Cosine Similarity}(a, b) = \frac{a \cdot b}{\Vert{}a\Vert{} \Vert{}b\Vert{}}$$

1. **Batch Ingestion:** Python/Arrow buffers are passed into C++ as raw pointers without memory duplication.
2. **Parallel Processing:** Query batches are partitioned across threads. Each worker thread computes candidate similarity scores across target vectors independently.
3. **Result Extraction:** Matches are evaluated to return `MatchResult` structures containing query indices, top target indices, and maximum similarity scores.

---

## Project Structure

```text
matching-engine/
├── .gitignore
├── README.md
├── CMakeLists.txt
├── include/
│   └── matching/
│       ├── engine.hpp          # Engine declarations and data structures
│       └── timer.hpp           # High-resolution performance timer
├── src/
│   ├── engine.cpp              # Multi-threaded matching core logic
│   └── bindings.cpp            # Pybind11 Python extension bindings
├── tests/
│   ├── CMakeLists.txt
│   └── test_matching.cpp       # C++ unit test suite (Catch2)
├── python/
│   ├── benchmark.py            # Micro-benchmarks & zero-copy verification
│   └── requirements.txt        # Python dependencies (pyarrow, numpy, pybind11)
└── cmake/
    └── PreventInSourceBuilds.cmake
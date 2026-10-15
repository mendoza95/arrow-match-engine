#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "matching/engine.hpp"

namespace py = pybind11;
using namespace matching;

PYBIND11_MODULE(matching_engine, m) {
    m.doc() = "High-Performance Zero-Copy Vector & Entity Matching Engine";

    // 1. Exponer la estructura MatchResult a Python
    py::class_<MatchResult>(m, "MatchResult")
        .def(py::init<>())
        .def_readwrite("query_idx", &MatchResult::query_idx)
        .def_readwrite("best_match_idx", &MatchResult::best_match_idx)
        .def_readwrite("max_score", &MatchResult::max_score)
        .def("__repr__", [](const MatchResult& r) {
            return "<MatchResult query_idx=" + std::to_string(r.query_idx) +
                   " best_match_idx=" + std::to_string(r.best_match_idx) +
                   " max_score=" + std::to_string(r.max_score) + ">";
        });

    // 2. Exponer la función match_batch_parallel con Zero-Copy mediante Buffer Protocol
    m.def("match_batch_parallel", [](
        py::array_t<float, py::array::c_style | py::array::forcecast> queries,
        py::array_t<float, py::array::c_style | py::array::forcecast> targets,
        std::size_t num_threads
    ) {
        // Obtener la información del buffer de las matrices de NumPy
        py::buffer_info buf_q = queries.request();
        py::buffer_info buf_t = targets.request();

        // Validaciones de dimensión (deben ser matrices 2D)
        if (buf_q.ndim != 2 || buf_t.ndim != 2) {
            throw std::runtime_error("Las matrices de entrada 'queries' y 'targets' deben ser bidimensionales (2D).");
        }

        // Validar que la dimensión de características (columnas) coincida
        if (buf_q.shape[1] != buf_t.shape[1]) {
            throw std::runtime_error("La dimensión de vectores (columnas) en queries y targets debe ser idéntica.");
        }

        std::size_t num_queries = buf_q.shape[0];
        std::size_t num_targets = buf_t.shape[0];
        std::size_t dim = buf_q.shape[1];

        // Obtener punteros brutos directos a la memoria de Python (Zero-Copy)
        const float* ptr_q = static_cast<const float*>(buf_q.ptr);
        const float* ptr_t = static_cast<const float*>(buf_t.ptr);

        // Llamar al motor de C++20 multihilo
        return VectorMatchingEngine::match_batch_parallel(
            ptr_q, num_queries,
            ptr_t, num_targets,
            dim, num_threads
        );
    },
    py::arg("queries"),
    py::arg("targets"),
    py::arg("num_threads") = 4,
    "Ejecuta el matching vectorial multihilo directamente sobre buffers de memoria de Python (Zero-Copy)."
    );
}
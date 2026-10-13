#pragma once

#include <vector>
#include <cstddef>

namespace matching {

// Estructura de salida para el resultado de un vector de consulta
struct MatchResult {
    std::size_t query_idx{0};      // Índice del vector consultado
    std::size_t best_match_idx{0}; // Índice del objetivo con mayor similitud
    float max_score{-1.0f};        // Puntaje de similitud (e.g. Cosine Similarity)
};

class VectorMatchingEngine {
public:
    VectorMatchingEngine() = default;

    // Calcula la similitud de coseno entre dos vectores continuos en memoria
    [[nodiscard]] static float cosine_similarity(
        const float* vec_a, 
        const float* vec_b, 
        std::size_t dim
    ) noexcept;

    // Ejecuta el matching en paralelo dividiendo las consultas entre N hilos
    [[nodiscard]] static std::vector<MatchResult> match_batch_parallel(
        const float* queries_ptr,
        std::size_t num_queries,
        const float* targets_ptr,
        std::size_t num_targets,
        std::size_t dim,
        std::size_t num_threads = 4
    );
};

} // namespace matching
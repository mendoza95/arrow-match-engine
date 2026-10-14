#include "matching/engine.hpp"

#include <cmath>
#include <algorithm>
#include <thread>

namespace matching {

float VectorMatchingEngine::cosine_similarity(
    const float* vec_a, 
    const float* vec_b, 
    std::size_t dim
) noexcept {
    // 1. Declara acumuladores para dot, norm_a, norm_b
    float dot = 0.0f; // stores dot product between vectors vec_a and vec_b
    float norm_a = 0.0f; //stores the sum of squares of vector vec_a
    float norm_b = 0.0f; //stores the sum of squares of vector vec_b
    
    // 2. Bucle para calcular productos y cuadrados
    for(std::size_t i = 0 ; i < dim ; ++i){
        dot += vec_a[i]*vec_b[i];
        norm_a += vec_a[i]*vec_a[i];
        norm_b += vec_b[i]*vec_b[i];
    }
    
    // 3. Validar magnitudes con std::sqrt y retornar el resultado
    const float denom = std::sqrt(norm_a) * std::sqrt(norm_b);
    return (denom > 0.0f) ? (dot / denom) : 0.0f;
}

// Stub temporal para poder compilar y probar Fase 1
std::vector<MatchResult> VectorMatchingEngine::match_batch_parallel(
    const float* queries_ptr,
    std::size_t num_queries,
    const float* targets_ptr,
    std::size_t num_targets,
    std::size_t dim,
    std::size_t num_threads
) {
    //Edge case: no data
    if (num_queries == 0 || num_targets == 0 || dim == 0){
        return {};
    }
    
    //Reserve space for results
    //This ensures that every element results[q] already exists in memory
    //and every thread will write on its own range without reassignation
    std::vector<MatchResult> results(num_queries);

    //Compute the number of threads so that there are
    //not more threads than queries
    const std::size_t actual_threads = std::min(num_threads, num_queries);
    const std::size_t chunk_size = (num_queries + actual_threads - 1) / actual_threads;

    //Vector to store the threads
    std::vector<std::thread> workers;
    workers.reserve(actual_threads);

    
    for(std::size_t t = 0 ; t < actual_threads ; ++t){
        std::size_t start_idx, end_idx;
        start_idx = t * chunk_size;
        end_idx = std::min(start_idx + chunk_size, num_queries);
        if (start_idx >= num_queries) break;

        // Lanzamos el hilo y le asignamos su tarea
        workers.emplace_back([=, &results]() {
            for (std::size_t q = start_idx; q < end_idx; ++q) {
                const float* q_vec = queries_ptr + (q * dim);
                
                float best_score = -1.0f;
                std::size_t best_idx = 0;

                // Compara la consulta actual 'q' contra todos los targets
                for (std::size_t target = 0; target < num_targets; ++target) {
                    const float* t_vec = targets_ptr + (target * dim);
                    float score = cosine_similarity(q_vec, t_vec, dim);

                    if (score > best_score) {
                        best_score = score;
                        best_idx = target;
                    }
                }

                // Guarda el resultado directamente en el vector compartido (sin mutex)
                results[q] = MatchResult{q, best_idx, best_score};
            }
        });
    }

    // Esperar a que todos los hilos terminen sus cálculos
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    return results;
}

} // namespace matching
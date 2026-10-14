#pragma once

#include <vector>
#include <cstddef>

namespace matching {

// Output Struct for a given query vector
struct MatchResult {
    std::size_t query_idx{0};      // Query vector index
    std::size_t best_match_idx{0}; // Target vector index with best cosine similarity
    float max_score{-1.0f};        // Cosine similarity score
};

class VectorMatchingEngine {
public:
    VectorMatchingEngine() = default;
    //It computes the cosine similarity between two continuous vectors in memory
    [[nodiscard]] static float cosine_similarity(
        const float* vec_a, 
        const float* vec_b, 
        std::size_t dim
    ) noexcept;

    //It executes the matching in parallel spliting the queries among N threads
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
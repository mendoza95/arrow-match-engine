#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "matching/engine.hpp"
#include <vector>

TEST_CASE("Cosine Similarity - Identical Vectors", "[math]") {
    std::vector<float> a = {1.0f, 2.0f, 3.0f};
    std::vector<float> b = {1.0f, 2.0f, 3.0f};

    float result = matching::VectorMatchingEngine::cosine_similarity(a.data(), b.data(), a.size());

    // Se usa WithinRel de Catch2 para comparar flotantes con margen de error
    REQUIRE_THAT(result, Catch::Matchers::WithinRel(1.0f, 0.0001f));
}

TEST_CASE("Cosine Similarity - Orthogonal Vectors", "[math]") {
    std::vector<float> a = {1.0f, 0.0f};
    std::vector<float> b = {0.0f, 1.0f};

    float result = matching::VectorMatchingEngine::cosine_similarity(a.data(), b.data(), a.size());

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0f, 0.0001f));
}

TEST_CASE("Cosine Similarity - Opposite Vectors", "[math]") {
    std::vector<float> a = {1.0f, 2.0f, 3.0f};
    std::vector<float> b = {-1.0f, -2.0f, -3.0f};

    float result = matching::VectorMatchingEngine::cosine_similarity(a.data(), b.data(), a.size());

    REQUIRE_THAT(result, Catch::Matchers::WithinRel(-1.0f, 0.0001f));
}

TEST_CASE("Parallel Matching - Deterministic Batch Test", "[parallel]") {
    // Matriz de 3 Consultas (2D, Dimensión = 3)
    std::vector<float> queries = {
        1.0f, 0.0f, 0.0f,  // Q0: Debe hacer match exacto con T0
        0.0f, 1.0f, 0.0f,  // Q1: Debe hacer match exacto con T1
        0.0f, 0.0f, 1.0f   // Q2: Debe hacer match exacto con T2
    };

    // Matriz de 3 Objetivos (2D, Dimensión = 3)
    std::vector<float> targets = {
        1.0f, 0.0f, 0.0f,  // T0
        0.0f, 1.0f, 0.0f,  // T1
        0.0f, 0.0f, 1.0f   // T2
    };

    const std::size_t num_queries = 3;
    const std::size_t num_targets = 3;
    const std::size_t dim = 3;
    const std::size_t num_threads = 2; // Probar distribución entre 2 hilos

    auto results = matching::VectorMatchingEngine::match_batch_parallel(
        queries.data(),
        num_queries,
        targets.data(),
        num_targets,
        dim,
        num_threads
    );

    // Verificar que se hayan procesado las 3 consultas
    REQUIRE(results.size() == num_queries);

    // Verificar resultados
    REQUIRE(results[0].query_idx == 0);
    REQUIRE(results[0].best_match_idx == 0);
    REQUIRE_THAT(results[0].max_score, Catch::Matchers::WithinRel(1.0f, 0.0001f));

    REQUIRE(results[1].query_idx == 1);
    REQUIRE(results[1].best_match_idx == 1);
    REQUIRE_THAT(results[1].max_score, Catch::Matchers::WithinRel(1.0f, 0.0001f));

    REQUIRE(results[2].query_idx == 2);
    REQUIRE(results[2].best_match_idx == 2);
    REQUIRE_THAT(results[2].max_score, Catch::Matchers::WithinRel(1.0f, 0.0001f));
}

TEST_CASE("Parallel Matching - Stress Concurrency Test", "[parallel]") {
    const std::size_t num_queries = 1000;
    const std::size_t num_targets = 100;
    const std::size_t dim = 128;
    const std::size_t num_threads = 8; // Forzar división entre 8 hilos

    // Generar datos sintéticos
    std::vector<float> queries(num_queries * dim, 0.5f);
    std::vector<float> targets(num_targets * dim, 0.5f);

    // Hacer que T42 sea una coincidencia idéntica para Q0
    for (std::size_t d = 0; d < dim; ++d) {
        targets[42 * dim + d] = 0.5f;
    }

    auto results = matching::VectorMatchingEngine::match_batch_parallel(
        queries.data(),
        num_queries,
        targets.data(),
        num_targets,
        dim,
        num_threads
    );

    // Validar integridad estructural y de concurrencia
    REQUIRE(results.size() == num_queries);
    for (std::size_t q = 0; q < num_queries; ++q) {
        REQUIRE(results[q].query_idx == q);
        REQUIRE(results[q].best_match_idx < num_targets);
        REQUIRE(results[q].max_score >= -1.0f);
        REQUIRE_THAT(results[q].max_score, Catch::Matchers::WithinAbs(0.0f, 1.0001f));    
    }
}
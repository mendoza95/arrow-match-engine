import pyarrow.parquet as pq
import numpy as np
import time
import sys

sys.path.append("./build")
try:
    import matching_engine
    print("✅ Módulo 'matching_engine' cargado exitosamente.")
except ImportError as e:
    print(f"❌ Error al importar matching_engine: {e}")
    sys.exit(1)

PARQUET_FILE = "python/dataset.parquet"
NUM_QUERIES = 5_000
DIM = 128
NUM_THREADS = 8

# 1. Leer la tabla Parquet con PyArrow
print(f"📖 Leyendo datos desde {PARQUET_FILE} usando PyArrow...")
start_io = time.perf_counter()

table = pq.read_table(PARQUET_FILE)
entity_ids = table.column("entity_id").to_pylist()

# Convertir la columna 'embedding' de Arrow a un buffer NumPy contiguo (Zero-Copy)
# .to_numpy() genera la vista 2D sin duplicar memoria
targets_matrix = np.vstack(table.column("embedding").to_numpy()).astype(np.float32)

end_io = time.perf_counter()
print(f"✅ Ingesta de Parquet completada en {(end_io - start_io):.4f} segundos.")
print(f"   Matriz de targets en RAM: {targets_matrix.shape} ({targets_matrix.nbytes / (1024*1024):.2f} MB)")

# 2. Generar consultas de prueba
queries_matrix = np.random.randn(NUM_QUERIES, DIM).astype(np.float32)

# 3. Ejecutar el motor en C++20
print(f"🚀 Ejecutando Matching C++ con {NUM_THREADS} hilos...")
start_match = time.perf_counter()

results = matching_engine.match_batch_parallel(
    queries=queries_matrix,
    targets=targets_matrix,
    num_threads=NUM_THREADS
)

end_match = time.perf_counter()
elapsed_match = end_match - start_match

print(f"✅ Matching completado en {elapsed_match:.4f} segundos.")
print(f"\n--- Ejemplo de Coincidencia ---")
top_result = results[0]
matched_entity = entity_ids[top_result.best_match_idx]
print(f"Query 0 -> Best Match Entity: '{matched_entity}' (Index: {top_result.best_match_idx}) con Score: {top_result.max_score:.4f}")
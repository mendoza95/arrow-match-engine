import numpy as np
import time
import sys

# Asegurar que Python encuentre el módulo compilado en build/
sys.path.append("./build")

try:
    import matching_engine
    print("✅ Módulo 'matching_engine' cargado exitosamente.")
except ImportError as e:
    print(f"❌ Error al importar matching_engine: {e}")
    sys.exit(1)

# Configurar dimensiones
NUM_QUERIES = 1_000
NUM_TARGETS = 10_000
DIM = 128
NUM_THREADS = 8

print(f"Generando arreglos NumPy ({NUM_QUERIES} queries x {NUM_TARGETS} targets, dim={DIM})...")
queries = np.random.randn(NUM_QUERIES, DIM).astype(np.float32)
targets = np.random.randn(NUM_TARGETS, DIM).astype(np.float32)

print("Ejecutando matching vectorial multihilo desde Python...")
start_time = time.perf_counter()

results = matching_engine.match_batch_parallel(
    queries=queries,
    targets=targets,
    num_threads=NUM_THREADS
)

end_time = time.perf_counter()
elapsed = end_time - start_time

print(f"✅ Procesadas {len(results)} consultas en {elapsed:.4f} segundos.")
print(f"Resultado Consulta 0: {results[0]}")
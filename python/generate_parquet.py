import pyarrow as pa
import pyarrow.parquet as pq
import numpy as np
import os

def create_sample_parquet(filename="python/dataset.parquet", num_records=50_000, dim=128):
    print(f"📦 Generando dataset Parquet con {num_records} registros (dim={dim})...")
    
    # Generar vectores aleatorios continuos en memoria
    vectors = np.random.randn(num_records, dim).astype(np.float32)
    entity_ids = [f"ENTITY_{i:06d}" for i in range(num_records)]
    
    # Definir el esquema de Arrow (Lista de tamaño fijo de float32)
    vector_type = pa.list_(pa.float32(), dim)
    
    # Crear Arrays de Arrow
    ids_array = pa.array(entity_ids, type=pa.string())
    # Convertir la matriz 2D de NumPy a una lista de tamaño fijo en Arrow
    flat_vectors = pa.array(vectors.reshape(-1), type=pa.float32())
    vectors_array = pa.FixedSizeListArray.from_arrays(flat_vectors, dim)
    
    # Crear la tabla de Arrow
    table = pa.Table.from_arrays(
        [ids_array, vectors_array],
        names=["entity_id", "embedding"]
    )
    
    # Guardar en disco en formato Parquet
    pq.write_table(table, filename)
    file_size_mb = os.path.getsize(filename) / (1024 * 1024)
    print(f"✅ Archivo '{filename}' creado exitosamente ({file_size_mb:.2f} MB).")

if __name__ == "__main__":
    create_sample_parquet()
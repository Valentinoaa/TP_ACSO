# I301-TP5 ACSO Thread Pool 

El patrón de diseño Thread Pool es una solución de concurrencia que pre-crea y mantiene un número fijo de hilos, listos para ejecutar tareas. Este patrón es fundamental en escenarios donde la creación de hilos es costosa o cuando se requiere un alto grado de control sobre la concurrencia de tareas. En un Thread Pool, las tareas son encoladas y los hilos disponibles del pool las ejecutan. Esto evita el overhead asociado con la creación y destrucción frecuente de hilos, mejorando significativamente el rendimiento en aplicaciones de alto tráfico.

## Archivos

Dentro del directorio **src** van a encontrar los siguientes archivos:

  -  **Semaphore.h/Semaphore.cc**: contiene una implementación de un semáforo hecha por la cátedra.

  -  **Thread-pool.h**:  define la clase ThreadPool.

  -  **Thread-pool.cc**: es el archivo que deberian implementar.
  
  -  **main.cc**: pueden usarlo para generar sus casos de tests.
    
  -  **tptest.cc/tpcustomtest.cc**: son casos de tests un poco mas robustos que pueden usar para probar su codigo.

## Set up

El siguente comando deberian ser capaces de poder compilar todo el proyecto:

    make
    
**Importante:** Cuando quiera compilar con tptest.cc o tpcustomtest.cc van a tener que modificar el Makefile. Mas info en el enunciado del TP en la seccion 4.

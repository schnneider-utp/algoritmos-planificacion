# Planificador de Procesos - Simulador de Algoritmos de Planificación

## Descripción
Este programa simula diferentes algoritmos de planificación de procesos en sistemas operativos, incluyendo tanto algoritmos no expropiativos como expropiativos. Proporciona análisis detallados de los tiempos de espera, retorno y diagramas de Gantt para cada algoritmo.

## Características principales
- Implementa 6 algoritmos de planificación diferentes
- Genera diagramas de Gantt para visualizar la ejecución
- Calcula métricas de rendimiento (tiempos de espera y retorno)
- Interfaz de menú interactivo

## Algoritmos implementados
1. **FCFS** (First Come, First Served)
2. **SPF** (Shortest Process First)
3. **HRRN** (Highest Response Ratio Next)
4. **SPN** (Shortest Process Next)
5. **Round Robin**
6. **Realimentación** (Multilevel Feedback Queue)

## Requisitos del sistema
- Compilador C++ compatible (g++, clang, etc.)
- Sistema operativo Windows/Linux/macOS

## Instalación y ejecución
1. Clona el repositorio o descarga el código fuente:
```bash
git clone https://github.com/schnneider-utp/algoritmos-planificacion
```

## Uso del programa
1. Al iniciar, ingresa el número de procesos a simular
2. Para cada proceso, proporciona:
   - Tiempo de llegada
   - Duración de la ráfaga de CPU

3. Selecciona el algoritmo a ejecutar desde el menú:
   - Opciones del 1 al 6 para algoritmos individuales
   - Opción 7 para ejecutar todos los algoritmos secuencialmente
   - Opción 8 para ingresar nuevos procesos
   - Opción 9 para salir

## Resultados
Para cada algoritmo ejecutado, el programa muestra:
- Tabla detallada con tiempos de cada proceso
- Tiempos promedios de espera y retorno
- Diagrama de Gantt de la ejecución

## Personalización
Puedes modificar:
- El quantum para Round Robin (opción 5)
- Los parámetros de la cola multinivel en Realimentación

## Ejemplo de salida
```
--------------------------------------------------------------------------------
Proceso Llegada Rafaga Inicio Final Espera Retorno
--------------------------------------------------------------------------------
P1      0       5       0      5     0      5
P2      1       3       5      8     4      7
--------------------------------------------------------------------------------
Tiempo promedio de espera: 2
Tiempo promedio de retorno: 6
Duracion total de la ejecucion: 8
--------------------------------------------------------------------------------
```

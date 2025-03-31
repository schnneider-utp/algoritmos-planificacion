#include <iostream>

using namespace std;

// funcionamiento general de los algotirmos --------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Proceso {
    int id, llegada, rafaga, inicio, finalizacion, espera, retorno;
    int tiempoRafaga;
    int tiempoLlegada;
    int tiempoEspera;
    int tiempoFinalizacion;
    int tiempoRetorno;
    int tiempoRafagaRestante; // Para algoritmos expropiativos
    int colaPrioridad[100]; // Arreglo para almacenar las prioridades
    int numPrioridades; // Numero de prioridades almacenadas
    int colaProcesos[100]; // Contador de procesos para validar si se baja la prioridad
};


// Funcion para reiniciar los valores de los procesos
void reiniciarProcesos(Proceso procesos[], int n) {
    for (int i = 0; i < n; i++) {
        procesos[i].tiempoEspera = 0;
        procesos[i].tiempoFinalizacion = 0;
        procesos[i].tiempoRetorno = 0;
        procesos[i].inicio = -1;
        procesos[i].tiempoRafagaRestante = procesos[i].tiempoRafaga;
        // Sincronizar campos duplicados
        procesos[i].espera = 0;
        procesos[i].finalizacion = 0;
        procesos[i].retorno = 0;
    }
}

// Función para copiar un arreglo de procesos
void copiarProcesos(Proceso destino[], Proceso fuente[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = fuente[i];
    }
}

// Función para imprimir los resultados de los algoritmo no expropiativos 
void imprimirResultados(Proceso procesos[], int n, const string& algoritmo, int tiempoTotal) {
    float total_espera = 0, total_retorno = 0;

    cout << "\nResultados " << algoritmo << ":\n";
    cout << "----------------------------------------------------------------\n";
    cout << "Proceso\tLlegada\tRafaga\tInicio\tFinal\tEspera\tRetorno\n";
    cout << "----------------------------------------------------------------\n";
    
    for (int i = 0; i < n; i++) {
        cout << "P" << procesos[i].id << "\t" 
             << procesos[i].llegada << "\t" 
             << procesos[i].rafaga << "\t" 
             << procesos[i].inicio << "\t" 
             << procesos[i].finalizacion << "\t" 
             << procesos[i].espera << "\t" 
             << procesos[i].retorno << endl;
             
        total_espera += procesos[i].espera;
        total_retorno += procesos[i].retorno;
    }
    
    cout << "----------------------------------------------------------------\n";
    cout << "Tiempo promedio de espera: " << total_espera / n << endl;
    cout << "Tiempo promedio de retorno: " << total_retorno / n << endl;
    cout << "Duracion total de la ejecucion: " << tiempoTotal << endl;
    cout << "----------------------------------------------------------------\n";
}

// Función para imprimir los resultados de los algoritmo expropiativos 
void mostrarProcesos(Proceso procesos[], int n) {
    cout << "\nTabla de resultados:\n";
    cout << "----------------------------------------------------------------\n";
    cout << "Proceso\tLlegada\tRafaga\tInicio\tFinaliz\tEspera\tRetorno\n"; // Encabezados
    cout << "----------------------------------------------------------------\n";
    
    float tiempoEsperaPromedio = 0;
    float tiempoRetornoPromedio = 0;
    
    for (int i = 0; i < n; i++) {
        cout << "P" << procesos[i].id << "\t" 
             << procesos[i].tiempoLlegada << "\t" 
             << procesos[i].tiempoRafaga << "\t" 
             << procesos[i].inicio << "\t" 
             << procesos[i].tiempoFinalizacion << "\t" 
             << procesos[i].tiempoEspera << "\t" 
             << procesos[i].tiempoRetorno << endl;
             
        tiempoEsperaPromedio += procesos[i].tiempoEspera;
        tiempoRetornoPromedio += procesos[i].tiempoRetorno;
    }
    
    tiempoEsperaPromedio /= n;
    tiempoRetornoPromedio /= n;
    
    cout << "----------------------------------------------------------------\n";
    cout << "Tiempo de espera promedio: " << tiempoEsperaPromedio << endl;
    cout << "Tiempo de retorno promedio: " << tiempoRetornoPromedio << endl;
    cout << "----------------------------------------------------------------\n";
}

void imprimirDiagramaGantt(Proceso procesos[], int n) {
    cout << "\nDiagrama de Gantt:\n";
    cout << "-------------------------------------------------------------------------------------------------\n";

    // Encontrar el tiempo máximo de finalización para saber cuánto dura la ejecución
    int tiempoMaximo = 0;
    for (int i = 0; i < n; i++) {
        if (procesos[i].finalizacion > tiempoMaximo) {
            tiempoMaximo = procesos[i].finalizacion;
        }
    }

    // Imprimir la línea de tiempo con separadores "|"
    cout << "Time  : ";
    for (int t = 0; t <= tiempoMaximo; t++) {
        cout << (t < 10 ? " " : "") << t << " |";  // Espacio para alineación + separador
    }
    cout << "\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    // Imprimir la ejecución de cada proceso con separadores "|"
    for (int i = 0; i < n; i++) {
        cout << "P" << procesos[i].id << "    : ";
        for (int t = 0; t <= tiempoMaximo; t++) {
            if (t >= procesos[i].inicio && t < procesos[i].finalizacion) {
                cout << " # |"; // Bloque de ejecución con separador
            } else {
                cout << "   |"; // Espacio vacío con separador
            }
        }
        cout << "\n";
    }

    cout << "-------------------------------------------------------------------------------------------------\n";
}

// algoritmos no expropiativos ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void fcfs(Proceso procesos[], int n) {
    // Ordenar procesos por tiempo de llegada para FCFS
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procesos[j].llegada > procesos[j + 1].llegada) {
                swap(procesos[j], procesos[j + 1]);
            }
        }
    }
    
    int tiempo = 0;
    for (int i = 0; i < n; i++) {
        // Asegurarse de que el tiempo actual no sea menor que el tiempo de llegada
        if (tiempo < procesos[i].llegada) {
            tiempo = procesos[i].llegada;
        }
        
        procesos[i].inicio = tiempo;
        procesos[i].finalizacion = tiempo + procesos[i].rafaga;
        procesos[i].espera = procesos[i].inicio - procesos[i].llegada;
        procesos[i].retorno = procesos[i].finalizacion - procesos[i].llegada;
        
        // Actualizar también los campos espejo para compatibilidad
        procesos[i].tiempoFinalizacion = procesos[i].finalizacion;
        procesos[i].tiempoEspera = procesos[i].espera;
        procesos[i].tiempoRetorno = procesos[i].retorno;
        
        tiempo = procesos[i].finalizacion;
    }
    imprimirResultados(procesos, n, "FCFS", tiempo);
    imprimirDiagramaGantt(procesos, n);
}

void spf(Proceso procesos[], int n) {
    bool completado[n];
    for (int i = 0; i < n; i++) completado[i] = false;

    int tiempo = 0, completados = 0;
    while (completados < n) {
        int idx = -1;
        int menor_rafaga = 999999; // Valor inicial grande

        for (int i = 0; i < n; i++) {
            if (!completado[i] && procesos[i].llegada <= tiempo && procesos[i].rafaga < menor_rafaga) {
                menor_rafaga = procesos[i].rafaga;
                idx = i;
            }
        }

        if (idx == -1) {
            // No hay procesos disponibles, buscar el siguiente tiempo de llegada
            int siguiente_llegada = 999999;
            for (int i = 0; i < n; i++) {
                if (!completado[i] && procesos[i].llegada < siguiente_llegada) {
                    siguiente_llegada = procesos[i].llegada;
                }
            }
            
            if (siguiente_llegada != 999999) {
                tiempo = siguiente_llegada;
            } else {
                tiempo++;
            }
            continue;
        }

        procesos[idx].inicio = tiempo;
        procesos[idx].finalizacion = tiempo + procesos[idx].rafaga;
        procesos[idx].espera = procesos[idx].inicio - procesos[idx].llegada;
        procesos[idx].retorno = procesos[idx].finalizacion - procesos[idx].llegada;
        
        // Actualizar también los campos espejo para compatibilidad
        procesos[idx].tiempoFinalizacion = procesos[idx].finalizacion;
        procesos[idx].tiempoEspera = procesos[idx].espera;
        procesos[idx].tiempoRetorno = procesos[idx].retorno;

        tiempo = procesos[idx].finalizacion;
        completado[idx] = true;
        completados++;
    }
    imprimirResultados(procesos, n, "SPF", tiempo);
    imprimirDiagramaGantt(procesos, n);
}

void hrrn(Proceso procesos[], int n) {
    bool procesado[n];
    for (int i = 0; i < n; i++) procesado[i] = false;

    int tiempo = 0, completados = 0;
    while (completados < n) {
        int idx = -1;
        float max_ratio = -1;

        for (int i = 0; i < n; i++) {
            if (!procesado[i] && procesos[i].llegada <= tiempo) {
                float response_ratio = (float)(tiempo - procesos[i].llegada + procesos[i].rafaga) / procesos[i].rafaga;
                if (response_ratio > max_ratio) {
                    max_ratio = response_ratio;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // No hay procesos disponibles, buscar el siguiente tiempo de llegada
            int siguiente_llegada = 999999;
            for (int i = 0; i < n; i++) {
                if (!procesado[i] && procesos[i].llegada < siguiente_llegada) {
                    siguiente_llegada = procesos[i].llegada;
                }
            }
            
            if (siguiente_llegada != 999999) {
                tiempo = siguiente_llegada;
            } else {
                tiempo++;
            }
            continue;
        }

        procesos[idx].inicio = tiempo;
        procesos[idx].finalizacion = tiempo + procesos[idx].rafaga;
        procesos[idx].espera = procesos[idx].inicio - procesos[idx].llegada;
        procesos[idx].retorno = procesos[idx].finalizacion - procesos[idx].llegada;
        
        // Actualizar también los campos espejo para compatibilidad
        procesos[idx].tiempoFinalizacion = procesos[idx].finalizacion;
        procesos[idx].tiempoEspera = procesos[idx].espera;
        procesos[idx].tiempoRetorno = procesos[idx].retorno;

        tiempo = procesos[idx].finalizacion;
        procesado[idx] = true;
        completados++;
    }
    imprimirResultados(procesos, n, "HRRN", tiempo);
    imprimirDiagramaGantt(procesos, n);
}

void spn(Proceso procesos[], int n) {
    bool* completado = new bool[n]; // Arreglo dinámico para procesos completados
    for (int i = 0; i < n; i++) completado[i] = false;

    int tiempo = 0, completados = 0;
    float total_espera = 0, total_retorno = 0;

    while (completados < n) {
        int idx = -1;
        int menor_rafaga = 999999;

        // Buscar el proceso con la ráfaga más corta que haya llegado
        for (int i = 0; i < n; i++) {
            if (!completado[i] && procesos[i].llegada <= tiempo && procesos[i].rafaga < menor_rafaga) {
                menor_rafaga = procesos[i].rafaga;
                idx = i;
            }
        }

        if (idx == -1) {
            // No hay procesos disponibles, buscar el siguiente tiempo de llegada
            int siguiente_llegada = 999999;
            for (int i = 0; i < n; i++) {
                if (!completado[i] && procesos[i].llegada < siguiente_llegada) {
                    siguiente_llegada = procesos[i].llegada;
                }
            }
            
            if (siguiente_llegada != 999999) {
                tiempo = siguiente_llegada;
            } else {
                tiempo++;
            }
            continue;
        }

        // Ejecutar el proceso seleccionado
        procesos[idx].inicio = tiempo;
        procesos[idx].finalizacion = tiempo + procesos[idx].rafaga;
        procesos[idx].espera = procesos[idx].inicio - procesos[idx].llegada;
        procesos[idx].retorno = procesos[idx].finalizacion - procesos[idx].llegada;
        
        // Actualizar también los campos espejo para compatibilidad
        procesos[idx].tiempoFinalizacion = procesos[idx].finalizacion;
        procesos[idx].tiempoEspera = procesos[idx].espera;
        procesos[idx].tiempoRetorno = procesos[idx].retorno;

        tiempo = procesos[idx].finalizacion;
        completado[idx] = true;
        completados++;

        total_espera += procesos[idx].espera;
        total_retorno += procesos[idx].retorno;
    }

    delete[] completado; // Liberar memoria
    imprimirResultados(procesos, n, "SPN", tiempo);
    imprimirDiagramaGantt(procesos, n);
}

// algoritmos expropiativos -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void RoundRobin(Proceso procesos[], int n, int quantum) {
    cout << "\n----- Round Robin (Quantum = " << quantum << ") -----\n";
    
    reiniciarProcesos(procesos, n);
    int tiempoActual = 0;
    int completados = 0;
    int tiemposRafagaRestantes[n]; // Tiempo de rafaga restante para cada proceso
    bool procesoIniciado[n];      // Indica si un proceso ya ha comenzado a ejecutarse
    
    // Matriz para almacenar el diagrama de Gantt
    char gantt[100][100]; // Asumimos un maximo de 100 procesos y 100 unidades de tiempo

    // Inicializar la matriz con espacios en blanco
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 100; j++) {
            gantt[i][j] = ' ';
        }
    }

    // Inicializar los tiempos de rafaga restantes y el estado de inicio de los procesos
    for (int i = 0; i < n; i++) {
        tiemposRafagaRestantes[i] = procesos[i].tiempoRafaga;
        procesoIniciado[i] = false; // Ningun proceso ha comenzado
    }

    // Bucle principal: ejecutar hasta que todos los procesos hayan terminado
    while (completados < n) {
        bool procesoEjecutado = false;

        // Iterar sobre todos los procesos
        for (int i = 0; i < n; i++) {
            // Verificar si el proceso esta listo para ejecutarse
            if (procesos[i].tiempoLlegada <= tiempoActual && tiemposRafagaRestantes[i] > 0) {
                procesoEjecutado = true;

                // Registrar el inicio si es la primera vez que el proceso se ejecuta
                if (!procesoIniciado[i]) {
                    procesos[i].inicio = tiempoActual;
                    procesoIniciado[i] = true;
                }

                // Calcular el tiempo a ejecutar (quantum o lo que quede del proceso)
                int tiempoEjecucion = min(quantum, tiemposRafagaRestantes[i]);
                
                // Ejecutar el proceso por el tiempo calculado
                for (int j = 0; j < tiempoEjecucion; j++) {
                    gantt[i][tiempoActual + j] = '#';
                }
                
                tiemposRafagaRestantes[i] -= tiempoEjecucion;
                tiempoActual += tiempoEjecucion;

                // Si el proceso ha terminado, calcular sus tiempos
                if (tiemposRafagaRestantes[i] == 0) {
                    completados++;
                    procesos[i].tiempoFinalizacion = tiempoActual;
                    procesos[i].tiempoRetorno = procesos[i].tiempoFinalizacion - procesos[i].tiempoLlegada;
                    procesos[i].tiempoEspera = procesos[i].tiempoRetorno - procesos[i].tiempoRafaga;
                }
            }
        }

        // Si no se ejecuto ningun proceso en este ciclo, avanzar el tiempo
        if (!procesoEjecutado) {
            tiempoActual++;
        }
    }
    
    cout << "\nDiagrama de Gantt (Round Robin):\n";
    // Mostrar la linea de tiempo
    cout << "Tiempo:  ";
    for (int j = 0; j < tiempoActual + 1; j++) {
        cout << j << "  ";
    }
    cout << endl;

    // Mostrar el diagrama de Gantt para cada proceso
    for (int i = 0; i < n; i++) {
        cout << "Proceso " << procesos[i].id << ": ";
        for (int j = 0; j < tiempoActual; j++) {
            cout << "|" << gantt[i][j] << "|";
        }
        cout << endl;
    }
    
    mostrarProcesos(procesos, n);
}

// Funcion para calcular el tiempo de espera y finalizacion en Retroalimentacion (Multilevel Feedback Queue)
void Realimentacion(Proceso procesos[], int n) {
    cout << "\n----- Realimentacion (Multilevel Feedback Queue) -----\n";
    
    reiniciarProcesos(procesos, n);
    int tiempoActual = 0;
    int completados = 0;
    int tiemposRafagaRestantes[n]; // Tiempo de ráfaga restante para cada proceso
    bool procesoIniciado[n];      // Indica si un proceso ya ha comenzado a ejecutarse
    int contadorInicios = 0;      // Contador de inicios de procesos

    // Inicializar los tiempos de ráfaga restantes y el estado de inicio de los procesos
    for (int i = 0; i < n; i++) {
        tiemposRafagaRestantes[i] = procesos[i].tiempoRafaga;
        procesoIniciado[i] = false; // Ningun proceso ha comenzado
        procesos[i].numPrioridades = 0; // Inicializar el contador de prioridades
    }

    // Crear una matriz para el diagrama de Gantt
    const int maxTiempo = 100; // Máximo tiempo asumido
    char gantt[n][maxTiempo];  // Matriz para el diagrama de Gantt

    // Inicializar la matriz con espacios en blanco
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < maxTiempo; j++) {
            gantt[i][j] = ' ';
        }
    }

    // Bucle principal: ejecutar hasta que todos los procesos hayan terminado
    while (completados < n) {
        bool procesoEjecutado = false;

        // Incrementar el contador de inicios si un proceso alcanza su tiempo de inicio
        for (int i = 0; i < n; i++) {
            if (procesos[i].tiempoLlegada == tiempoActual && !procesoIniciado[i]) {
                contadorInicios++;
                procesoIniciado[i] = true;
                procesos[i].inicio = tiempoActual;
                cout << "Contador de inicios aumento a: " << contadorInicios << endl;
            }
        }

        // Buscar el proceso listo con el menor numero de prioridades
        int idx = -1; // indice del proceso seleccionado
        int minPrioridades = 999999; // Minimo numero de prioridades

        for (int i = 0; i < n; i++) {
            if (procesos[i].tiempoLlegada <= tiempoActual && tiemposRafagaRestantes[i] > 0) {
                if (procesos[i].numPrioridades < minPrioridades) {
                    minPrioridades = procesos[i].numPrioridades;
                    idx = i;
                }
            }
        }

        // Si se encontro un proceso listo, ejecutarlo
        if (idx != -1) {
            procesoEjecutado = true;

            // Verificar cuantos procesos estan listos para ejecutarse
            int procesosListos = 0;
            for (int i = 0; i < n; i++) {
                if (procesos[i].tiempoLlegada < tiempoActual && tiemposRafagaRestantes[i] > 0) {
                    procesosListos++;
                }
            }

            // Incrementar el numero de prioridades solo si hay mas de un proceso listo
            if (procesosListos < contadorInicios) {
                procesos[idx].numPrioridades++;
                cout << "Proceso " << procesos[idx].id << " tiene ahora " << procesos[idx].numPrioridades << " prioridades." << endl;
            }
            
            // Ejecutar el proceso por un quantum de 1 unidad de tiempo
            tiemposRafagaRestantes[idx]--;
            gantt[idx][tiempoActual] = '#'; // Marcar en el diagrama de Gantt
            tiempoActual++;

            // Si el proceso ha terminado, calcular sus tiempos
            if (tiemposRafagaRestantes[idx] == 0) {
                completados++;
                procesos[idx].tiempoFinalizacion = tiempoActual;
                procesos[idx].tiempoRetorno = procesos[idx].tiempoFinalizacion - procesos[idx].tiempoLlegada;
                procesos[idx].tiempoEspera = procesos[idx].tiempoFinalizacion - procesos[idx].tiempoLlegada - procesos[idx].tiempoRafaga;
            }
        }

        // Si no se ejecuto ningun proceso en este ciclo, avanzar el tiempo
        if (!procesoEjecutado) {
            tiempoActual++;
        }
    }

    // Imprimir el diagrama de Gantt
    cout << "\nDiagrama de Gantt (Realimentacion):" << endl;
    cout << "Tiempo:  ";
    for (int j = 0; j < tiempoActual; j++) {
        cout << j << "  ";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << "Proceso " << procesos[i].id << ": ";
        for (int j = 0; j < tiempoActual; j++) {
            cout << "|" << gantt[i][j] << "|";
        }
        cout << endl;
    }

    // Imprimir la cola de prioridad de cada proceso
    cout << "\nCola de prioridad de cada proceso:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Proceso " << procesos[i].id << ": ";
        for (int j = 0; j < procesos[i].numPrioridades; j++) {
            cout << procesos[i].colaPrioridad[j] << " ";
        }
        cout << endl;
    }
    
    mostrarProcesos(procesos, n);
}

void ejecutarTodos(Proceso procesos[], Proceso procesosOriginal[], int n) {
    // FCFS
    copiarProcesos(procesos, procesosOriginal, n);
    fcfs(procesos, n);
    
    // SPF
    copiarProcesos(procesos, procesosOriginal, n);
    spf(procesos, n);
    
    // HRRN
    copiarProcesos(procesos, procesosOriginal, n);
    hrrn(procesos, n);
    
    // SPN
    copiarProcesos(procesos, procesosOriginal, n);
    spn(procesos, n);
    
    // Round Robin (quantum = 1)
    copiarProcesos(procesos, procesosOriginal, n);
    RoundRobin(procesos, n, 1);
    
    // Realimentacion
    copiarProcesos(procesos, procesosOriginal, n);
    Realimentacion(procesos, n);
}

// Menú principal
void menu() {
    int n;
    cout << "\nIngrese el numero de procesos: ";
    cin >> n;
    
    Proceso procesos[100]; // Maximo 100 procesos
    Proceso procesosOriginal[100]; // Copia de los procesos originales
    
    // Solicitar datos de procesos
    for (int i = 0; i < n; i++) {
        procesos[i].id = i + 1;
        procesos[i].inicio = -1; // Inicializar inicio
        cout << "Ingrese tiempo de llegada y rafaga para P" << i + 1 << ": ";
        cin >> procesos[i].tiempoLlegada >> procesos[i].tiempoRafaga;
        
        // Sincronizar los campos duplicados
        procesos[i].llegada = procesos[i].tiempoLlegada;
        procesos[i].rafaga = procesos[i].tiempoRafaga;
        
        procesos[i].tiempoRafagaRestante = procesos[i].tiempoRafaga; // Inicializar tiempo restante
        procesos[i].tiempoEspera = 0;
        procesos[i].tiempoFinalizacion = 0;
        procesos[i].tiempoRetorno = 0;
        procesos[i].espera = 0;
        procesos[i].finalizacion = 0;
        procesos[i].retorno = 0;
        procesos[i].numPrioridades = 0;
    }
    
    // Guardar copia de los procesos originales
    copiarProcesos(procesosOriginal, procesos, n);
    
    cout << "-----------------------------------------------------------------------------------------------";
    
    int opcion;
    do {
        cout << "\n1. FCFS\n2. SPF\n3. HRRN\n4. SPN\n5. Round Robin\n6. Realimentacion\n7. Todos\n8. nuevos procesos a calcular\n9. Salir\nSeleccione: ";
        cin >> opcion;
        cout << "-----------------------------------------------------------------------------------------------";
        switch (opcion) {
            case 1:
                copiarProcesos(procesos, procesosOriginal, n);
                fcfs(procesos, n);
                break;
            case 2:
                copiarProcesos(procesos, procesosOriginal, n);
                spf(procesos, n);
                break;
            case 3:
                copiarProcesos(procesos, procesosOriginal, n);
                hrrn(procesos, n);
                break;
            case 4:
                copiarProcesos(procesos, procesosOriginal, n);
                spn(procesos, n);
                break;
            case 5: {
                int q;
                cout << "\nIngrese el quantum: ";
                cin >> q;
                copiarProcesos(procesos, procesosOriginal, n);
                RoundRobin(procesos, n, q);
                break;
            }
            case 6:
                copiarProcesos(procesos, procesosOriginal, n);
                Realimentacion(procesos, n);
                break;
            case 7:
                ejecutarTodos(procesos, procesosOriginal, n);
                break;
            case 8:
                menu();
            	break;
            case 9:
            	cout << "\nSaliendo...\n";
                break;
            default:
                cout << "\nOpcion invalida.\n";
                opcion = 9;
        }
    } while (opcion != 9);

}

int main() {
    menu();
    return 0;
}
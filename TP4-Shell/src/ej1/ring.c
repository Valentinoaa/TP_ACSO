#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{	
	int start, status, pid, n;
	int buffer[1];

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
    /* Parsing of arguments */
    n = atoi(argv[1]);          // número de procesos
    buffer[0] = atoi(argv[2]);  // caracter/valor a enviar
    start = atoi(argv[3]);      // proceso que inicia
    
    // Verificar argumentos válidos
    if (n <= 0 || start < 0 || start >= n) {
        printf("Error: argumentos inválidos\n");
        exit(1);
    }
    
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer[0], start);
    
    // Crear arrays para pipes - cada proceso tiene un pipe
    int pipes[n][2];
    
    // Crear todos los pipes
    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    
    // Crear los procesos del anillo
    for (int i = 0; i < n; i++) {
        pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        
        if (pid == 0) {  // Proceso hijo
            // Cada proceso lee de su pipe i y escribe al pipe (i+1)%n
            int next = (i + 1) % n;
            
            // Cerrar todos los extremos que no necesito
            for (int j = 0; j < n; j++) {
                if (j == i) {
                    // Mi pipe de lectura - mantener extremo de lectura
                    close(pipes[j][1]);
                } else if (j == next) {
                    // Pipe del siguiente proceso - mantener extremo de escritura
                    close(pipes[j][0]);
                } else {
                    // Otros pipes - cerrar ambos extremos
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }
            
            int value;
            
            if (i == start) {
                // Proceso que inicia: envía el valor inicial y luego lee
                printf("Proceso %d: enviando valor inicial %d\n", i, buffer[0]);
                if (write(pipes[next][1], buffer, sizeof(int)) == -1) {
                    perror("write inicial");
                    exit(1);
                }
                
                // Ahora espera a que le llegue de vuelta el valor
                if (read(pipes[i][0], &value, sizeof(int)) == -1) {
                    perror("read final");
                    exit(1);
                }
                printf("Proceso %d: recibí de vuelta el valor %d - fin del anillo\n", i, value);
            } else {
                // Otros procesos: leen y reenvían
                if (read(pipes[i][0], &value, sizeof(int)) == -1) {
                    perror("read");
                    exit(1);
                }
                printf("Proceso %d: recibí valor %d, reenviando\n", i, value);
                
                if (write(pipes[next][1], &value, sizeof(int)) == -1) {
                    perror("write");
                    exit(1);
                }
            }
            
            // Cerrar mis pipes
            close(pipes[i][0]);
            close(pipes[next][1]);
            exit(0);
        }
    }
    
    // Proceso padre: cerrar todos los pipes y esperar a los hijos
    for (int i = 0; i < n; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Esperar a que terminen todos los procesos hijos
    for (int i = 0; i < n; i++) {
        wait(&status);
    }
    
    printf("Comunicación en anillo completada\n");
    return 0;
}

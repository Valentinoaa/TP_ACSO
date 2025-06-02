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
    
    n = atoi(argv[1]);
    buffer[0] = atoi(argv[2]);
    start = atoi(argv[3]);
    
    if (n <= 0 || start < 0 || start >= n) {
        printf("Error: argumentos inválidos\n");
        exit(1);
    }
    
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer[0], start);
    
    int pipes[n][2];
    
    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    
    for (int i = 0; i < n; i++) {
        pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        
        if (pid == 0) {
            int next = (i + 1) % n;
            
            for (int j = 0; j < n; j++) {
                if (j == i) {
                    close(pipes[j][1]);
                } else if (j == next) {
                    close(pipes[j][0]);
                } else {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }
            
            int value;
            
            if (i == start) {
                printf("Proceso %d: enviando valor inicial %d\n", i, buffer[0]);
                if (write(pipes[next][1], buffer, sizeof(int)) == -1) {
                    perror("write inicial");
                    exit(1);
                }
                
                if (read(pipes[i][0], &value, sizeof(int)) == -1) {
                    perror("read final");
                    exit(1);
                }
                printf("Proceso %d: recibí de vuelta el valor %d - fin del anillo\n", i, value);
            } else {
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
            
            close(pipes[i][0]);
            close(pipes[next][1]);
            exit(0);
        }
    }
    
    for (int i = 0; i < n; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    for (int i = 0; i < n; i++) {
        wait(&status);
    }
    
    printf("Comunicación en anillo completada\n");
    return 0;
}

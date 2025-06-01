#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 201
#define MAX_ARGS 64

void process_quotes(char* str) {
    char* src = str;
    char* dst = str;
    int in_double_quotes = 0;
    int in_single_quotes = 0;
    
    while (*src) {
        if (*src == '"' && !in_single_quotes) {
            in_double_quotes = !in_double_quotes;
            src++;
        } else if (*src == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

int check_balanced_quotes(char* command) {
    int double_quotes = 0;
    int single_quotes = 0;
    
    for (char* p = command; *p; p++) {
        if (*p == '"' && single_quotes == 0) {
            double_quotes = !double_quotes;
        } else if (*p == '\'' && double_quotes == 0) {
            single_quotes = !single_quotes;
        }
    }
    
    return (double_quotes == 0 && single_quotes == 0);
}

int parse_args(char* command, char** args) {
    int argc = 0;
    char* start = command;
    char* end = command;
    int in_double_quotes = 0;
    int in_single_quotes = 0;
    
    while (*start == ' ' || *start == '\t') start++;
    
    if (*start == '\0') return 0;
    
    end = start;
    
    while (*end && argc < MAX_ARGS - 1) {
        while (*end) {
            if (*end == '"' && !in_single_quotes) {
                in_double_quotes = !in_double_quotes;
            } else if (*end == '\'' && !in_double_quotes) {
                in_single_quotes = !in_single_quotes;
            } else if ((*end == ' ' || *end == '\t') && !in_double_quotes && !in_single_quotes) {
                break;
            }
            end++;
        }
        
        int len = end - start;
        char* arg = malloc(len + 1);
        strncpy(arg, start, len);
        arg[len] = '\0';
        
        process_quotes(arg);
        
        args[argc++] = arg;
        
        while (*end == ' ' || *end == '\t') end++;
        start = end;
        
        if (*end == '\0') break;
    }
    
    while (*end == ' ' || *end == '\t') end++;
    if (*end != '\0' && argc >= MAX_ARGS - 1) {
        args[argc] = NULL;
        return -1;
    }
    
    args[argc] = NULL;
    return argc;
}

void free_args(char** args, int argc) {
    for (int i = 0; i < argc; i++) {
        if (args[i]) free(args[i]);
    }
}

int validate_pipe_syntax(char** commands, int command_count) {
    for (int i = 0; i < command_count; i++) {
        char* cmd = commands[i];
        while (*cmd == ' ' || *cmd == '\t') cmd++;
        
        if (*cmd == '\0') {
            return 0;
        }
    }
    return 1;
}

int main() {
    char command[4096];
    char *commands[MAX_COMMANDS];
    int command_count = 0;

    while (1) 
    {
        if (isatty(STDIN_FILENO)) {
            printf("Shell> ");
        }
        
        /*Reads a line of input from the user from the standard input (stdin) and stores it in the variable command */
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // EOF
        }
        
        /* Removes the newline character (\n) from the end of the string stored in command, if present. 
           This is done by replacing the newline character with the null character ('\0').
           The strcspn() function returns the length of the initial segment of command that consists of 
           characters not in the string specified in the second argument ("\n" in this case). */
        command[strcspn(command, "\n")] = '\0';
        
        if (strlen(command) == 0) {
            continue;
        }
        
        if (!check_balanced_quotes(command)) {
            fprintf(stderr, "Error: unbalanced quotes\n");
            continue;
        }

        command_count = 0;
        
        /* Tokenizes the command string using the pipe character (|) as a delimiter using the strtok() function. 
           Each resulting token is stored in the commands[] array. 
           The strtok() function breaks the command string into tokens (substrings) separated by the pipe character |. 
           In each iteration of the while loop, strtok() returns the next token found in command. 
           The tokens are stored in the commands[] array, and command_count is incremented to keep track of the number of tokens found. */
        
        if (command[0] == '|') {
            fprintf(stderr, "Error: pipe at beginning of command\n");
            continue;
        }
        
        int len = strlen(command);
        if (len > 0 && command[len-1] == '|') {
            fprintf(stderr, "Error: pipe at end of command\n");
            continue;
        }
        
        if (strstr(command, "||") != NULL) {
            fprintf(stderr, "Error: double pipe\n");
            continue;
        }
        
        char *token = strtok(command, "|");
        while (token != NULL && command_count < MAX_COMMANDS) 
        {
            commands[command_count++] = token;
            token = strtok(NULL, "|");
        }

        if (!validate_pipe_syntax(commands, command_count)) {
            fprintf(stderr, "Error: empty command in pipeline\n");
            continue;
        }

        /* You should start programming from here... */
        
        if (command_count == 1) {
            char* args[MAX_ARGS];
            int argc = parse_args(commands[0], args);
            if (argc == -1) {
                free_args(args, MAX_ARGS - 1);
                fprintf(stderr, "Error: too many arguments\n");
                continue;
            }
            if (argc > 0 && strcmp(args[0], "exit") == 0) {
                free_args(args, argc);
                break;
            }
            free_args(args, argc);
        }
        
        if (command_count == 1) {
            char* args[MAX_ARGS];
            int argc = parse_args(commands[0], args);
            
            if (argc == -1) {
                free_args(args, MAX_ARGS - 1);
                fprintf(stderr, "Error: too many arguments\n");
                continue;
            }
            
            if (argc > 0) {
                pid_t pid = fork();
                
                if (pid == 0) {
                    execvp(args[0], args);
                    perror("execvp failed");
                    exit(1);
                } else if (pid > 0) {
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    perror("fork failed");
                }
            }
            
            free_args(args, argc);
        }
        else if (command_count > 1) {
            int pipes[command_count - 1][2];
            pid_t pids[command_count];
            int all_processes_created = 1;
            
            for (int i = 0; i < command_count - 1; i++) {
                if (pipe(pipes[i]) == -1) {
                    perror("pipe failed");
                    for (int j = 0; j < i; j++) {
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }
                    goto next_command;
                }
            }
            
            for (int i = 0; i < command_count; i++) {
                char* args[MAX_ARGS];
                int argc = parse_args(commands[i], args);
                
                if (argc == -1) {
                    free_args(args, MAX_ARGS - 1);
                    fprintf(stderr, "Error: too many arguments in command %d\n", i+1);
                    all_processes_created = 0;
                    break;
                }
                
                if (argc == 0) {
                    free_args(args, argc);
                    continue;
                }
                
                pids[i] = fork();
                
                if (pids[i] == 0) {
                    if (strcmp(args[0], "exit") == 0) {
                        if (i < command_count - 1) {
                            close(pipes[i][1]);
                        }
                        exit(0);
                    }
                    
                    if (i > 0) {
                        dup2(pipes[i-1][0], STDIN_FILENO);
                    }
                    
                    if (i < command_count - 1) {
                        dup2(pipes[i][1], STDOUT_FILENO);
                    }
                    
                    for (int j = 0; j < command_count - 1; j++) {
                        if (i > 0 && j == i-1) {
                            close(pipes[j][1]);
                        } else if (i < command_count - 1 && j == i) {
                            close(pipes[j][0]);
                        } else {
                            close(pipes[j][0]);
                            close(pipes[j][1]);
                        }
                    }
                    
                    execvp(args[0], args);
                    perror("execvp failed");
                    exit(1);
                    
                } else if (pids[i] < 0) {
                    perror("fork failed");
                    all_processes_created = 0;
                    break;
                }
                
                free_args(args, argc);
            }
            
            if (all_processes_created) {
                for (int i = 0; i < command_count - 1; i++) {
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
                
                for (int i = 0; i < command_count; i++) {
                    int status;
                    waitpid(pids[i], &status, 0);
                }
            } else {
                for (int i = 0; i < command_count - 1; i++) {
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
                for (int i = 0; i < command_count; i++) {
                    if (pids[i] > 0) {
                        int status;
                        waitpid(pids[i], &status, 0);
                    }
                }
            }
        }
        
        next_command:
        command_count = 0;
    }
    return 0;
}


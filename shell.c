#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void parse_comm(char *command, char **args){
    int i = 0;
    char *token = strtok(command, " ");
    while(token != NULL && i < MAX_ARGS - 1){
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void execute_comm(char **args){
    pid_t pid = fork();
    if(pid < 0){
        perror("Fork error");
    } else if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
    } else{
        int status;
        waitpid(pid, &status, 0);
    }
}

int main(){
    char command[MAX_LINE];
    char *args[MAX_ARGS];
    
    while(1){
        printf("Shell> ");
        fflush(stdout);
        
        if(fgets(command, MAX_LINE, stdin) == NULL){
            perror("fgets error");
            continue;
        }
        
        command[strcspn(command, "\n")] = 0;
        
        parse_comm(command, args);
        
        if(args[0] == NULL){
            continue;
        }
        
        if(strcmp(args[0], "exit") == 0){
            break;
        }
        
        execute_comm(args);
    }
    
    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define BSIZE 20

/***********************************************************************
 * This starter shell implements a simple shell that implements:
 * 
 * (1) An internal EXIT command that exits the shell.
 * (2) Executes a external program without any command line arguments
 * 
 ***********************************************************************/
int main(int argc, char *argv[]){
    char buffer[BSIZE];
    pid_t child;
    int status;
   // char *args[]={"",NULL};
    char *args[10];


    //prevents Ctrl+C from killing shell
    signal(SIGINT, SIG_IGN);

    while(1){
        printf("shell> ");
        fflush(stdout);
        fgets(buffer,BSIZE-1,stdin);
        buffer[(strlen(buffer)-1)]='\0';  //Remove NewLine
        //args[0]=buffer;
        //Parse command line arguments into the *args[10]
         int i = 0;
        args[i] = strtok(buffer, " ");

        while(args[i] != NULL && i < 9){
            i++;
            args[i] = strtok(NULL, " ");
        }

        if(args[0] == NULL){
            continue;
        }

        //** Internal Command **
        if(strcmp(args[0],"exit")==0){
            break;
        }

         if(strcmp(args[0],"cd")==0){
            if(args[1] == NULL){
                chdir(getenv("HOME"));
            }
            else if(chdir(args[1]) != 0){
                perror("cd");
            }
            continue;
        }


        //** External Command **
        child = fork();
        if(child==0){
            //*** Child Process ***

            //allow Ctrl+C to interupt the child not the shell itself. 
            signal(SIGINT, SIG_DFL);

            execvp(args[0],args);
            perror("execvp");
            exit(1);
        }else if (child > 0){
            //*** Parent Process **
            waitpid(child,&status,0);
        }else{
            perror("fork");
        }

    }
    return 0;
}
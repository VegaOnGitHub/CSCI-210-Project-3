#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12
#define MAX_ARGS 20 // argument limits

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise

	//check if the command matches with one of the 12 char arrays commands.
	for (int i = 0; i < N; i++) {
		if(strcmp(cmd, allowed[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
	char *argv[MAX_ARGS+2]; // 20 arguments + command + NULL terminator
	pid_t pid; // initialize process id
	int status;

    while (1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';

		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c

		// tokenize into argv
		int argc = 0;
		char *token = strtok(line, " ");

		while (token != NULL && argc < MAX_ARGS + 1) {
			argv[argc++] = token;
			token = strtok(NULL, " ");
		}

		argv[argc] = NULL;

		if (argc == 0) continue;

		// check if given command is allowed
		if(!isAllowed(argv[0])) {
			printf("NOT ALLOWED!\n");
			continue;
		}

		// exit command
		if (strcmp(argv[0], "exit") == 0) {
			return 0;
		}

		// help command
		if (strcmp(argv[0], "help") == 0) {
			printf("The allowed commands are:\n");
			for (int i = 0; i < N; i++){
				printf("%d: %s\n", i + 1, allowed[i]);
			}
			continue;
		}

		// cd command
		if (strcmp(argv[0], "cd") == 0) {
			if (argc > 2){
				printf("-rsh: cd: too many arguments\n");
			} else if (argc == 2) {
				chdir(argv[1]);
			}
		// argc == 1 (just the "cd" command alone) does nothing
		continue;
		}

		//spawn external command (first 9 commands)
		if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) == 0){
			waitpid(pid, &status, 0);
		} else {
			perror("spawn failed");
		}
    }
    return 0;
}

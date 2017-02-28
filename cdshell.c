#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
//Chase Deets
//chd5hq@virginia.edu
//CS 4414-001 - Project 1: Implementing a Shell in C
//currently compiled with: gcc -lreadline -std=c11 cdshell.c -o cdshell

//returns an int after populating an array of token groups
//input: line from commandline, array of strings for arguments
int parseTok(char* cmdline, char** args)
{
	int numArgs = 0;
	char s[64];
	strcpy(s, cmdline);
	char *tok;

	if(!tok){
		printf("ERROR: failed to tokenize input, exiting...\n");
		return -1;
	}

	tok = strtok(s, "|");
	while(tok != NULL) {
		printf("tok: %s\n", tok);
		args[numArgs] = malloc(strlen(tok)+1);
		strcpy(args[numArgs], tok);
		printf("arg: %s - ", args[numArgs]);
		//args[numArgs] = strdup(tok);
		//args[numArgs] = strcpy(args[numArgs], tok);
		//printf("added arg: %s\n", args[numArgs]);
		numArgs++;
		
		printf("%d\n", numArgs);
		tok = strtok(NULL, "|"); //pass null to continue strtok on tok
		
	}
	//args[numArgs] = NULL;
	printf("done, returning args..");
	return numArgs;
}

int validateCommand(char *argument, char **parameters, int numIns, int numOuts){
	char temp[64];
	char *par;
	int p;
	strcpy(temp, argument);
	printf("validating: %s\n", temp);
	par = strtok(temp, " ");
	p = 0;
	while(par != 0){
		parameters[p] = malloc(strlen(par)+1);			
		strcpy(parameters[p], par);
		printf("parameter[%d]:%s\n", p, parameters[p]);
		par = strtok(NULL, " ");
		p++;
	}
	parameters[p] = '\0';
	for(int i = 0; i < p; i++){
		if(strcmp(parameters[i], "<") == 0) 
			numIns++;
		if(strcmp(parameters[i], ">") == 0)
			numOuts++;
	}
	if(numIns >= 2)
		return -1;
	else if(numOuts >= 2)
		return -2;
	else
		return 0;
}


int validateToken(char **tokens, int numToks){
	
	return 0;
}


int executeCommand(char **cmdArgs){
	printf("executing %s\n", cmdArgs[0]);
	//char *testArgs[5] = {"ls", "-l", NULL};
	pid_t pid;

	//fork process
	pid = fork();
	
	if(pid < 0){
		fprintf(stderr, "ERROR: fork failed\n");
		return 1;
	}
	else if(pid == 0) {
		execvp(cmdArgs[0], cmdArgs);
		return 0;
	}
	else{
		wait(NULL);
		printf("Child has executed process!\n");
		return 0;
	}
	return 0;
}



int main(int argc, char** argv)
{
	printf("- Starting Shell...\n");
	printf("-- CDShell V0.1 --\n");
	
	int numTokenGroups;
	int numPipes;
	int numInputRedirects;
	int numOutRedirects;
	int execStatus;
	int validCmd;
	int validToks;
	char *inpt; //input from user
	char command[64];
	//char **tokenGroup = malloc(256 * sizeof(char*));
	char **tokenGroup; 	//collection of tokens
	char *params[64];	//token --> Params to be executed 
	

	while(1)
	{
		inpt = readline(">>> ");
		add_history(inpt);	//allows for repeating previous cmds
		
		if((strcmp(inpt, "exit") == 0) || (strcmp(inpt, "Exit") == 0)){
			printf("Exiting shell... Ciao!\n");			
			exit(0);
		}
		
		
		//populate token group with input line
		numTokenGroups = parseTok(inpt, tokenGroup);
		//token group with tokens
		numPipes = numTokenGroups - 1;
		numInputRedirects = 0;
		numOutRedirects = 0;
		
		printf("successful parse! numCmds: %d\n", numTokenGroups);
		printf("number of pipes: %d\n", numPipes);

		if(numTokenGroups == 0)
			printf("ERROR: No command entered\n");

		if(numTokenGroups == 1){	//no pipes
			strcpy(command, tokenGroup[0]);
			printf("command found: %s\n", command);	
			//params[0] = malloc(strlen(command)+1);			
			//strcpy(params[0], command);
			//printf("parameter[%d]:%s\n", 0, params[0]);
			validCmd = validateCommand(command, params, numInputRedirects, numOutRedirects);
			printf("validCmd: %d\n", validCmd);
			if(validCmd != 0){
				printf("ERROR: Invalid command entered. Check syntax.\n");
				}
			else
				printf("ready to execute command - %s\n", params[0]);
			execStatus = executeCommand(params);
			if(execStatus == 0)
				printf("Successful Command Execution\n");		

			
			
		}	
		
		




			

		free(inpt);
		//free(temp);
		//free(tokenGroup);
	}
	
	return 0;
}


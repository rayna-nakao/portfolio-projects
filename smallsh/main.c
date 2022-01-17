#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

//credit 6
//for SIGTSTP handler function
bool backgroundAllowed = true;

//to store background pids
int checkPids[200];
int pidCounter = 0;

//to hold user input
//first element of arguments array holds command
struct input {
  char *command;
  char *arguments[513];
  bool *inputFileYes;
  char *inputFile;
  bool *outputFileYes;
  char *outputFile;
};

/**********************************************************************
* storePid: accepts pid (int), stores pid in checkPids array and
* increments pidCounter
**********************************************************************/
void storePid(int thePid) {
  checkPids[pidCounter] = thePid;
  pidCounter++;
}


/**********************************************************************
* checkBackground: checks status of background processes, prints
* message and exit signal if process complete, and removes terminated
* pids from the checkPids array
**********************************************************************/
void checkBackground() {
  int checkPid, childStatus;
  int counter = pidCounter;

  //go through pidCounter array
  for (int i = 0; i < counter; i++) {
    //check if process terminated
    if((checkPid = waitpid(checkPids[i], &childStatus, WNOHANG)) > 0) {
      //process terminated normally
      if (WIFEXITED(childStatus)) {
        printf("background pid %d is done: terminated by signal %d\n", checkPids[i], WEXITSTATUS(childStatus));
        fflush(stdout);
        pidCounter--;
        checkPids[i] = -100;
      }
      //process terminated abnormally
      else if (WIFSIGNALED(childStatus)) {
        printf("background pid %d is done: terminated by signal %d\n", checkPids[i], WTERMSIG(childStatus));
        fflush(stdout);
        pidCounter--;
        checkPids[i] = -100;
      }
    }
  }

  //remove elements == -100 (terminated processes)
  for (int i = counter - 1; i > -1; i--) {
    if (checkPids[i] == -100) {
      for (int j = i + 1; i < counter; i++) {
        checkPids[j-1] = checkPids[j];
      }
    }
  }
}


/**********************************************************************
* killBackground: kills background processes
**********************************************************************/
void killBackground() {
  for (int i = 0; i < pidCounter; i++) {
    kill(checkPids[i], SIGTERM);
  }
}


//credit 7
/**********************************************************************
* executeCmd: executes the command in foreground or background, creates
* new process; if in background, store pid and return. if in 
* foreground, wait for process to terminate then return
**********************************************************************/
void executeCmd(struct input *cmd, bool background, int *childStatus, struct sigaction cntrlc, struct sigaction cntrlz) {
  pid_t spawnpid = -5;
  spawnpid = fork();
  int backgroundChildStat, backgroundChildPid, foregroundChildStatus;

  switch (spawnpid) {
    //fork fails
    case -1:
      perror("fork() failed\n");
      exit(1);
      break;
    case 0:
      //don't ignore SIGINT if running in foreground
      if (!background) {
        cntrlc.sa_handler = SIG_DFL;
        sigaction(SIGINT, &cntrlc, NULL);
      }

      //ignore sigtstp in child
      cntrlz.sa_handler = SIG_IGN;
      sigaction(SIGTSTP, &cntrlz, NULL);


      //credit 11, 12
      //if input file, open it and redirect
      if (cmd->inputFileYes) {
        int inputF = open(cmd->inputFile, O_RDONLY);
        if (inputF == -1) {
          perror("open()");
          exit(1);
        }

        int result = dup2(inputF, 0);
        if (result == -1) {
          perror("source dup2()");
          exit(2);
        }

        fcntl(inputF, F_SETFD, FD_CLOEXEC);
      }

      //credit 11, 12
      //if output file, open it and redirect
      if (cmd->outputFileYes) {
        int outputF = open(cmd->outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0664);
        if (outputF == -1) {
          perror("open()");
          exit(1);
        }

        int result = dup2(outputF, 1);
        if (result == -1) {
          perror("source dup2()");
          exit(2);
        }

        fcntl(outputF, F_SETFD, FD_CLOEXEC);
      }
 
      //execute, if fails print error message
      if (execvp(cmd->arguments[0], cmd->arguments) == -1) {
        printf("%s: no such file or directory\n", cmd->arguments[0]);
        fflush(stdout);
        exit(1);
      }
   
      break;
    //credit 8, credit 9
    default:
      //if backgroundAllowed and background == true, run in background, print pid
      //do not wait for child process to terminate (WNOHANG)
      //store background pid 
      if (backgroundAllowed && background) {
        pid_t childPid = waitpid(spawnpid, &backgroundChildStat, WNOHANG);
        printf("background pid is %d\n", spawnpid);
        fflush(stdout);
        storePid(spawnpid);
      }

      //else, run in foreground and wait for child process to terminate
      else {
        pid_t childPid = waitpid(spawnpid, childStatus, 0);

        //if process terminated abnormally (sigint) print signal 
        if (WIFSIGNALED(*childStatus)) {
          printf("terminated by signal %d\n", WTERMSIG(*childStatus));
          fflush(stdout);
         }
      }
  }
}


//credit 4
/**********************************************************************
* handleSIGTSTP: handles SIGTSTP, changes backgroundAllowed bool
**********************************************************************/
void handleSIGTSTP(int signo) {
  //first receipt of SIGTSTP, foreground only
  if (backgroundAllowed) {
    char *message = "\nEntering foreground-only mode (& is now ignored)\n: ";
    write(STDOUT_FILENO, message, strlen(message));
    fflush(stdout);
    backgroundAllowed = false;
  }

  //second receipt of SIGTSTP, exit foreground only
  else {
    char *message = "\nExiting foreground-only mode\n: ";
    write(STDOUT_FILENO, message, strlen(message));
    fflush(stdout);
    backgroundAllowed = true;
  }
}


/**********************************************************************
* changeDirectory: executes change in directory command
**********************************************************************/
void changeDirectory(struct input *cmd) {
  //no argument 
  if (cmd->arguments[1] == NULL) {
    //credit 5
    chdir(getenv("HOME"));
  }

  //relative or absolute path provided
  else {
    chdir(cmd->arguments[1]);
  }
}


//credit 10
/**********************************************************************
* getStatus: displays exit value
**********************************************************************/
void getStatus(int childStatus) {
  //process terminated normally
  if (WIFEXITED(childStatus)) {
    printf("exit value %d\n", WEXITSTATUS(childStatus));
    fflush(stdout);
  }
  
  //process terminated abnormally 
  else {
    printf("exit value %d\n", WTERMSIG(childStatus));
    fflush(stdout);
  }
}


//credit 2
/**********************************************************************
* stringReplace: replaces all "$$" occurrences with pid
**********************************************************************/
char* stringReplace(char *source, char *substring, char *pid) {
  //get pointer to substring
  char *sourceSub = strstr(source, substring);
  if (sourceSub == NULL) {
    return NULL;
  }
  
  memmove(sourceSub + strlen(pid), sourceSub + strlen(substring), strlen(sourceSub) - strlen(substring) + 1);
  memcpy(sourceSub, pid, strlen(pid));

  return sourceSub + strlen(pid); 
}


//credit 3
/**********************************************************************
* getOccurrences: returns the number of "$$" occurrences 
**********************************************************************/
int getOccurrences(char *string, char *replace) {
  int counter = 0;
  for (int i = 0; string[i] != '\0'; i++) {
    if (strstr(&string[i], replace) == &string[i]) {
      counter++;
      i += strlen(replace) - 1;
    }
  }
  return counter;
}

//credit 13
/**********************************************************************
* getpidLen: returns length of the passed in pid
**********************************************************************/
int getpidLen(int thePid) {
  int counter = 0;
  while (thePid != 0) {
    counter++;
    thePid /= 10;
  }
  return counter;
}


/**********************************************************************
* storeCommand: parses user input, expands "$$" if any, 
* stores command/arguemnts in input struct, returns struct
**********************************************************************/
struct input *storeCommand(char line[], bool *background) {
  struct input *newLine = malloc(sizeof(struct input));
  
  //credit 1
  char *saveSpot;
  char delims[] = {" \t\r\v\f\n\0"};

  //get and store command
  char *token = strtok_r(line, delims, &saveSpot);
  newLine->command = calloc(strlen(token) + 1, sizeof(char));
  strcpy(newLine->command, token);
  newLine->arguments[0] = calloc(strlen(token) + 1, sizeof(char));
  strcpy(newLine->arguments[0], token);
  
  //loop to store arguments
  token = strtok_r(NULL, delims, &saveSpot);
  int counter = 0;
  while (token) {
    //if there is an input file...
    //get next token and store it in inputFile
    if (strcmp(token, "<") == 0) {
      newLine->inputFileYes = true;
      token = strtok_r(NULL, delims, &saveSpot);
      newLine->inputFile = calloc(strlen(token) + 1, sizeof(char));
      strcpy(newLine->inputFile, token);
    }

    //if there is an output file...
    //get next token and store it in outputFile
    else if (strcmp(token, ">") == 0) {
      newLine->outputFileYes = true;
      token = strtok_r(NULL, delims, &saveSpot);
      newLine->outputFile = calloc(strlen(token) + 1, sizeof(char));
      strcpy(newLine->outputFile, token);
    }

    //token is part of argument
    else {
      counter++;

      //if argument is '&' and is last argument, background = true
      if (strcmp(token, "&") == 0 && strcmp(saveSpot, "\0") == 0) {
        *background = true;
      }

      //if no "&", store token as is
      else {
        newLine->arguments[counter] = calloc(strlen(token) + 1, sizeof(char));
        strcpy(newLine->arguments[counter], token);
      }
    }
    token = strtok_r(NULL, delims, &saveSpot);
  }
  return newLine;
}


/**********************************************************************
* readline: get user input, call other functions to parse input/store
* command/arguments, then executes the command
**********************************************************************/
void readline(struct sigaction cntrlc, struct sigaction cntrlz) {
  bool isExit = false;

  //to replace $$
  char *var = "$$";
  int pidLen = getpidLen(getpid());
  char pidChar[pidLen];
  snprintf(pidChar, pidLen + 1, "%d", getpid());

  do {
    bool background = false;
    int childStatus;

    //print colon
    printf(": ");

    //store user input
    char userInput[2048];
    fgets(userInput, 2048, stdin);

    //if blank line, do nothing
    if (strlen(userInput) == 1) {
      checkBackground();
      continue;
    }

    //struct to hold input
    struct input *newLine;

    //expand any occurences of $$ into newToken
    //parse data and store in newLine struct
    if (strstr(userInput, var)) {
      //create var to hold newToken with "$$"s replaced
      int count = getOccurrences(userInput, var);
      char newToken[strlen(userInput) + count * (pidLen - strlen(var)) + 1];
      strcpy(newToken, userInput);

      //replace all occurrence 
      while(stringReplace(newToken, var, pidChar));

      newLine = storeCommand(newToken, &background);
    }

    //no occurences of $$, parse data and store in struct
    else {
      newLine = storeCommand(userInput, &background);
    }

    //if line is comment, ignore and check background processes
    if (strcmp(newLine->command, "#") == 0 || strncmp(newLine->command, "#", 1) == 0) {
      checkBackground();
    }

    //command is exit
    else if (strcmp(newLine->command, "exit") == 0) {
      //stop any running processes 
      killBackground();

      //set to exit loop/program
      isExit = true;
    }

    //command is cd
    else if (strcmp(newLine->command, "cd") == 0) {
      changeDirectory(newLine);
    }

    //command is status
    else if (strcmp(newLine->command, "status") == 0) {
      getStatus(childStatus);
    }

    //do command
    else {
      executeCmd(newLine, background, &childStatus, cntrlc, cntrlz);
    }

    //check background processes
    checkBackground();
  } while (!isExit);
}


int main(void) {
  //credit 4
  //ignore sigint (control-c)
  struct sigaction SIGINT_action = {0};
  SIGINT_action.sa_handler = SIG_IGN;
  sigfillset(&SIGINT_action.sa_mask);
  SIGINT_action.sa_flags = 0;
  sigaction(SIGINT, &SIGINT_action, NULL);

  //handle (print message) SIGTSTP (control-z)
  struct sigaction SIGTSTP_action = {0};
  SIGTSTP_action.sa_handler = handleSIGTSTP;
  sigfillset(&SIGTSTP_action.sa_mask);
  SIGTSTP_action.sa_flags = SA_RESTART;
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);

  //execute main portion of program 
  readline(SIGINT_action, SIGTSTP_action);
}
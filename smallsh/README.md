# smallsh
### Oreogon State University, CS 340, Portfolio Project
#### Program Requirements: <br />
In this assignment you will write smallsh your own shell in C. smallsh will implement a subset of features of well-known shells, such as bash. Your program will: <br /><br />
Provide a prompt for running commands <br />
Handle blank lines and comments, which are lines beginning with the # character <br />
Provide expansion for the variable $$ <br />
Execute 3 commands exit, cd, and status via code built into the shell <br />
Execute other commands by creating new processes using a function from the exec family of functions <br />
Support input and output redirection <br />
Support running commands in foreground and background processes <br />
Implement custom handlers for 2 signals, SIGINT and SIGTSTP <br />

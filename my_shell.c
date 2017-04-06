
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>




#define BUFFER 64
#define DELIM " \t\r\n\a"


int check = 0;
char *history[10];
int counter = 0;
int argslen;
int x;


char **parseline(){
  int buffer = BUFFER;
  int pos = 0;
  char *line = NULL;
  char **tokens = malloc(buffer *sizeof(char*));
  char *tok;



  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  //@line is now the line from stdin, need to parse the line

  //saving line to history

  if (counter < 10) {
    history[counter] = line;
    counter++;
  }
  else {
    int j = 0;
    counter = 9;
    for (int i = 0; i < 9; i++) {
      j = i + 1;
      history[i] = history[j];
    }
  }

  tok = strtok(line,DELIM);

  while(tok!=NULL) {
    tokens[pos] = tok;
    pos++;
    tok = strtok(NULL,DELIM);
    argslen++;

  }
  tokens[pos] = NULL;
  return tokens;
}


/*

INTERNAL COMMANDS (CD, EXIT, PWD, HISTORY)


*/


int mycd(char **args) {

  if (args[1] == NULL) {
    fprintf(stderr, "No location given for cd\n" );
  }
  else {
    if (chdir(args[1]) != 0) {
      perror("");
    }



  }
  return 1;
}

int myexit(char **args)
{
  _exit(0);
  check = 1;
  return 0;
}

int mypwd(char **args) {
  char *cwd;
  char buff[420];

  cwd = getcwd(buff, 420);
  if(cwd != NULL) {
    printf("%s\n",cwd );
  }
}

int myhistory(char ** args) {
  for (int i = 0; i < counter; ++i) {
    printf("%s\n", history[i] );
  }
  return 0;
}


/*
CHECKING IF USER HAS GIVEN AN INTERNAL COMMAND
*/

int checkcomm(char **args) {
  if (strcmp(args[0],"cd") == 0) {
    mycd(args);
    return 1;
  }
  if (strcmp(args[0], "exit") == 0) {
    myexit(args);
    return 1;

  }
  if (strcmp(args[0],"pwd") == 0) {
    mypwd(args);
    return 1;

  }
  if (strcmp(args[0],"history") == 0) {
    myhistory(args);
    return 1;

  }

  return 0;
}

/*
SIGNAL HANDLER
*/
//
// void signal_handler (int sig) {
//
//   signal(SIGINT,signal_handler);
//   if (sig==SIGINT)
// 	{
// 		printf ("\nCTRL PRESSED");
//     x = sig;
//       }
//
// }


/*
EXECUTING, CREATING PROCESSES & CHECKING FOR INTERNAL COMMANDS
*/
int checkpipe(char **args) {
  for(int i = 0; i < argslen; i++ ) {

    if (strcmp(args[i],"|") == 0) {
      return i;
     }
   }
   return 0;
}


int execute(char **args) {
  int i,j,k;



  if (checkpipe(args) != 0) {
    char **cut;
    j = checkpipe(args);
    printf("%i\n", j );
    printf("%i\n",argslen - checkpipe(args) );
    for(i = 0; i < (argslen - checkpipe(args)) - 1;i++) {
      cut[i] = args[j];
      j++;
    }
    printf("%s\n",cut[0] );

    execute(cut);
  }
  // for(i = 0; i < argslen; i++ ) {
  //   // if (strcmp(args[i],"|") == 0) {
  //   //   printf("we pipe here fam\n");
  //   //   printf("%s\n",args[2] );
  //   //   // int k = 0;
  //   //   // char *new[argslen - i - 1];
  //   //   // for(j = i+1; j < argslen;j++){
  //   //   //   new[k] = args[i];
  //   //   //   k++;
  //   //   //   }
  //   //   // execute(new);
  //   // }
  // }

  if (args[0] == NULL) {
    return 1;
  }

  if (checkcomm(args) == 1) {
    return 0;
  }
  x = 0;

int pid;

if ((pid = fork())==0) {

  if (checkcomm(args) == 1) {
    return 0;
  }
  int j = execvp(args[0],args);
}
else {
  if (checkcomm(args) == 1) {
    return 0;
  }
   wait(NULL);
  if (check !=0) {
    return 0;
  }
  }
}




int main() {

  printf("\n============================================\n");
  printf("\tWELCOME TO JASON'S SHELL\n");
  printf("\tjsoliman@sfu.ca | 301218953\n");
  printf("\tInternal commands include exit, pwd, history and cd.\n" );
  printf("============================================\n\n");

  while (check == 0) {
  char user[1204] = "";
  gethostname(user, sizeof(user));
  printf("%s@%s > ", getenv("LOGNAME"), user);

  char **args = parseline();

  execute(args);
  argslen = 0;
}


  return 0;

}

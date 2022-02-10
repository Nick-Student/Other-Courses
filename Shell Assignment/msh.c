/*

  Name: Nicholas Soliz
  ID:   1001744764

*/


// The MIT License (MIT)
// 
// Copyright (c) 2016, 2017, 2020 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11     // Mav shell only supports five arguments

int main()
{

  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  char history[15][MAX_COMMAND_SIZE];
  int history_index = 0;
  int history_total = 0;

  int pid_index = 0;
  int pid_history[15];
  int pid_total = 0;

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    
    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    // The following if statement makes it so if the user
    // hits enter, then the system won't throw an error
    // and will simply print a blank new line and wait for input.
    if(cmd_str[0] == '\n') continue;

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr;                                         
                                                           
    char *working_str  = strdup( cmd_str );                

    // History storage begins here, this bit of code allows us to store
    // the full string of what is entered by the user
    // This string is stored in an array that can be referenced later
    // in order to keep the array to a size of 15, overwirting occurs
    // after the 14th entry, resetting to the 0th position.

    strncpy( history[history_index++], cmd_str, 255);
    history_total++;
    if(history_index > 14) history_index = 0;

    int i = 0;

    int index = history_index;
    
    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input strings with whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }
    


    pid_total++;
    if(pid_index > 14) pid_index = 0;
    int control = pid_index;

    int token_index  = 0;

    if( token[token_index] == NULL ) continue;

    

///////////////////////////////////////////////////////////////////////////////////////////
    // This is where history is printed, if the command history is typed
    // then there will be a check to see if the history is greater than 15
    // if it is then it prints only the 15 most recent, otherwise it prints all 
    // commands run so far.
    if(strcmp(token[0],"history") == 0)
    {
      if(history_total > 14)
        for( i = 0; i < 15; i++)
        {
          printf("%d: %s", index, history[index] );
          index++;
          if(index > 14 ) index = 0;
        }
      else
        for(i = 0; i < history_total; i++)
        {
          printf("%d: %s", i+1, history[i] ); 
        }
      continue;
    }

    
    


///////////////////////////////////////////////////////////////////////////////////////////
    // THis is where i check to see if the command entered is to change directories
    // If it is, then i call the command to change the directory to the parameter entered
    else if(strcmp(token[0],"cd") == 0)
    {
      chdir(token[1]);
      continue;
    }
///////////////////////////////////////////////////////////////////////////////////////////
    // This is where i check to see if the command entered is to quit out of the program
    // If so the program exits on a successful condition.
    else if(strcmp(token[0],"quit") == 0)
    {
      return 0;
    }
///////////////////////////////////////////////////////////////////////////////////////////
    // Second verse same as the first, but instead of quit the command is exit.
    else if(strcmp(token[0],"exit") == 0)
    {
      return 0;
    }
///////////////////////////////////////////////////////////////////////////////////////////
    // This is where i allow for the call of the command showpids. If the command is 
    // entered then the system will check to see how many pids have been generated
    // If the pids generated exceeds 15, then only the most recent 15 will be printed
    // If less than 15, then all pids generated thus far are printed.
    else if(strcmp(token[0],"showpids") == 0)
    {
      if(pid_total > 14)
        for( i = 0; i < 15; i++)
        {
          printf("%d: %d\n", control, pid_history[control] );
          control++;
          if(control > 14 ) control = 0;
        }
      else
          for(i = 0; i < pid_total; i++)
          {
            printf("%d: %d\n", i, pid_history[i] ); 
          }
      continue;
    }
////////////////////////////////////////////////////////////////////////////////////////////
    // This is the exec function calling section. This is where all other commands not
    // previously caught and handled in the above sections are run. The fork function
    // creates a child and the child runs the execvp of the command with any parameters
    // once the command is done, the child will die and the parent will wake up from a 
    // forced wait condition. If an invalid command is entered then an error will be 
    // printed and the child will exit, dying and being reaped by the parent.
    else
    {
      pid_history[pid_index++] = fork();
      
      if( pid_history[pid_index-1] == 0)
      {
        int ret = execvp( token[0], &token[0] );  
        if( ret == -1 )
        {
          printf(" %s: Command Not Found.\n", token[0]);
          exit(0);
          //perror("execl failed: ");
        }
      }
    
        else 
        {
          int status;
          wait( & status );
        }
        
    }

    free( working_root );

  }
  return 0;
}

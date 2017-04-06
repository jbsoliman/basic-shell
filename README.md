# basic-shell


This project contains:my_shell.c,
                      a makefile that compiles the C file
                      and a my_shell executable


The shell prints out a prompt,
          reads a line from the user
          and parses the line into arguments that are used to execute commands.

The fork() system is used to create new processes, and the entire shell runs in a loop.

There are also internal commands: exit, pwd, history and cd.

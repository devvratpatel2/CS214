#ifndef MY_PS

// This is the helper header class for this project and it contains the struct that will provide
// all the required fields for this project. 
// Devvrat Patel
// dap299
// Cs214 

#include <stdio.h>
#include <stdlib.h>

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>


typedef struct structObj{
  char *user;
  int pid;
  float pcpu, pmem, vsz, rss;
  char tty;
  char *start;
  char *time;
  char *command;

  struct structObj *next;
}procObj;


#endif

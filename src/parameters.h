#ifndef PARAMETERS_H
#define PARAMETERS_H

/* header file includes ------------------------------------------ */
#include <unistd.h>
#include "ncurses.h"

/* function macro definitions ------------------------------------ */
#define CHECK(sts, msg) \
  if (-1 == (sts)) { perror (msg); exit (EXIT_FAILURE); }
#define CHECK_NULL(sts, msg) \
  if (NULL == (sts)) { perror (msg); exit (EXIT_FAILURE); }
#define UNUSED(x) (void) x

/* type definition ----------------------------------------------- */
struct element 
{
  char * label;
  int x;
  int y;
  int type;
  int value;
};

/* symbolic constants -------------------------------------------- */
#define CHECKBOX  0
#define NUMBER    1
#define BUTTON    2

/* function prototypes ------------------------------------------- */
int * parameters (int * img_parameters);

#endif
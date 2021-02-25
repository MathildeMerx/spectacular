#ifndef RUN_H
#define RUN_H

/* header file includes ------------------------------------------ */
#include "ncurses.h"
#include "parameters.h"
#include "file.h"
#include "image.h"

/* function macro definitions ------------------------------------ */
#define CHECK(sts, msg) \
  if (-1 == (sts)) { perror (msg); exit (EXIT_FAILURE); }
#define CHECK_NULL(sts, msg) \
  if (NULL == (sts)) { perror (msg); exit (EXIT_FAILURE); }
#define UNUSED(x) (void) x

/* symbolic constants -------------------------------------------- */
#define UID_LENGTH  100
#define CMD_PYTHON  "python3 ../script/rfid.py"
#define MATCH_FILE  "../files/match"
#define LASTID_FILE "../files/lastid"
#define IMG_PATH    "../img/"

#define WAITING     0
#define RETRIEVE    1
#define DISPLAY     2
#define PARAM       3


/* function prototypes ------------------------------------------- */
void run ();

#endif
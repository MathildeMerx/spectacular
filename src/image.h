#ifndef IMAGE_H
#define IMAGE_H

/* header file includes ------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

/* function macro definitions ------------------------------------ */
#define CHECK(sts, msg) \
  if (-1 == (sts)) { perror (msg); exit (EXIT_FAILURE); }

/* symbolic constants -------------------------------------------- */
#define CMD_LENGTH 100

#ifndef TARGET
  #define TARGET  "PC"
  #define FBVS    "./fbvs_pc"
#else
  #define FBVS    "./fbvs_pi"
#endif

/* function prototypes ------------------------------------------- */
void display_image (char * image_name, int * img_param);

#endif
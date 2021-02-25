/* ---------------------------------------------------------------
 *
 * FILENAME     : image.c
 * CREATION     : 29/01/2021
 * AUTHOR       : Luca Jakomulski
 * PURPOSE      : build command with parameters and display image
 * 
 * --------------------------------------------------------------- */

/* header file includes ------------------------------------------ */
#include "image.h"

/* function prototypes ------------------------------------------- */
static char * build_command (char * img_filename, int * img_param);

/* functions ----------------------------------------------------- */
  void
display_image (char * img_filename, int * img_param)
{
  char * command;
  int duperr;

  command = build_command (img_filename, img_param);
  
  /* hide error coming from shell */
  duperr = dup (STDERR_FILENO);
  close (STDERR_FILENO);

  /* display image */
  system (command);

  /* display error coming from shell */
  dup2 (duperr, STDERR_FILENO);
  close (duperr);
}

/* --------------------------------------------------------------- */
  static char *
build_command (char * img_filename, int * img_param)
{
  char * command;
  char options[6] = " -";

  if (img_param[0]) {
    /* use the alpha channel if applicable */
    strcat (options, "a");
  }

  if (img_param[1]) {
    /* enlarge the image to fit the whole screen if necessary */
    strcat (options, "e");
  }

  if (img_param[2]) {
    /* ignore the image aspect while resizing */
    strcat (options, "r");
  }

  if (strlen (options) < 3) {
    /* remove options if none were selected */
    options[0] = 0;
  }

  /* build command to display image */
  command = (char *) calloc (CMD_LENGTH, sizeof(char));
  sprintf (command, "%s%s < %s", FBVS, options, img_filename);

  return command;
}
/* ---------------------------------------------------------------
 *
 * FILENAME     : run.c
 * CREATION     : 12/02/2021
 * AUTHOR       : Luca Jakomulski
 * PURPOSE      : 
 * 
 * --------------------------------------------------------------- */

/* header file includes ------------------------------------------ */
#include "run.h"

/* function prototypes ------------------------------------------- */
static void event_loop ();
static void signal_handler (int num_sig);
static struct list_images * retrieve_images (struct list_images * images);
static void display_images (struct list_images * images, int * img_parameters);
static void read_id (const char * command);
static void waiting (void);

/* global variables ---------------------------------------------- */
int state = PARAM;
int pipefd[2];

/* functions ----------------------------------------------------- */
  void
run ()
{
  pid_t child_pid;
  struct sigaction new_action;
  int stat_loc;
  
  /* create pipe */
  CHECK (pipe (pipefd), "pipe ()");

  /* ignore SIGTSTP signal */
  new_action.sa_handler = SIG_IGN;
  new_action.sa_flags = 0;
  CHECK (sigemptyset (&new_action.sa_mask), "sigemptyset ()");
  CHECK (sigaction (SIGTSTP, &new_action, NULL), "sigaction ()");

  /* create child process */
  do { child_pid = fork ();
  } while ((child_pid == -1) && (errno == EAGAIN));

  switch (child_pid) {
  case -1 :
    /* not enough memory */
    perror ("Failed to create child process.");
    exit (EXIT_FAILURE);
    break;
  case 0 :
    /* child process */
    read_id (CMD_PYTHON);
    exit (EXIT_SUCCESS);
    break;
  default :
    /* father process */
    event_loop ();
    break;
  }

  /* waiting the end of child process */
  while (wait (&stat_loc) == -1 && errno == EINTR);

  /* normal termination of father process */
  exit (EXIT_SUCCESS); 
}

/* --------------------------------------------------------------- */
  static void
event_loop ()
{
  struct sigaction new_action;
  struct list_images * images = NULL;
  
  int * img_parameters = calloc (4, sizeof (int));
  img_parameters[3] = 10;

  /* replace SIGUSR1 and SIGTSTP signal actions */
  new_action.sa_handler = signal_handler;
  new_action.sa_flags = 0;
  CHECK (sigemptyset (&new_action.sa_mask), "sigemptyset ()");
  CHECK (sigaction (SIGUSR1, &new_action, NULL), "sigaction ()");
  CHECK (sigaction (SIGTSTP, &new_action, NULL), "sigaction ()");

  /* initialize ncurses */
  initscr ();
  /* no need to hit enter key */
  cbreak ();
  /* hide pressed key */
  noecho();
  /* enable use of function keys */
  keypad (stdscr, TRUE);
  /* clear screen */
  clear ();

  while (1) {
    switch (state) {
    case WAITING:
      waiting ();
      break;
    case RETRIEVE:
      images = retrieve_images (images);
      break;
    case DISPLAY:
      display_images (images, img_parameters);
      break;
    case PARAM:
      img_parameters = parameters (img_parameters);
      state = WAITING;
      break;
    default:
      state = WAITING;
      break;
    }
  }

  endwin ();
}

/* --------------------------------------------------------------- */
  static void
signal_handler (int num_sig)
{
  char card_id[UID_LENGTH];

  if (num_sig == SIGUSR1) {
    if (state != PARAM) {
      state = RETRIEVE;
    }
    else {
      /* read card id from pipe */
      close (pipefd[1]);
      read (pipefd[0], card_id, UID_LENGTH);
    }
  }

  if (num_sig == SIGTSTP) {
    state = PARAM;
  }
}

/* --------------------------------------------------------------- */
  static void
waiting (void)
{
  /* hide cursor */
  curs_set (0);
  /* clear screen */
  clear ();
  /* apply changes on screen */
  refresh ();

  /* wait for signal */
  pause ();
}

/* --------------------------------------------------------------- */
  static struct list_images *
retrieve_images (struct list_images * images)
{
  char card_id[UID_LENGTH];

  /* read card id from pipe */
  close (pipefd[1]);
  read (pipefd[0], card_id, UID_LENGTH);

  if (strlen (card_id) == 0) {
    /* an error occured when reading from card */
    state = WAITING;
  }
  else {
    /* retrieve images associated to card id read */
    write_card_id (card_id, LASTID_FILE);
    destroy_list_images (images);
    images = new_list_images ();
    fill_list_images (images, card_id, MATCH_FILE);

    state = DISPLAY;
  }

  return images;
}

/* --------------------------------------------------------------- */
  static void
display_images (struct list_images * images, int * img_parameters)
{
  char img_filename[FILENAME_MAX];

  /* clear screen */
  clear ();
  /* apply changes on screen */
  refresh ();

  if (images != NULL && images->current < images->total) {
    /* display next image if any */
    sprintf (img_filename, "%s%s", IMG_PATH, images->list[images->current++]);
    display_image (img_filename, img_parameters);
    sleep (img_parameters[3]);
  }
  else if (images != NULL && images->total == 0) {
    /* inform user if no image found */
    mvprintw (1, 2, "No image was found for your card");
    mvprintw (2, 2, "Connect to spectacular wifi and go to http://172.24.1.1 to upload some");
    refresh();
    sleep (6);
    state = WAITING;
  }
  else {
    state = WAITING;
  }
}

/* --------------------------------------------------------------- */
  void
read_id (const char * command)
{
  FILE * pipe;
  char line[UID_LENGTH];
  
  /* read from pipe */
  pipe = popen (command, "r");
  
  if (pipe == NULL) {
    fprintf (stderr, "An error occured when reading card id.\n");
  }
  else {
    while (fgets (line, UID_LENGTH, pipe) != NULL) {

      /* keep all characters except newline */
      line[strcspn(line, "\n")] = 0;

      if (strlen (line) > 0) {
        close (pipefd[0]);
        write (pipefd[1], (const void *) line, strlen (line) + 1);
        kill (getppid (), SIGUSR1);
      }
    }
  }

  /* close pipe */
  CHECK (pclose (pipe), "pclose ()");
  
  /* normal termination */
  exit (EXIT_SUCCESS);
}
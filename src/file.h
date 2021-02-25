#ifndef FILE_H
#define FILE_H

/* header file includes ------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* function macro definitions ------------------------------------ */
#define CHECK(sts, msg) \
  if (-1 == (sts)) { perror (msg); exit (EXIT_FAILURE); }
#define CHECK_NULL(sts, msg) \
  if (NULL == (sts)) { perror (msg); exit (EXIT_FAILURE); }
#define CHECK_EOF(sts, msg) \
  if (EOF == (sts)) { perror (msg); exit (EXIT_FAILURE); }

/* type definition ----------------------------------------------- */
struct list_images {
  char ** list;
  int current;
  int total;
};

/* symbolic constants -------------------------------------------- */
#define LINE_LENGTH 200

/* function prototypes ------------------------------------------- */
struct list_images * new_list_images ();
void destroy_list_images (struct list_images * images);
void fill_list_images (struct list_images * images, const char * card_id, const char * filename);
void write_card_id (const char * card_id, const char * filename);


#endif
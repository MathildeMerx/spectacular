/* ---------------------------------------------------------------
 *
 * FILENAME     : file.c
 * CREATION     : 29/01/2021
 * AUTHOR       : Luca Jakomulski
 * PURPOSE      : read from file and write to file
 * 
 * --------------------------------------------------------------- */

/* header file includes ------------------------------------------ */
#include "file.h"


/* functions ----------------------------------------------------- */
  struct list_images *
new_list_images ()
{
  struct list_images * images = calloc (1, sizeof (struct list_images));

  images->list = NULL;
  images->current = 0;
  images->total = 0;
  
  return images;
}

/* --------------------------------------------------------------- */
  void
destroy_list_images (struct list_images * images)
{
  int i;
  
  if (images != NULL) {
    for (i = 0; i < images->total; i++) {
      free (images->list[i]);
    }

    free (images->list);
    free (images);
  }
}

/* --------------------------------------------------------------- */
  void
fill_list_images (struct list_images * images, const char * card_id, const char * filename)
{
  FILE * file;
  char line[LINE_LENGTH];
  char * token, * image;

  /* read from file */
  file = fopen (filename, "r");
  
  if (file == NULL) {
    fprintf (stderr, "An error occured when reading file.\n");
  }
  else {
    while (fgets (line, LINE_LENGTH, file) != NULL) {

      /* extract the token which corresponds to the card id from line */
      token = strtok (line, ":");

      /* count the number of images to display */
      if (strcmp (card_id, token) == 0) {
        token = strtok (NULL, "\n");

        if (token != NULL) {
          images->total++;
        }
      }
    }
    
    images->list = (char **) calloc (images->total, sizeof(*(images->list)));
    
    if (images->list != NULL) {
      char ** list_it = images->list;

      /* read file again from beginning */
      rewind(file);

      while (fgets (line, LINE_LENGTH, file) != NULL) {

        token = strtok (line, ":");

        /* add the image associated to card id to the list of images */
        if (strcmp (card_id, token) == 0) {
          token = strtok (NULL, "\n");

          if (token != NULL) {
            CHECK_NULL (image = strdup (token), "strdup ()");
            *list_it = image;
            list_it++;
          }
        }
      }
    }
  }

  /* close file */
  CHECK_EOF (fclose(file), "fclose ()");
}

/* --------------------------------------------------------------- */
  void
write_card_id (const char * card_id, const char * filename)
{
  FILE * file;

  /* write to file */
  file = fopen (filename, "w");
  
  CHECK_EOF (fputs (card_id, file), "fputs ()");

  /* close file */
  CHECK_EOF (fclose(file), "fclose ()");
}

/* ---------------------------------------------------------------
 *
 * FILENAME     : parameters.c
 * CREATION     : 12/02/2021
 * AUTHOR       : Luca Jakomulski
 * PURPOSE      : 
 * 
 * --------------------------------------------------------------- */

/* header file includes ------------------------------------------ */
#include "parameters.h"

/* function prototypes ------------------------------------------- */
void draw (struct element * elements, int nbr_param);
void modify_param (struct element * elements, int nbr_param);

/* functions ----------------------------------------------------- */
  int *
parameters (int * img_parameters)
{
  /* elements to display */
  struct element elements[5] = {
    {"alpha\t: use the alpha channel if applicable", 0, 0, CHECKBOX, img_parameters[0]},
    {"enlarge\t: enlarge the image to fit the whole screen", 0, 0, CHECKBOX, img_parameters[1]},
    {"aspect\t: ignore the image aspect while resizing", 0, 0, CHECKBOX, img_parameters[2]},
    {"duration\t: duration in seconds of image display", 0, 0, NUMBER, img_parameters[3]},
    {"<Let's begin>", 0, 0, BUTTON, 0},
  };

  /* draw elements on screen */
  draw (elements, 5);
  /* manage user inputs */
  modify_param (elements, 5);

  /* gather parameters */
  for (int i = 0; i < 4; i++) {
    img_parameters[i] = elements[i].value;
  }

  return img_parameters;
}

/* --------------------------------------------------------------- */
  void
draw (struct element * elements, int nbr_param)
{
  int x, y, i;
  char c;

  /* display cursor */
  curs_set (1);
  /* clear screen */
  clear ();
  
  getmaxyx (stdscr, y, x);
  x = x / 5;

  /* display information to user */
  mvprintw (2, x, "Spectacular settings");
  mvprintw (y - 10, x, "hit space to select option");
  mvprintw (y - 9, x, "use up and down arrow keys to move");
  mvprintw (y - 8, x, "hit Ctrl-Z to access spectacular settings");
  
  for (i = 0; i < nbr_param; i++) {
    if (elements[i].type == CHECKBOX) {
      /* display checkbox element */
      if (elements[i].value == 0) {
        c = ' ';
      } else {
        c = 'x';
      }

      mvprintw (i + 5, x, "[%c]  %s", c, elements[i].label);

      /* update xy-position to place cursor */
      elements[i].x = x + 1;
      elements[i].y = i + 5;
    }
    else if (elements[i].type == NUMBER) {
      /* display number element */
      mvprintw (i + 5, x, "%02ds  %s", elements[i].value, elements[i].label);

      /* update xy-position to place cursor */
      elements[i].x = x + 1;
      elements[i].y = i + 5;
    }
    else if (elements[i].type == BUTTON) {
      /* display button element */
      mvprintw (i + 6, x, elements[i].label);

      /* update xy-position to place cursor */
      elements[i].x = x + 1;
      elements[i].y = i + 6;
    }
  }
}

/* --------------------------------------------------------------- */
  void
modify_param (struct element * elements, int nbr_param)
{
  int x, y, key;
  int j = 0;
  int end = 0;

  getmaxyx (stdscr, y, x);
  x = x / 5;

  /* move on the first element */
  move (elements[0].y, elements[0].x);

  while (end == 0 && (key = getch ())) {
    switch (key) {
    case KEY_UP:
      /* go up with the arrow */
      if (j > 0) {
        j--;
      }
      break;
    case KEY_DOWN:
      /* go down with the arrow */
      if (j + 1 < nbr_param) {
        j++;
      }
      break;
    case ' ':
      /* tick checkbox with space */
      if (elements[j].type == 0 && elements[j].value == 0) {
        elements[j].value = 1;
        mvprintw (j + 5, x + 1, "x");
      }
      else if (elements[j].type == 0 && elements[j].value == 1) {
        elements[j].value = 0;
        mvprintw (j + 5, x + 1, " ");
      }
      break;
    case KEY_LEFT:
      /* decrease number with arrow */
      if (elements[j].type == 1 && elements[j].value > 1) {
        elements[j].value--;
        mvprintw (j + 5, x, "%02d", elements[j].value);
      }
      break;
    case KEY_RIGHT:
      /* increase number with arrow */
      if (elements[j].type == 1 && elements[j].value < 99) {
        elements[j].value++;
        mvprintw (j + 5, x, "%02d", elements[j].value);
      }
      break;
    case KEY_ENTER:
    case 10:
      /* press button with enter key */
      if (elements[j].type == 2) {
        end = 1;
      }
      break;
    default:
      break;
    }

    /* display information according to current element */
    if (elements[j].type == 0) {
      move (y - 10, x);
      clrtoeol ();
      mvprintw (y - 10, x, "hit space to select option");
    }
    else if (elements[j].type == 1) {
      mvprintw (y - 10, x, "use left and right arrow keys to adjust time");
    }
    else {
      move (y - 10, x);
      clrtoeol ();
      mvprintw (y - 10, x, "hit enter to validate your settings");
    }

    /* move on the current element */
    move (elements[j].y, elements[j].x);
  }
}
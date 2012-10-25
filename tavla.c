#include <stdio.h>

/* CONSTANT DEFINITIONS */

#define PIECES_SIZE 30
#define LOCATIONS 24

/* STRUCT DEFINITIONS */

typedef struct {
  int index;
  const char* color;
  int location;
} piece;

/* FUNCTION DECLERATIONS */

void initialize_pieces(piece* const, const char* []);

/* MAIN FUNCTION*/

int main(int argc, char *argv[])
{
  /* VARIABLE DEFINITIONS */
  piece pieces[PIECES_SIZE];

  const char *color[] = {"black", "white"};

  initialize_pieces(pieces, color);
  int i;
  for(i = 0; i < PIECES_SIZE; i++) {
    printf("[%s, %d, %d]\n", pieces[i].color, pieces[i].index, pieces[i].location);
  }

  return 0;
}

/* FUNCTION DEFINITIONS */

void initialize_pieces(piece* const pieces, const char* color[])
{
  int i;

  for(i = 0; i < PIECES_SIZE / 2; i++) {
    /* black piece */
    pieces[i].index = i + 1;
    pieces[i].color = color[0];

    /* white piece */
    pieces[i+15].index = i + 1;
    pieces[i+15].color = color[1];

    if(i < 2) {
      pieces[i].location = 24;
      pieces[i+15].location = 1;
    }
    else if(i < 7) {
      pieces[i].location = 13;
      pieces[i+15].location = 12;
    }
    else if(i < 10) {
      pieces[i].location = 8;
      pieces[i+15].location = 17;
    }
    else if(i < 15) {
      pieces[i].location = 6;
      pieces[i+15].location = 19;
    }
  }

}

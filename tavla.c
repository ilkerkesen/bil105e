#include <stdio.h>

/* CONSTANT DEFINITIONS */

#define PIECES 30
#define LOCATIONS 26
#define BOARD_LINES 30
#define BOARD_COLUMNS 12

/* STRUCT DEFINITIONS */

typedef struct {
  int index;
  const char* color;
  int location;
} piece;

/* FUNCTION DECLERATIONS */

void initialize_pieces(piece* const, const char* []);
void initialize_locations(int [], int []);
void update_locations(piece* const, int [], int []);
void initialize_state(char [][BOARD_COLUMNS]);
void update_state(char [][BOARD_COLUMNS], int [], int[], int*);

/* MAIN FUNCTION*/

int main(int argc, char *argv[])
{
  /* VARIABLE DEFINITIONS */
  piece pieces[PIECES];
  int black_pieces_locations[26];
  int white_pieces_locations[26];
  char board_state[BOARD_LINES][BOARD_COLUMNS];
  int blank_lines;

  const char *color[] = {"black", "white"};

  initialize_pieces(pieces, color);
  initialize_locations(black_pieces_locations, white_pieces_locations);
  initialize_state(board_state);
  update_locations(pieces, black_pieces_locations, white_pieces_locations);
  update_state(board_state, black_pieces_locations, white_pieces_locations, &blank_lines);

  int i, j;
  for(i = 0; i < LOCATIONS; i++) {
    for(j = 0; j < LOCATIONS / 2 - 1; j++)
      printf("%c ", board_state[i][j]);
    printf("\n");
  }

  return 0;
}

/* FUNCTION DEFINITIONS */

void initialize_pieces(piece* const pieces, const char* color[])
{
  int i;

  for(i = 0; i < PIECES / 2; i++) {
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

void initialize_locations(int black_piece_locations[], int white_piece_locations[])
{
  int i;
  for(i = 0; i < LOCATIONS; i++) {
    black_piece_locations[i] = 0;
    white_piece_locations[i] = 0;
  }
}

void update_locations(piece* const pieces, int black_pieces_locations[], int white_pieces_locations[])
{
  int i;
  for(i = 0; i < PIECES; i++) {    
    if(pieces[i].color == "black")
      black_pieces_locations[pieces[i].location] += 1;
    else if(pieces[i].color == "white")
      white_pieces_locations[pieces[i].location] += 1;
  }
}

void initialize_state(char board_state[][BOARD_COLUMNS])
{
  int i, j;
  for(i = 0; i < BOARD_LINES; i++) {
    for(j = 0; j < BOARD_COLUMNS; j++)
      board_state[i][j] = ' ';
  }
}

/* FIXME */
void update_state(char board_state[][BOARD_COLUMNS], int black_pieces_locations[], int white_pieces_locations[], int* blank_lines)
{
  int i, j, k;
  int top_null_chars, bot_null_chars;
  *blank_lines = 0;

  for(i = 0; i < 15; i++) {
    for(j = 13; j < 25; j++) {
      if(black_pieces_locations[j] != 0 && i < black_pieces_locations[j])
	board_state[i][j-12] = 'B';
      else if(white_pieces_locations[j] != 0 && i < white_pieces_locations[j])
	board_state[i][j-12] = 'W';

    }
  }

  /*  
  for(i = 0, m = BOARD_LINES - 1; i < BOARD_LINES / 2; i++, m--) {
    top_null_chars = 0;
    bot_null_chars = 0;

    for(j = LOCATIONS / 2, n = LOCATIONS / 2 - 1, k = 0; j < LOCATIONS - 1; j++, n--, k++) {
      /* top board 
      if(black_pieces_locations[j] != 0 && i < black_pieces_locations[j])
	board_state[i][k] = 'B';
      else if(white_pieces_locations[j] != 0 && i < white_pieces_locations[j])
	board_state[i][k] = 'W';
      else
	top_null_chars += 1;

      /* bottom board 
      if(black_pieces_locations[n] != 0 && 30 - m < black_pieces_locations[n])
	board_state[m][k] = 'B';
      else if(white_pieces_locations[n] != 0 && 30 - m < white_pieces_locations[n])
	board_state[m][k] = 'W';
      else
	bot_null_chars += 1;
    }
    
    if(top_null_chars == 12)
      *blank_lines += 1;
    
    if(bot_null_chars == 12)
      *blank_lines += 1;
  
  }*/
}

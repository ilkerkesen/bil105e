#include <stdio.h>

/* CONSTANT DEFINITIONS */

#define PIECES 30
#define LOCATIONS 26
#define BOARD_LINES 30
#define BOARD_COLUMNS 12

/* STRUCT DEFINITIONS */

/* FUNCTION DECLERATIONS */

void initialize_pieces(int [], int []);
void initialize_board(char [][BOARD_COLUMNS]);
void update_board(char [][BOARD_COLUMNS], int [], int[], int*);
void print_board(char [][BOARD_COLUMNS], int*);

/* MAIN FUNCTION*/

int main(int argc, char *argv[])
{
  /* VARIABLE DEFINITIONS */
  int black_pieces[26];
  int white_pieces[26];
  char board[BOARD_LINES][BOARD_COLUMNS];
  int blank_lines;

  const char *color[] = {"black", "white"};

  initialize_pieces(black_pieces, white_pieces);
  initialize_board(board);
  update_board(board, black_pieces, white_pieces, &blank_lines);


  int i, j;
  for(i = 0; i < BOARD_LINES; i++) {
    for(j = 0; j < BOARD_COLUMNS; j++)
      printf("%c [%2d, %2d] ", board[i][j]);
    printf("\n");
  }

  printf("Blank lines: %d\n", blank_lines);

  return 0;
}

/* FUNCTION DEFINITIONS */

void initialize_pieces(int black_pieces[], int white_pieces[])
{
  /* initialize pieces */
  black_pieces[6] = 5;
  white_pieces[19] = 5;

  black_pieces[8] = 3;
  white_pieces[17] = 3;

  black_pieces[13] = 5;
  white_pieces[12] = 5;

  black_pieces[24] = 2;
  white_pieces[1] = 2;


  /* fill empty locations with zero  */
  int i;
  for(i = 0; i < LOCATIONS; i++) {
    if(i != 6 && i != 8 && i != 13 && i != 24) {
      black_pieces[i] = 0;
      white_pieces[LOCATIONS - 1 - i] = 0;
    }
  }
}

void initialize_board(char board_state[][BOARD_COLUMNS])
{
  int i, j;
  for(i = 0; i < BOARD_LINES; i++) {
    for(j = 0; j < BOARD_COLUMNS; j++)
      board_state[i][j] = ' ';
  }
}

void update_board(char board_state[][BOARD_COLUMNS], int black_pieces[], int white_pieces[], int* blank_lines)
{
  int i, j, k, m, n;
  int top_null_chars, bot_null_chars;
  *blank_lines = 0;

  for(i = 0, m = BOARD_LINES - 1; i < BOARD_LINES / 2; i++, m--) {
    top_null_chars = 0;
    bot_null_chars = 0;

    for(j = LOCATIONS / 2, n = LOCATIONS / 2 - 1, k = 0; j < LOCATIONS - 1; j++, n--, k++) {
      /* top board */
      if(black_pieces[j] != 0 && i < black_pieces[j])
	board_state[i][k] = 'B';
      else if(white_pieces[j] != 0 && i < white_pieces[j])
	board_state[i][k] = 'W';
      else
	top_null_chars += 1;

      /* bottom board */ 
      if(black_pieces[n] != 0 && BOARD_LINES - 1 - m < black_pieces[n])
	board_state[m][k] = 'B';
      else if(white_pieces[n] != 0 && BOARD_LINES - 1 - m < white_pieces[n])
	board_state[m][k] = 'W';
      else
	bot_null_chars += 1;
    }
    
    if(top_null_chars == 12)
      *blank_lines += 1;
    
    if(bot_null_chars == 12)
      *blank_lines += 1;
  
  }
}

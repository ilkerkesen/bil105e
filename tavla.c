#include <stdio.h>

/* CONSTANT DEFINITIONS */

#define PIECES 30
#define LOCATIONS 26

/* STRUCT DEFINITIONS */


/* FUNCTION DECLERATIONS */

void initialize_pieces(int [], int []);
void print_board(int [], int []);
int get_maximum_piece(int [], int[]);
int get_lines_length(int);
void print_side(const char*);

/* MAIN FUNCTION*/

int main(int argc, char *argv[])
{
  /* VARIABLE DEFINITIONS */
  int black_pieces[26];
  int white_pieces[26];

  initialize_pieces(black_pieces, white_pieces);
  print_board(black_pieces, white_pieces);

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

void print_board(int black_pieces[], int white_pieces[])
{
  /* counters  */
  int i, j, k;
  
  const int limit = get_lines_length(get_maximum_piece(black_pieces, white_pieces));

  print_side("top");
  
  for(i = 0; i < limit; i++) {
    printf("||||");
    for(j = 12, k = 13; j > 0; j--, k++) {
      printf("%3s", "");

      if(black_pieces[k] > i || black_pieces[j] + i >= limit)
	printf("B");
      else if(white_pieces[k] > i || white_pieces[j] + i >= limit)
	printf("W");
      else
	printf(" ");

      if(k == 18 || k == 24 || j == 7 || j == 1)
	printf("%3s||||", "");
    }
    printf("\n");
  }

  print_side("bottom");


}

int get_maximum_piece(int black_pieces[], int white_pieces[])
{
  /* evulate max piece on a location and its opposite location */
  int i;
  int hold;
  int max_piece = 0;

  for(i = 1; i < LOCATIONS / 2 - 1; i++) {
    hold = black_pieces[i] + white_pieces[i] + black_pieces[LOCATIONS - 1 - i] + white_pieces[LOCATIONS - 1 - i];
    if(hold > max_piece)
      max_piece = hold;
  }

  return max_piece;
}

int get_lines_length(int max_piece)
{
  if (max_piece < 15)
    return 14;

  return max_piece;
}

void print_side(const char* side)
{
  int i;
  
  if(side == "top") {
    printf("%40sWhite Home Base\n", "");
    for(i = 13; i < 25; i++) {
      if(i % 6 == 1)
	printf("%6s", "");
      if(i == 19)
	printf(" ");
      printf("%2d%2s", i, "");    
    }
    printf("\n");
  }

  /* ||||---|---|.... */
  for(i = 0; i < 14; i++) {
    if(i % 7 == 0) {
      printf("|||");
    }
    printf("|---");
  }
  printf("||||\n");
  

  if(side == "bottom") {
    for(i = 12; i > 0; i--) {
      if(i % 6 == 0)
	printf("%6s", "");
      if(i == 6)
	printf(" ");
      printf("%2d%2s", i, "");    
    }
    printf("\n");
    printf("%40sBlack Home Base\n", "");
  }
}

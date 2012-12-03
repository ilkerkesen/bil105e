/* --------------------------- INCLUDED LIBRARIES --------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------- CONSTANT DEFINITONS --------------------------- */

#define PIECES 30
#define LOCATIONS 26

/* -------------------------- FUNCTION PROTOTYPES --------------------------- */

/* initialize board */
void initialize_pieces(int [], int []);

/* movement functions */
int is_valid_move(int [], int [], int, int);
void move(int [], int [], int, int);

/* board drawing functions */
void print_board(int [], int []);
void print_side(const char *);

/* Utilize functions for board drawing */
int get_maximum_piece(int [], int[]);
int get_lines_length(int);

/* ----------------------------- MAIN FUNCTION ----------------------------- */

int main(int argc, char *argv[])
{
  /* ------------------------- VARIABLE DEFINITIONS -------------------------- */

  /* counters */
  int i;

  /* arrays that stores game/board state */
  int black_pieces[26];
  int white_pieces[26];

  /* variables for user interactions */
  int location[2];
  int new_location[2];

  /* initialize pieces for once */
  initialize_pieces(black_pieces, white_pieces);

  /* print hello */
  printf("Hello! It is the solution of first question of midterm.\n");
    printf("Press Ctrl + C to exit.\n");

  /* main program loop */
  while(1) {
    /* print board */
    print_board(black_pieces, white_pieces);
    printf("\n");

    /* get movements and move it */
    for(i = 0; i < 2; i++) {
      do {
	/* get piece location from user */
	printf("Enter piece-%d location: ", i+1);
	scanf("%d", &location[i]);

	/* get new location from user */
	printf("Enter piece-%d new location: ", i+1);
	scanf("%d", &new_location[i]);

	/* if the move is not valid */
	if(!is_valid_move(black_pieces, white_pieces, location[i], new_location[i]))
	  printf("Invalid movement. Please try again.\n");

	printf("\n");

	/* repeat it until the move is valid */
      } while(!is_valid_move(black_pieces, white_pieces, location[i], new_location[i]));
      
      /* move piece */
      move(black_pieces, white_pieces, location[i], new_location[i]);
    }

    /* clear screen */
    system("clear");

    /* print movements */
    for(i = 0; i < 2; i++)
      printf("movement-%d: from %d to %d.\n", i+1, location[i], new_location[i]);

    /* print information about exit */
    printf("Press Ctrl + C to exit.\n");
  }

  return 0;
}

/* ------------------------- FUNCTION DEFINITIONS -------------------------- */

void initialize_pieces(int black_pieces[], int white_pieces[])
{
  /* initializes pieces and their locations. */

  /* initialize pieces */
  black_pieces[6] = 5;
  white_pieces[19] = 5;

  black_pieces[8] = 3;
  white_pieces[17] = 3;

  black_pieces[13] = 5;
  white_pieces[12] = 5;

  black_pieces[24] = 2;
  white_pieces[1] = 2;

  /* fill empty locations with zero */
  int i;
  for(i = 0; i < LOCATIONS; i++) {
    if(i != 6 && i != 8 && i != 13 && i != 24) {
      black_pieces[i] = 0;
      white_pieces[LOCATIONS - 1 - i] = 0;
    }
  }
}

int is_valid_move(int black_pieces[], int white_pieces[], int location, int new_location)
{
  /* checks validation of movement */

  /* if location and new_location does not exist on board */
  if(location < 0 || location > 25 || new_location < 0 || new_location > 25)
    return 0;

  /* if no piece exist at location */
  if(black_pieces[location] == 0 && white_pieces[location] == 0)
    return 0;

  /* if there is an enemy gate on new location (except 0 and 25 location) */
  if(((black_pieces[location] != 0 && white_pieces[new_location] > 1) || 
      (white_pieces[location] != 0 && black_pieces[new_location] > 1)) &&
     (new_location != 25 || new_location != 0))
    return 0;

  return 1;
}

void move(int black_pieces[], int white_pieces[], int location, int new_location)
{
  /* moves piece */

  /* black piece movement */
  if(black_pieces[location] != 0) {
    black_pieces[location] -= 1;
    black_pieces[new_location] += 1;

    /* if there is a single white piece, hit it */
    if(white_pieces[new_location] != 0) {
      white_pieces[new_location] -= 1;
      white_pieces[0] += 1;
    }
  }

  /* white piece movement  */
  if(white_pieces[location] != 0) {
    white_pieces[location] -= 1;
    white_pieces[new_location] += 1;

    /* if there is a single black piece, hit it */
    if(black_pieces[new_location] != 0) {
      black_pieces[new_location] -= 1;
      black_pieces[25] += 1;
    }
  }

}

void print_board(int black_pieces[], int white_pieces[])
{
  /* prints current state of board to screen */

  /* counters */
  int i, j, k;
  
  /* get number of board lines */
  int limit = get_lines_length(get_maximum_piece(black_pieces, white_pieces));

  /* write top of board */
  print_side("top");
  
  /* write every line */
  for(i = 0; i < limit; i++) {
    /* write left part of board (it is not a location)*/
    printf("||||");
    
    /* i know, one variable is enough, but it is more aesthetic */
    for(j = 12, k = 13; j > 0; j--, k++) {
      /* print spaces for ( B W B...)*/
      printf("%3s", "");
      
      /*
       * first_condition || second_condition
       *
       * pieces[k] -> number of pieces in k location
       * i -> line number
       *
       * first_condition is for top board (locations from 13 to 24)
       * - it is not difficult to understand.
       * - if number of pieces in k location is greater than line number,
       * then write B (or W).
       * - ex. let k is 13 and pieces[k] = 5, it will print five times B (or W)
       * for i = 0, 1, 2, 3, 4. as you can see, there must be 5 pieces.
       *
       * second_condition is for bottom board (locations from 12 to 1)
       * - it is a little bit hard to understand, but solution is very practical.
       * - if number of pieces in k location plus line number is greater than or
       * equal to limit, then write B (or W).
       * - ex. (IMPORTANT) let k is 12, pieces[k] = 5 and limit = 14, it will
       * print five times B (or W) for i = 9 (5 + 9 >= 14, then print),
       * i = 10 (5 + 10 >= 14, then print) ... i = 14 (5 + 14 >= 14, then print)
       * as you can see, i = 8 (8 + 5 >= 14, it is not true, do not print)
       */

      if(black_pieces[k] > i || black_pieces[j] + i >= limit)
	printf("B");
      else if(white_pieces[k] > i || white_pieces[j] + i >= limit)
	printf("W");
      else
	printf(" ");

      /* print |||| middle of board row and right of board row */
      if(k == 18 || k == 24 || j == 7 || j == 1)
	printf("%3s||||", "");
    }
    
    /* new line */
    printf("\n");
  }

  /* write bottom of board */
  print_side("bottom");
}

int get_maximum_piece(int black_pieces[], int white_pieces[])
{
  /* evulate max piece on a location and its opposite location */
  int i;
  int hold;
  int max_piece;

  max_piece = 0;

  for(i = 1; i < LOCATIONS / 2 - 1; i++) {
    hold = 0;
    hold += black_pieces[i];
    hold += white_pieces[i];
    hold += black_pieces[LOCATIONS - 1 - i];
    hold += white_pieces[LOCATIONS - 1 - i];
    
    if(hold > max_piece)
      max_piece = hold;
  }

  return max_piece;
}

int get_lines_length(int max_piece)
{
  /* evalute board line number according to maximum piece */
  if (max_piece < 14)
    return 14;

  /*
   * + 1 because, let 13th location have 15 black pieces
   * without + 1, you wont understand locations of all
   * these pieces, they will be on 12th location, too.
   * it is possible :)
   */
  
  return max_piece + 1;
}

void print_side(const char *side)
{
  /* prints top/bottom part of board, location numbers */

  int i;
  
  /* if side is top */
  if(!strcmp(side, "top")) {
    printf("%40sWhite Home Base\n", "");
    for(i = 13; i < 25; i++) {
      /* print location numbers */
      if(i % 6 == 1)
	printf("%6s", "");
      if(i == 19)
	printf(" ");
      printf("%2d%2s", i, "");
    }
    printf("\n");
  }

  /* print ||||---|---|.... like board part */
  for(i = 0; i < 14; i++) {
    if(i % 7 == 0) {
      printf("|||");
    }
    printf("|---");
  }
  printf("||||\n");
  
  /* if side is bottom */
  if(!strcmp(side, "bottom")) {
    for(i = 12; i > 0; i--) {
      /* print location numbers */
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

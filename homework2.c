/* -------------------------------- COPYING --------------------------------- */

/*
 *   Copyright (C) 2012 by İlker Kesen <ilkerksn[at]gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/* --------------------------------- ABOUT ---------------------------------- */

/*
 * Tavla (backgammon) game with text-based ui
 * Author: İlker Kesen
 * E-mail: keseni _at_ itu.edu.tr
 * Number: 040100411
 * CRN: 12887
 */

/* ----------------------------- DOCUMENTATION ------------------------------ */

/*
 *   Coding Standarts
 *     1. Use under_score instead of camelCase.
 *     2. Set maximum row to 80 character.
 *     3. Use type *var instead of type* var for pointers.
 *     4. Indentations must be two spaces.
 *   
 *   General Algorithm
 *     There are five important things you should know:
 *       1. Game/Board state is stored in two 26-element-sized integer arrays 
 *          (black_pieces,  white_pieces). 1-24 elements are available locations
 *          on board, 0 is location for collected black pieces and broken white
 *          pieces. Therefore, 25 is location for broken black pieces and collected
 *          white pieces.
 *       2. Drawing board is a little bit hard to understand, but in my opinion,
 *          it is well documented in drawing functions.
 *	 3. is_available_movement is the most crucial function of game, that checks
 *	    validation of a movement based on backgammon rules and returns 0 or 1.
 *       4. AI is not developed totally yet, but play_first_move function works
 *          nice. It plays first available movement in current state.
 *       5. Hard AI will depend on "state ranking system". I'll explain it later.
 *
 *     If you want to get involved about algorithm, just read the code. 
 *
 *   Building (I will generate a Makefile in the future)
 *     on Linux: compile it with gcc or clang (gcc tavla.c or clang tavla.c)
 *     on Windows: just compile it with your IDE (Dev-C++/mingw32 works fine)
 *     on Mac: never tried, but gcc/clang are able to build it.
 *
 *   Running
 *     on Linux: /path/to/source/a.out
 *     on Windows: just click twice
 * 
 *   Hacking
 *     github.com/ilkerkesen/tavla (will be available after homework deadline)
 */

/* --------------------------- INCLUDED LIBRARIES --------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* -------------------------- CONSTANT DEFINITONS --------------------------- */

#define PIECES 30
#define LOCATIONS 26
#define LINE 80
#define VERSION "0.1"

FILE *file_pointer;

/* -------------------------- FUNCTION PROTOTYPES --------------------------- */

/* Welcome and goodbye functions: prints information at start/exit */
void welcome();
void goodbye();

/* functions related to command-line interface/arguments */
void print_help(const char *);
void print_version();
void print_invalid(const char *);
void print_arg_error(const char *, const char *);
void set_logging_variables(int*, char*, char*, char *[]);

/* General game functions: options, states and initializations */
void select_game_options(int *, int *, int *, int);
void initialize_pieces(int [], int []);
void initialize_game(int *, int *, int, int, int *, int);
void update_scores(int, int, int *, int *, int *, int);
void game_over(int, int, int *, int);

/* Board drawing functions */
void print_board(int [], int [], char, char, int);
void print_side(const char *, int);
void print_info(int, int, int [], int, int, int, int, int, int, int);

/* Utilize functions for board drawing */
int get_maximum_piece(int [], int[]);
int get_lines_length(int);

/* Dice functions  */
void throw_dices(int *, int *);
void update_movements(int, int, int []);

/* Piece movement control functions */
int is_available_movement(int [], int [], int [], int, int, const char *);
int any_movements_exist(int [], int [], int [], const char *);

/* Piece movement action functions */
void move_piece(int [], int [], int, int, const char *);
int play_first_move(int [], int [], int [], const char *, int);

/* Turn control functions */
void play_turn(int [], int [], int, int, int [], int, int, int *, int, int, char, char, int);
void human_play(int [], int [], int, int, int [],
		int, int, const char *,char, char, int);
void computer_play_noob(int[], int [], int, int, int [], int, int, const char*, char, char, int);

/* ----------------------------- MAIN FUNCTION ----------------------------- */

int main(int argc, char *argv[])
{
/* ------------------------- VARIABLE DEFINITIONS -------------------------- */
  
  /* arrays that stores game/board state  */
  int black_pieces[26];
  int white_pieces[26];

  /* variables that stores score data */
  int black_score;
  int white_score;

  /* dice variables */
  int dice1;
  int dice2;
  
  /* movements rights caused by dice rolling process */
  int movements[4];

  /* turn % 2 == 0 means black's turn,
   * turn % 2 == 1 means white's turn */
  int turn;

  /* variables that store data whether player is human. */
  /* 0-> player is computer */
  /* 1-> player is human */
  int is_black_human; 
  int is_white_human;

  /* game maximum score */
  int score_limit;

  /* 0-> do not play, break main game loop */
  /* 1-> wanna play it again, i like it */
  int play_again;

  /* 1 -> logging enabled, 0 -> logging disabled */
  int logging;

  /* stores symbol for pieces */
  char black_symbol;
  char white_symbol;

  /* initialization for randomizing operations */
  srand(time(NULL));

  if(argc == 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
    print_version();
  }
  else if(argc == 2 &&
	  (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) {
    print_help(argv[0]);
  }
  else if(argc == 4 && (strlen(argv[1]) > 1 || strlen(argv[2]) > 1)) {
    print_arg_error(argv[1], argv[2]);
  }
  else if(argc == 4) {
    set_logging_variables(&logging, &black_symbol, &white_symbol, argv);
  }
  else if (argc > 1) {
    print_invalid(argv[0]);
  }
  else {
    black_symbol = 'B';
    white_symbol = 'W';
    logging = 0;
  }

  /* welcome function, writes information about game at start*/
  welcome();
  
  /* main loop */
  do {
    /* game type selection */
    select_game_options(&is_black_human, &is_white_human,
			&score_limit, logging);

    /* game initialization */
    initialize_game(&black_score, &white_score, dice1, dice2,
		    &turn, logging);

    /* game loop, it is over when a player's score equals 5 or more */
    while(black_score < score_limit && white_score < score_limit) {
      initialize_pieces(black_pieces, white_pieces);
      
      /* subgame loop. it finishes when a player collects his all pieces  */
      while(black_pieces[0] != 15 && white_pieces[25] != 15) {
	/* roll dices and update movement rights list */
	throw_dices(&dice1, &dice2);
	update_movements(dice1, dice2, movements);

	/* turn loop, check play_turn function */
	play_turn(black_pieces, white_pieces, dice1, dice2,
		  movements, black_score, white_score, &turn,
		  is_black_human, is_white_human,
		  black_symbol, white_symbol, logging);
      
      }
      /* update scores when subgame is over */
      update_scores(black_pieces[0], white_pieces[25],
		    &black_score, &white_score, &turn, logging);
    }
    /* game over, check game_over function */
    game_over(black_score, white_score, &play_again, logging);
  
    /* if user want to play again, then repeat iteration */
  } while(play_again == 1);

  /* goodbye function, check goodbye function */
  goodbye();

  /* close file */
  fclose(file_pointer);

  /* return 0, it is succesful */
  return 0;
}

/* ------------------------- FUNCTION DEFINITIONS -------------------------- */

void welcome()
{
  /* Prints information about game at start. */

  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n", 
	 "///////////////////////////////////////////////////",
	 "//                                               //",
	 "//   Tavla (backgammon) game, homework project   //",
	 "//   İlker Kesen, keseni_itu.edu.tr, 040100411   //",
	 "//                                               //",
	 "//   Howto play?                                 //",
	 "//   Piece: enter the location of the piece      //",
	 "//   Dice: enter the dice you want to play       //",
	 "//                                               //",
	 "///////////////////////////////////////////////////");

  printf("%s\n%s\n%s\n%s\n\n",
	 "Hint about broken pieces",
	 " If there are any broken pieces, then you must move",	 
	 " broken piece first. 25 is location for broken black",
	 " pieces, 0 is for broken white pieces.              ");
}

void goodbye()
{
  /* Prints informaton about game at exit. */

  printf("\nThanks for playing.\n");
}

void print_help(const char* file_name)
{
  /* print help information */

  printf("usage: %s\n", file_name);
  printf("       %s [options]\n", file_name);
  printf("       %s B W output.txt\n\n", file_name);
  printf("Parameters:\n");
  printf("  B - character that symbolizes black pieces on board\n");
  printf("  W - character that symbolizes white pieces on board\n");
  printf("  output.txt - output file, stores all the game states\n\n");
  printf("Options:\n");
  printf("  -h - this help text (also --help)\n");
  printf("  -v - version number (also --version)\n");

  exit(0);
}

void print_version()
{
  /* prints version information */

  printf("Tavla (backgammon) homework project, version %s\n", VERSION);

  exit(0);
}

void print_invalid(const char *file_name)
{
  printf("Invalid operation. Try `%s --help' for more information.\n", file_name);

  exit(0);
}

void print_arg_error(const char *arg1, const char *arg2)
{

  printf("Error.\n");

  if(strlen(arg1) > 1)
    printf("  - %s is not a character.\n", arg1);

  if(strlen(arg2) > 1)
    printf("  - %s is not a character.\n", arg2);

  exit(0);
}

void set_logging_variables(int *logging, char *black_symbol,
			   char *white_symbol, char *argv[])
{
  file_pointer = fopen(argv[3], "w");
  
  if(file_pointer == NULL) {
    *logging = 0;
  }
  else {
    *logging = 1;
    *black_symbol = argv[1][0];
    *white_symbol = argv[2][0];
  }
}

void select_game_options(int *is_black_human, int *is_white_human,
			 int *score_limit, int logging)
{
  /* Gets game options (game mode, color, score limit) from user and sets them. */

  /* 
   * game_type 0 -> human vs. human 
   *           1 -> human vs. computer
   *           2 -> computer vs. computer
   */
  int game_type;
  
  /* color: 0 -> black, 1 -> white */
  int color;

  do {
    printf("%s\n  %s\n  %s\n  %s\n%s",
	   "Select Game Type",
	   "0. Human vs. Human Game",
	   "1. Human vs. Computer Game",
	   "2. Computer vs. Computer Game",
	   "Enter game type (0, 1 or 2): ");
    
    /* get game type from user */
    scanf("%d", &game_type);

    /* if it is not valid, print warning */
    if(game_type != 0 && game_type != 1 && game_type != 2)
      printf("Invalid selection. Please try again.\n");

    printf("\n");
    
    /* if it is not valid, repeat it */
  } while(game_type != 0 && game_type != 1 && game_type != 2);

  if(game_type == 0) {
    /* both players are human */
    *is_black_human = 1;
    *is_white_human = 1;
  }
  else if(game_type == 1) {
    do {
      printf("%s\n  %s\n  %s\n%s",
	     "Select Color",
	     "0. Black",
	     "1. White",
	     "Enter color (0 or 1): ");

      /* get color data of human player from user */
      scanf("%d", &color);

      /* if it is wrong, then print warning */
      if(color != 0 && color != 1)
	printf("Invalid selection. Please try again.\n");

      printf("\n");

      /* do it until valid value entered */
    } while(color != 0 && color != 1);

    if(color == 0) {
      /* black is human, white is computer */
      *is_black_human = 1;
      *is_white_human = 0;
    }
    else if(color == 1) {
      /* black is computer, white is human */
      *is_black_human = 0;
      *is_white_human = 1;
    }

  }
  else if(game_type == 2) {
    /* both players are computer */
    *is_black_human = 0;
    *is_white_human = 0;
  }

  do {
    printf("Enter score limit (min: 1, max: 5): ");
    scanf("%d", score_limit);

    if(*score_limit < 1 || *score_limit > 5)
      printf("Invalid selection. Please try again.\n");

    printf("\n");

  } while(*score_limit < 1 || *score_limit > 5);

  if(logging == 1) {
    fprintf(file_pointer, "NEW GAME!\n\nOptions:\n  Game Type -> ");
    if(*is_black_human + *is_white_human == 2) 
      fprintf(file_pointer, "Human vs. Human");
    else if(*is_black_human + *is_white_human == 1) {
      if(*is_black_human == 1)
	fprintf(file_pointer, "Human (black) vs. Computer (white)");
      else if(*is_white_human == 1)
	fprintf(file_pointer, "Human (white) vs. Computer (black)");	
    }
    else if(*is_black_human + *is_white_human == 0)
      fprintf(file_pointer, "Computer vs. Computer");

    fprintf(file_pointer, "\n  Score Limit -> %d\n\n", *score_limit);
  }

}

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

  /* fill empty locations with zero  */
  int i;
  for(i = 0; i < LOCATIONS; i++) {
    if(i != 6 && i != 8 && i != 13 && i != 24) {
      black_pieces[i] = 0;
      white_pieces[LOCATIONS - 1 - i] = 0;
    }
  }
}

void initialize_game(int *black_score, int *white_score, int dice1, 
		     int dice2, int *turn, int logging)
{
  /* 
   * initializes main game:
   *   - set scores to zero
   *   - decides which player starts
   */

  /* set scores to zero */
  *black_score = 0;
  *white_score = 0;

  /* roll dices */
  do {
    printf("\nThrowing dices.");
    throw_dices(&dice1, &dice2);
    printf("\n%s%d\n%s%d\n",
	   "  Black: ", dice1,
	   "  White: ", dice2);
  } while(dice1 == dice2);

  if(logging == 1) {
    fprintf(file_pointer, "%s\n  %s%d\n  %s%d\n",
	    "Throwing dices.",
	    "Black: ", dice1,
	    "White: ", dice2);
  }

  if(dice1 > dice2) {
    /* if dice1 > dice2, then black starts */
    *turn = 0;
    printf("Black");

    if(logging == 1) {
      fprintf(file_pointer, "Black starts.\n");
    }
  }
  else {
    /* if dice1 < dice2, then white starts */
    *turn = 1;
    printf("White");

    if(logging == 1) {
      fprintf(file_pointer, "White starts.\n");
    }
  }
  
  printf(" starts.\n");
}

void game_over(int black_score, int white_score, int *play_again, int logging)
{
  /* prints "game over" */

  /* output */
  char output[LINE * 80];

  /* write scoreboard */
  sprintf(output, "\n%s\n  %s%d\n  %s%d\n",
	 "Game Over.",
	 "Black: ", black_score,
	 "White: ", white_score);

  if(black_score > white_score)
    /* 
     * if black score is greater than white 
     * score, then black player is winner 
     */
    strcat(output, "Black");
  else if(black_score < white_score)
    /* 
     * if white score is greater than black 
     * score, then white player is winner 
     */
    strcat(output, "White");

  strcat(output, " player won!\n\n");

  printf("%s", output);

  if(logging == 1)
    fprintf(file_pointer, "%s", output);

  /* get play_again value*/
  printf("If you want to play again, enter 1: ");
  scanf("%d", play_again);

  printf("\n");
}

void print_board(int black_pieces[], int white_pieces[],
		 char black_symbol, char white_symbol,
		 int logging)
{
  /* prints current state of board to screen */

  /* counters */
  int i, j, k;

  /* line as a string */
  char line[LINE];
  
  /* get number of board lines */
  int limit = get_lines_length(get_maximum_piece(black_pieces, white_pieces));

  /* write top of board */
  print_side("top", logging);
  
  /* write every line */
  for(i = 0; i < limit; i++) {
    strcpy(line, "");

    /* write left part of board (it is not a location)*/
    strcat(line, "||||");
    
    /* i know, one variable is enough, but it is more aesthetic */
    for(j = 12, k = 13; j > 0; j--, k++) {
      /* print spaces for (   B   W       B...)*/
      strcat(line, "   ");
      
      /* 
       * first_condition || second_condition
       *
       * pieces[k] -> number of pieces in k location
       * i -> line number 
       *
       * first_condition is for top board (locations from 13 to 24)
       *   - it is not difficult to understand.
       *   - if number of pieces in k location is greater than line number,
       *     then write B (or W).
       *   - ex. let k is 13 and pieces[k] = 5, it will print five times B (or W)
       *       for i = 0, 1, 2, 3, 4. as you can see, there must be 5 pieces.
       *
       * second_condition is for bottom board (locations from 12 to 1)
       *   - it is a little bit hard to understand, but solution is very practical.
       *   - if number of pieces in k location plus line number is greater than or
       *     equal to limit, then write B (or W).
       *   - ex. (IMPORTANT) let k is 12, pieces[k] = 5 and limit = 14, it will
       *     print five times B (or W) for i = 9 (5 + 9 >= 14, then print),
       *     i = 10 (5 + 10 >= 14, then print) ... i = 14 (5 + 14 >= 14, then print)
       *     as you can see, i = 8 (8 + 5 >= 14, it is not true, do not print)
       */

      if(black_pieces[k] > i || black_pieces[j] + i >= limit)
	sprintf(line, "%s%c", line, black_symbol); //printf("B");
      else if(white_pieces[k] > i || white_pieces[j] + i >= limit)
	sprintf(line, "%s%c", line, white_symbol); //printf("W");
      else
	strcat(line, " "); //printf(" ");

      /* print |||| middle of board row and right of board row */
      if(k == 18 || k == 24 || j == 7 || j == 1)
	strcat(line, "   ||||");//printf("%3s||||", "");
    }
    
    /* new line */
    printf("%s\n", line);
    
    if(logging == 1)
      fprintf(file_pointer, "%s\n", line);
  }

  /* write bottom of board */
  print_side("bottom", logging);
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

void print_side(const char *side, int logging)
{
  /* prints top/bottom part of board, location numbers */

  /* counters */
  int i;

  /* line as a string */
  char line[LINE];
  
  /* a string for formatted output */
  char fstr[LINE];

  /* if side is top */
  if(!strcmp(side, "top")) {
    sprintf(line, "%40sWhite Home Base\n", "");
    printf("%s", line);
    for(i = 13; i < 25; i++) {
      strcpy(line, "");

      /* print location numbers */
      if(i % 6 == 1) {
	sprintf(fstr, "%6s", "");
	strcat(line, fstr); //printf("%6s", "");

	if(i == 19)
	  strcat(line, " ");
      }

      sprintf(fstr, "%2d%2s", i, " ");

      strcat(line, fstr);
      printf("%s", line);    

      if(logging == 1)
	fprintf(file_pointer, "%s", line);
    }
    printf("\n");

    if(logging == 1)
      fprintf(file_pointer, "\n");
  }

  /* clean line string */
  strcpy(line, "");

  /* print ||||---|---|.... like board part */
  for(i = 0; i < 14; i++) {
    if(i % 7 == 0) {
      strcat(line, "|||"); //printf("|||");
    }
    strcat(line, "|---"); //printf("|---");
  }
  strcat(line, "||||"); //printf("||||\n");

  printf("%s\n", line);

  if(logging == 1)
    fprintf(file_pointer, "%s\n", line);
  
  /* if side is bottom */
  if(!strcmp(side, "bottom")) {
    strcpy(line, "");

    for(i = 12; i > 0; i--) {
      /* print location numbers */
      if(i % 6 == 0) {
	sprintf(fstr, "%6s", "");//printf("%6s", "");
	strcat(line, fstr);
	
	if(i == 6)
	  strcat(line, " ");//printf(" ");
      }
      sprintf(fstr, "%2d%2s", i, "");
      strcat(line, fstr);
      //printf("%2d%2s", i, "");    
    }
    printf("%s\n", line);

    sprintf(line, "%40sBlack Home Base\n", "");
    printf("%s", line);

    if(logging == 1)
      fprintf(file_pointer, "%s", line);
    //printf("%40sBlack Home Base\n", "");
  }
}

void print_info(int dice1, int dice2, int movements[], int black_score,
		int white_score, int broken_black, int collected_black,
		int broken_white, int collected_white,
		int logging)
{
  /* 
   * prints all data that do not seem on board
   * like broken/collected pieces, dices, movements 
   */

  /* counter */
  int i;

  /* a string for formatted two-lined outputs */
  char info[LINE * 2];

  sprintf(info, "\n%s%d%s%d%s%d%s%d %d\n%s%d%s%d%s%d%s",
	 "Black Score: ", black_score,
	 " Broken Blacks: ", broken_black, 
	 " Collected Blacks: ", collected_black, 
	 " Dices: ", dice1, dice2,
	 "White Score: ", white_score,
	 " Broken Whites: ", broken_white, 
	 " Collected Whites: ", collected_white, 
	 " Moves: ");

  for(i = 0; i < 4; i++) {
    if(movements[i] != 0) {
      sprintf(info, "%s%d ", info, movements[i]);
    }
  }

  sprintf(info, "%s\n\n", info);

  printf("%s", info);

  if(logging == 1)
    fprintf(file_pointer, "%s", info);
}

void throw_dices(int *dice1, int *dice2)
{
  /* roll dices */
  *dice1 = rand() % 6 + 1;
  *dice2 = rand() % 6 + 1;
}


void update_movements(int dice1, int dice2, int movements[])
{
  /* get movement rights from rolled dices */
  int i;
  
  if(dice1 == dice2) {
    for(i = 0; i < 4; i++) {
      movements[i] = dice1;
    }
  }
  else {
    movements[0] = dice1;
    movements[1] = dice2;
    movements[2] = 0;
    movements[3] = 0;
  }
}

int any_movements_exist(int black_pieces[], int white_pieces[],
			int movements[], const char *color)
{
  /* returns 0 if there are no valid movements, otherwise returns 1 */

  /* 
   * checks location user has just entered 
   * if there is no piece on this location
   * return 0
   */
  
  if(movements[0] == 0 && movements[1] == 0 &&
     movements[2] == 0 && movements[3] == 0)
    return 0;
 
  /* counters  */
  int i, j;

  /* 
   * check broken black pieces, if there is any valid 
   * movement for  broken black piece, then return 1
   */
  
  if(!strcmp(color, "black") && black_pieces[25] != 0) {
    for(i = 0; i < 4; i++) {
      if(is_available_movement(black_pieces, white_pieces,
			       movements, 25, movements[i],
			       color) && movements[i] != 0)
	return 1;
    }
  }

  /* 
   * check broken white pieces, if there is any valid
   * movement for broken white piece, then return 1
   */

  if(!strcmp(color, "white") && white_pieces[0] != 0) {
    for(i = 0; i < 4; i++) {
      if(movements[i] != 0 && 
	 is_available_movement(black_pieces, white_pieces,
			       movements, 0, movements[i], color))
	return 1;
    }
  }

  /*
   * if there is no broken pieces, then check other pieces
   * if there is a valid movement, then return 1
   */
  
  for(i = 1; i < LOCATIONS - 1; i++) {
    for(j = 0; j < 4; j++) {
      if(is_available_movement(black_pieces, white_pieces, 
			       movements, i, movements[j], color))
	return 1;
    }
  }

  return 0;
}

int is_available_movement(int black_pieces[], int white_pieces[],
			  int movements[], int location,
			  int movement, const char *color)
{
  int i;

  /* check movements */
  if(movement != movements[0] && movement != movements[1] && 
     movement != movements[2] && movement != movements[3])
    return 0;

  if(!strcmp(color, "black")) {
    /* black check location */
    if(black_pieces[location] == 0)
      return 0;

    /* black check broken */
    else if(black_pieces[25] != 0 && location != 25)
      return 0;

    /* black collecting */
    else if(location - movement < 1) {
      /* if there is a piece out of black home, return 0 */
      for(i = 7; i < LOCATIONS; i++) {
	if(black_pieces[i] != 0)
	  return 0;
      }
      
      /* 
       * it is for that scenario.
       * black is collecting but dices are 6 and 6.
       * there is no piece at 6th location.
       * but there are pieces on 5th location.
       * than player can move the piece on 5th location.
       */

      if(movement > location) {
	for(i = location + 1; i < 7; i++) {
	  if(black_pieces[i] != 0)
	    return 0;
	}
      }
      
    }
    
    /* black standart movement */
    else if(white_pieces[location - movement] > 1)
      return 0;
      
  }
  else if(!strcmp(color, "white")) {
    /* white check location */
    if(white_pieces[location] == 0)
      return 0;

    /* white check broken */
    else if(white_pieces[0] != 0 && location != 0)
      return 0;

    /* white collecting */
    else if(location + movement > 25) {
      /* if there is a piece out of white home, return 0 */
      for(i = 18;  i > -1; i--) {
	if(white_pieces[i] != 0)
	  return 0;
      }

      /* 
       * it is for that scenario.
       * white is collecting but dices are 6 and 6.
       * there is no piece at 18th location.
       * but there are pieces on 19th location.
       * than player can move the piece on 19th location.
       */

      if(location + movement > 24) {
	for(i = location - 1; i > 18; i--) {
	  if(white_pieces[i] != 0)
	    return 0;
	}
      }
    }

    /* white standart movement */
    else if(black_pieces[location + movement] > 1) {
      return 0;
    }
  }
  
  return 1;
}

void play_turn(int black_pieces[], int white_pieces[],
	       int dice1, int dice2, int movements[],
	       int black_score, int white_score,
	       int *turn, int is_black_human, int is_white_human,
	       char black_symbol, char white_symbol,
	       int logging)
{
  /* decides who is playing */

  if(is_black_human == 1 && *turn % 2 == 0) {
    human_play(black_pieces, white_pieces, dice1, dice2, 
	       movements, black_score, white_score, "black",
	       black_symbol, white_symbol, logging);
  }

  if(is_black_human == 0 && *turn %2 == 0) {
    computer_play_noob(black_pieces, white_pieces, dice1, dice2,
		       movements, black_score, white_score, "black",
		       black_symbol, white_symbol, logging);
  }

  if(is_white_human == 1 && *turn % 2 == 1) {
    human_play(black_pieces, white_pieces, dice1, dice2, 
	       movements, black_score, white_score, "white",
	       black_symbol, white_symbol, logging);
  }

  if(is_white_human == 0 && *turn % 2 == 1) {
    computer_play_noob(black_pieces, white_pieces, dice1, dice2,
		       movements, black_score, white_score, "white",
		       black_symbol, white_symbol, logging);
  }

  *turn = *turn + 1;
}

void human_play(int black_pieces [], int white_pieces [], 
		int dice1, int dice2, int movements [],
		int black_score, int white_score, const char *color,
		char black_symbol, char white_symbol,
		int logging)
{
  /* turn function for human players */

  /* counters */
  int i;

  /* line as a string for screen/file output */
  char output[LINE];

  /* user input variables */
  int location;
  int movement;

  /* if there is no available movement, than tell it to player */
  if(!any_movements_exist(black_pieces, white_pieces, movements, color)) {
    print_board(black_pieces, white_pieces,
		black_symbol, white_symbol, logging);
    print_info(dice1, dice2, movements, black_score, white_score,
	       black_pieces[25], black_pieces[0], white_pieces[0],
	       white_pieces[25], logging);
    sprintf(output, "%s's turn. No available movement.\n\n", color);
    
    printf("%s", output);

    if(logging == 1)
      fprintf(file_pointer, "%s", output);
  }

  /* while there is a valid movement, then let player play */
  while(any_movements_exist(black_pieces, white_pieces, movements, color)) {
    print_board(black_pieces, white_pieces, 
		black_symbol, white_symbol,	logging);
    print_info(dice1, dice2, movements, black_score, white_score,
	       black_pieces[25],black_pieces[0], white_pieces[0],
	       white_pieces[25], logging);
    
    do {
      strcpy(output, "");
      sprintf(output, "%s's turn.\n", color);
      sprintf(output, "  Enter piece location: ");

      printf("%s", output);

      scanf("%d", &location);

      if(logging == 1)
	fprintf(file_pointer, "%s%d", output, location);

      strcpy(output, "");
      sprintf(output, "  Enter movement: ");
      printf("%s", output);
      scanf("%d", &movement);

      if(logging == 1)
	fprintf(file_pointer, "%s%d", output, movement);

      /* if user input is not valid, warn player */
      if(!is_available_movement(black_pieces, white_pieces, 
				movements, location, movement, color)) {
	sprintf(output, "Invalid movement. Please try again.\n\n");
	printf("%s", output);

	if(logging == 1)
	  fprintf(file_pointer, "%s", output);
      }

      /* if it is not a valid movement, than repeat it */
    } while(!is_available_movement(black_pieces, white_pieces, movements,
				   location, movement, color));
    
    /* move piece */
    move_piece(black_pieces, white_pieces, location, movement, color);

    /* set played movement value to zero */
    for(i = 0; i < 4; i++) {
      if(movements[i] == movement) {
	movements[i] = 0;
	break;
      }
    }

  }
}
void computer_play_noob(int black_pieces [], int white_pieces [], int dice1,
			int dice2, int movements [], int black_score, 
			int white_score, const char *color,
			char black_symbol, char white_symbol,
			int logging)
{
  /* computer plays his turn and computer is a total noob. */

  /* output as a string for formatted output to screen/file */
  char output[LINE];

  /* if there is no valid movements, than tell it to user */
  if(!any_movements_exist(black_pieces, white_pieces, movements, color)) {
    print_board(black_pieces, white_pieces,
		black_symbol, white_symbol,
		logging);
    print_info(dice1, dice2, movements, black_score, white_score,
	       black_pieces[25], black_pieces[0], white_pieces[0],
	       white_pieces[25], logging);
    sprintf(output, "%s's turn. No available movement.\n\n", color);
    printf("%s", output);

    if(logging == 1)
      fprintf(file_pointer, "%s", output);
  }

  /* while there is valid movement */
  while(any_movements_exist(black_pieces, white_pieces, movements, color)) {
    print_board(black_pieces, white_pieces,
		black_symbol, white_symbol,
		logging);
    print_info(dice1, dice2, movements, black_score, white_score,
	       black_pieces[25], black_pieces[0], white_pieces[0], 
	       white_pieces[25], logging);

    /* find a valid movement and play it */
    play_first_move(black_pieces, white_pieces, movements, color, logging);
  }
}

int play_first_move(int black_pieces[], int white_pieces[],
		    int movements[], const char *color,
		    int logging)
{
  /* plays first valid movement */

  /* counters */
  int i;
  int j;
  int k;

  /* output string for formatted output to screen/file */
  char output[LINE * 3];
  
  sprintf(output, "%s's turn.\n", color);

  /* check all locations */
  for(i = 0, j = 25; i < LOCATIONS; i++, j--) {
    /* check all movements */
    for(k = 0; k < 4; k++) {
      /* if computer player is black */
      if(!strcmp(color, "black")) {
	/* if the movement is valid */
	if(is_available_movement(black_pieces, white_pieces, movements,
				 j, movements[k], color) && movements[k] != 0) {
	  sprintf(output, "%s  %s%d\n  %s%d\n", output,
		  "  Enter location: ", j,
		  "  Enter movement: ", movements[k]);

	  printf("%s", output);

	  if(logging == 1)
	    fprintf(file_pointer, "%s", output);

	  move_piece(black_pieces, white_pieces, j, movements[k], color);
	  movements[k] = 0;
	  return 1;
	}
      }

      /* if computer player is white */
      else if(!strcmp(color, "white")) {
	/* if the movement is valid */
	if(is_available_movement(black_pieces, white_pieces, movements,
				 i, movements[k], color) && movements[k] != 0) {	
	  sprintf(output, "%s  %s%d\n  %s%d\n", output,
		 "Enter location: ", i,
		 "Enter movement: ", movements[k]);

	  printf("%s", output);

	  if(logging == 1)
	    fprintf(file_pointer, "%s", output);

	  move_piece(black_pieces, white_pieces, i, movements[k], color);
	  movements[k] = 0;
	  return 1;
	}
      }
    }
  }

  return 0;
}

void move_piece(int black_pieces [], int white_pieces [],
		int location, int movement, const char *color)
{
  /* moves piece */

  /* for black pieces */
  if(!strcmp(color, "black")) {
    /* decrease piece number of location of selected piece (-1) */
    black_pieces[location] -= 1;
    
    /* if player is collecting (see is_available_movement function )*/
    if(movement > location) {
      black_pieces[0] += 1;
    }
    else {
      /* increase piece number of new location */
      black_pieces[location - movement] += 1;

      /* if there is an enemy piece, then hit it */
      if(white_pieces[location - movement] == 1) {
	white_pieces[location - movement] = 0;
	white_pieces[0] += 1;
      }
    }
  }
  
  /* for white pieces */
  if(!strcmp(color, "white")) {
    /* decrease piece number of location of selected piece (-1) */
    white_pieces[location] -= 1;

    /* if player is collecting (see is_available_movement function )*/
    if(movement + location > 25) {
      white_pieces[25] += 1;
    }
    else {
      /* increase piece number of new location */
      white_pieces[location + movement] += 1;

      /* if there is an enemy piece, then hit it */
      if(black_pieces[location + movement] == 1) {
	black_pieces[location + movement] = 0;
	black_pieces[25] += 1;
      }
    }
  }
}

void update_scores(int collected_black_pieces, int collected_white_pieces,
		   int *black_score, int *white_score, int *turn, 
		   int logging)
{
  /* updates scores */

  /* output string for formatted output to screen/file */
  char output[LINE * 2];

  /* check collected black pieces */
  if(collected_black_pieces == 15) {
    /* increase black score */
    *black_score += 1;
    strcpy(output, "Black");
    /* check mars: double game points */
    if(collected_white_pieces == 0)
      *black_score += 1;

    /* next game's first turn will be black's */
    *turn = 0;
  }

  /* check collected white pieces*/
  if(collected_white_pieces == 15) {
    /* increase white score */
    *white_score += 1;
    strcpy(output, "White");
    /* check mars: double game points */
    if(collected_black_pieces == 0)
      *white_score += 1;
    /* next game's first turn will be white's */
    *turn = 1;
  }

  sprintf(output, "%s%s%d, %s%d\n",
	  output, "scores! Black: ", *black_score, "White: ", *white_score);

  printf("%s", output);

  if(logging == 1)
    fprintf(file_pointer, "%s", output);
}

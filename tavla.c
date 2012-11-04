#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* CONSTANT DEFINITIONS */

#define PIECES 30
#define LOCATIONS 26

/* FUNCTION DECLERATIONS */

void welcome();
void goodbye();

void initialize_pieces(int [], int []);
void initialize_game(int*, int*, int, int, int*);

void print_board(int [], int []);
int get_maximum_piece(int [], int[]);
int get_lines_length(int);
void print_side(const char*);
void print_info(int, int, int, int [], int, int, int, int);

void throw_dices(int*, int*);
void update_movements(int, int, int []);
int any_movements_exist(int [], int [], int [], const char*);
int is_avaible_move(int [], int [], int, int, const char*);

void play_turn(int [], int [], int, int, int [], int*, int, int);
void human_play(int [], int [], int, int, int [], const char*);
void computer_play(int [], int [], int, int, int [], const char*);
void move(int [], int [], int, int);
void update_scores(int, int, int*, int*);

/* MAIN FUNCTION */

int main(int argc, char *argv[])
{
  /* VARIABLE DEFINITIONS */
  int black_pieces[26];
  int white_pieces[26];

  int black_score;
  int white_score;

  int dice1, dice2;
  int turn; /* turn % 2 == 0 means black's turn, turn % 2 == 1 means white's turn */
  int movements[4]; /* movements rights caused by dice rolling process */

  int is_black_human = 1; 
  int is_white_human = 1;

  char response;

  srand(time(NULL));

  welcome();
  
  printf("Do you want to play? [y/n]: ");
  scanf("%c", &response);

  while(response == 'y') {
    /* game initialization */
    initialize_game(&black_score, &white_score, dice1, dice2, &turn);

    while(black_score < 5 && white_score < 5) {
      initialize_pieces(black_pieces, white_pieces);
      while(black_pieces[0] != 15 || white_pieces[25] != 15) {
	throw_dices(&dice1, &dice2);
	update_movements(dice1, dice2, movements);
	play_turn(black_pieces, white_pieces, dice1, dice2, movements, &turn, is_black_human, is_white_human);
      }
      update_scores(black_pieces[0], white_pieces[25], &black_score, &white_score);
    }
  }

  goodbye();
  return 0;
}

/* FUNCTION DEFINITIONS */

void welcome()
{
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
}

void goodbye()
{
  printf("\nThanks for playing.\n");
}

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

void initialize_game(int* black_score, int* white_score, int dice1, int dice2, int* turn)
{
    *black_score = 0;
    *white_score = 0;
    
    do {
      printf("\nThrowing dices.");
      throw_dices(&dice1, &dice2);
      printf("\n%s%d\n%s%d\n",
	     "  Black (human): ", dice1,
	     "  White (computer): ", dice2);
    } while(dice1 == dice2);

    if(dice1 > dice2) {
      *turn = 0;
      printf("Black");
    }
    else {
      *turn = 1;
      printf("White");
    }

    printf(" starts.\n");
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

  printf("\n%s%d %s%d %s%d %s%d\n\n",
	 "Broken Black: ", black_pieces[25],
	 "Collected Black: ", black_pieces[0],
	 "Broken White: ", white_pieces[0],
	 "Collected White: ", white_pieces[25]);

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

void print_info(int turn, int dice1, int dice2, int movements[], int broken_black, int collected_black, int broken_white, int collected_white)
{
  NULL;
}

void throw_dices(int* dice1, int* dice2)
{
  *dice1 = rand() % 6 + 1;
  *dice2 = rand() % 6 + 1;
}


void update_movements(int dice1, int dice2, int movements[])
{
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

int any_movements_exist(int black_pieces[], int white_pieces[], int movements[], const char* color)
{
  if(movements[0] == 0 && movements[1] == 0 && movements[2] == 0 && movements[3] == 0)
    return 0;

  int i, j;

  for(i = 0; i < LOCATIONS; i++) {
    for(j = 0; j < 4; j++) {
      if(is_available_movement(black_pieces, white_pieces, i, movements[j], color))
	return 1;
    }
  }

  return 0;
}

int is_available_movement(int black_pieces[], int white_pieces[], int location, int movement, const char* color)
{
  int i;

  /* black check location */
  if(color == "black" && black_pieces[location] == 0)
    return 0;

  /* black check broken */
  if(color == "black" && black_pieces[25] != 0 && location != 25)
    return 0;

  /* black collecting */
  if(color == "black" && location - movement <= 0) {

    for(i = 7; i < LOCATIONS; i++) {
      if(black_pieces[i] != 0)
	return 0;
    }

    if(movement > location) {
      for(i = location; i < 7; i++) {
	if(black_pieces[i] != 0)
	  return 0;
      }
    }

  }

  /* black standart movement */
  if(color == "black" && white_pieces[location - movement] >= 2)
    return 0;


  /* white check location */
  if(color == "white" && white_pieces[location] == 0)
    return 0;

  /* white check broken */
  if(color == "white" && white_pieces[0] != 0 && location != 0)
    return 0;

  /* white collecting */
  if(color == "white" && location + movement >= 25) {
    for(i = 18;  i > -1; i--) {
      if(white_pieces[i] != 0)
	return 0;
    }

    if(location + movement > 25) {
      for(i = location; i > 18; i--) {
	if(white_pieces[i] != 0)
	  return 0;
      }
    }
  }

  /* white standart movement */
  if(color == "white" && black_pieces[location + movement] >= 2)
    return 0;

  return 1;
}

void play_turn(int black_pieces[], int white_pieces[], int dice1, int dice2, int movements[], int* turn, int is_black_human, int is_white_human)
{
  if(is_black_human && *turn % 2 == 0) {
    human_play(black_pieces, white_pieces, dice1, dice2, movements, "black");
  }

  if(!is_black_human && *turn %2 == 0) {
    computer_play(black_pieces, white_pieces, dice1, dice2, movements, "black");
  }

  if(is_white_human && *turn % 2 == 1) {
    human_play(white_pieces, black_pieces, dice1, dice2, movements, "white");
  }

  if(!is_white_human && *turn % 2 == 1) {
    computer_play(white_pieces, black_pieces, dice1, dice2, movements, "black");
  }

  *turn++;
}

void human_play(int black_pieces [], int white_pieces [], int dice1, int dice2, int movements [], const char* color)
{
  NULL;
}
v
void computer_play(int black_pieces [], int white_pieces [], int dice1, int dice2, int movements [], const char* color)
{
  NULL;
}

void move(int ally_pieces [], int enemy_pieces [], int location, int new_location)
{
  NULL;
}

void update_scores(int collected_black_pieces, int collected_white_pieces, int* black_score, int* white_score)
{
  if(collected_black_pieces == 15) {
    *black_score++;
    if(collected_white_pieces == 0) /* mars */
      *black_score++;
  }

  if(collected_white_pieces == 15) {
    *white_score++;
    if(collected_black_pieces == 0) /* mars */
      *white_score++;
  }
}

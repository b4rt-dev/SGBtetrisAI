/*
 * START OF TETRIS ALG
 */

#include "string.h"
#include "limits.h"

//Size of game field
#define GRID_W              (10)
#define GRID_H              (18)

//Max size of each tetris piece
#define PIECE_W             (4)
#define PIECE_H             (4)

//How many kinds of pieces
#define NUM_PIECE_TYPES     (7)

//Size of list with possible positions
#define POS_LIST_SIZE (GRID_W*8)

const boolean pauzeMenu[INPUTSIZE-8] = {
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,1,1,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0
};

const boolean gameOverMenu[INPUTSIZE-8] = {
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,1,1,1,0,0
};

const boolean mainMenu[INPUTSIZE-8] = {
  0,0,0,0,1,1,1,1,1,1,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  1,0,0,1,0,1,1,0,0,1,
  0,0,0,1,0,0,0,0,0,1,
  0,1,0,1,0,1,0,1,0,1,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,1,1,
  0,0,0,0,0,0,0,0,0,0

};

//Definitions of the pieces in tetris
//Each piece is max 4 wide, 4 tall, and 4 rotations.
//Adapted to GB version
const char piece_I[] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,

  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
};

const char piece_L[] = {
  0, 0, 0, 0,
  1, 1, 1, 0,
  1, 0, 0, 0,
  0, 0, 0, 0,

  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 0, 1, 0,
  1, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,
};

const char piece_J[] = {
  0, 0, 0, 0,
  1, 1, 1, 0,
  0, 0, 1, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,

  1, 0, 0, 0,
  1, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
};

const char piece_T[] = {
  0, 0, 0, 0,
  1, 1, 1, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  1, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
};

const char piece_S[] = {
  0, 0, 0, 0,
  0, 1, 1, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,

  1, 0, 0, 0,
  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  0, 1, 1, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,

  1, 0, 0, 0,
  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
};

const char piece_Z[] = {
  0, 0, 0, 0,
  1, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  1, 1, 0, 0,
  1, 0, 0, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  1, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  1, 1, 0, 0,
  1, 0, 0, 0,
  0, 0, 0, 0,
};

const char piece_O[] = {
  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,
};

//Index array of pieces
const char *pieces[NUM_PIECE_TYPES] = {
  piece_S,
  piece_Z,
  piece_L,
  piece_J,
  piece_O,
  piece_T,
  piece_I,
};

//Helper array for converting piece id to block name
const char blockname[NUM_PIECE_TYPES] = {
  'S', 'Z', 'L', 'J', 'O', 'T', 'I'
};

//Definition of datastructure that describes a position of a piece
struct Piece_position {
  int x; //x of piece
  int y; //y of piece
  int r; //rotation of piece
};

//2D Board array, without falling block
boolean grid[GRID_W][GRID_H];

//2D Board array, without falling block
boolean grid_with_block[GRID_W][GRID_H];

//The 8 pixels sent over from FPGA that represent the next piece
boolean piecePixels[8];

//Virtual buttons for AI
int sideways = 0; //left: -1, right: 1, no movement: 0
int rotation = 0; //rotate: 1, no rotation: 0
int softdrop = 0; //softdrop: 1, no softdrop: 0

//Current piece information
int piece_id;
int piece_rotation;
int piece_x;
int piece_y;

//Optimal placement for current piece
int piece_rotation_target;
int piece_x_target;
int piece_y_target;

//States of game
//boolean waitForFrame = true;      //when waiting for next frame
boolean inMenu = false;             //when in menu, not in game
boolean newpiece = false;           //when there is a new piece on row 17&18
boolean waitingForNewPiece = false; //waiting for new piece to show up
boolean placingPiece = false;       //when placing the current piece
boolean plan = false;               //if plan is calculated

boolean newpiece_prev = false;      //the value of newpiece on previous frame

//List of possible positions
Piece_position pos_list[POS_LIST_SIZE] = {};

//Weight vars
double w_heights    = -0.91;
double w_bumpiness  = -0.26;
double w_holes      = -0.16;
double w_lines      =  0.85;

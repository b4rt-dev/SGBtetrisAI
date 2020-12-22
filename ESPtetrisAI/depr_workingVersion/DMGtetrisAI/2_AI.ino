
/*States of game
boolean waitForFrame = true;        //when waiting for next frame
boolean inMenu = false;             //when in menu, not in game
boolean newpiece = false;          //when there is a new piece on row 17&18
???boolean waitingForNewPiece = false; //waiting for new piece to show up
???boolean placingPiece = false;       //when placing the current piece
boolean plan = false;               //if plan is calculated
*/




int start_counter = 10;

void play()
{
  //first: reset the input
  sideways = 0;
  rotation = 0;
  softdrop = 0;

  if (startcounter > 0)
  {
    startcounter--;
    //start = !start;
     updateButtons();
  }
  else
  {
 
  
    //decode received data
    decodeData();
  
    
    //get current state
    updateStates();
    
    
    
      if(inMenu)
      {
        if (start_counter < 10)
        {
          start = false;
          start_counter++;
        }
        else
        { 
          start_counter = 0;
          start = true;
        }
      }
      else //in game
      {
        start = false;
        if (newpiece)
        {
          
          if (!newpiece_prev)
          {
            //Serial.println(blockname[piece_id]);
            ai_generate_plan();
            plan = true;
            softdrop = true;
          }
          else
          {
            softdrop = true;
          }
          newpiece_prev = true;
        }
        else //not new piece
        {
          newpiece_prev = false;
    
          //even frame number
          if ((frameCounter & 0x01) == 0)
          {
          
            if (plan)
            {
              ai_execute_plan();
            }
            else
            {
              if (safe_softdrop())
              {
                //Serial.println("s");
                softdrop = 1;
              }
              else
              {
                softdrop = 0;
              }
            }
          }
          else
          {
            sideways = 0;
            rotation = 0;
            //softdrop = 0;
            startDelay++;
            start = false;
            convertInput();
          }
        }
      }
      
    convertInput();
    updateButtons();
  
  }
}


//check in what state we are in
void updateStates()
{
  checkMenuState();
  checkNewPieceState();
}

void checkNewPieceState()
{
  boolean startPixels[8];

  startPixels[0] = grid_with_block[3][1];
  startPixels[1] = grid_with_block[4][1];
  startPixels[2] = grid_with_block[5][1];
  startPixels[3] = grid_with_block[6][1];
  startPixels[4] = grid_with_block[3][2];
  startPixels[5] = grid_with_block[4][2];
  startPixels[6] = grid_with_block[5][2];
  startPixels[7] = grid_with_block[6][2];

  //check if all 8 start pixels define a legit piece, 
  //  which should only be the case if there is a new piece
  int pieceByte = 0; //bits to byte for faster checking
  for(int i=0; i<8; i++){
     pieceByte |= startPixels[i] << i;
  }
  //Serial.println(pieceByte);

  switch (pieceByte)
  {
    case 15: //I
      newpiece = true;
      piece_id = 6;
      piece_x = 3;
      piece_y = -1;
      piece_rotation = 0;
      break;
      
    case 71: //J
      newpiece = true;
      piece_id = 3;
      piece_x = 3;
      piece_y = 0;
      piece_rotation = 0;
      break;

    case 23: //L
      newpiece = true;
      piece_id = 2;
      piece_x = 3;
      piece_y = 0;
      piece_rotation = 0;
      break;

    case 54: //S
      newpiece = true;
      piece_id = 0;
      piece_x = 3;
      piece_y = 0;
      piece_rotation = 0;
      break;

    case 99: //Z
      newpiece = true;
      piece_id = 1;
      piece_x = 3;
      piece_y = 0;
      piece_rotation = 0;
      break;

    case 39: //T
      newpiece = true;
      piece_id = 5;
      piece_x = 3;
      piece_y = 0;
      piece_rotation = 0;
      break;

    case 102: //O
      newpiece = true;
      piece_id = 4;
      piece_x = 3;
      piece_y = 0;
      piece_rotation = 0;
      break;
         
    default:
      newpiece = false;
      break;
      
  }
 
}

//int tmp = 0;

void checkMenuState()
{
  inMenu = false;
  
  //check board against constant menu frames
  boolean inPauzeMenu = true;
  boolean inGameOverMenu = true;
  boolean inMainMenu = true;
  boolean noBlock = true;

  /*if (tmp < 100)
  {
    tmp++;
  }
  else
  {
    tmp = 0;
    for (int i=0; i< INPUTSIZE; i++)
    {
      Serial.print(rdata[i]);
    }
    Serial.println();
  }*/
  
  
  for (int x = 0; x < GRID_W; x++)
  {
    for (int y = 0; y < GRID_H; y++)
    {
      if (pauzeMenu[y*GRID_W + x] != grid_with_block[x][y])
      {
        inPauzeMenu = false;
      }
      if (gameOverMenu[y*GRID_W + x] != grid_with_block[x][y])
      {
        inGameOverMenu = false;
      }
      if (mainMenu[y*GRID_W + x] != grid_with_block[x][y])
      {
        inMainMenu = false;
      }
    }
  }

  for (int i = 0; i < 8 ; i++)
  {
    if (piecePixels[i])
    {
      noBlock = false;
    }
  }  
    
  if (inPauzeMenu || inGameOverMenu || inMainMenu || noBlock)
  {
    /*
    if (inPauzeMenu)
      Serial.println("pauze");
    if (inGameOverMenu)
      Serial.println("gameover");
    if (inMainMenu)
      Serial.println("main");
    if (noBlock)
      Serial.println("noBlock");
    */

    /*
    for (int i = 0; i < INPUTSIZE; i++)
    {
      Serial.print(rdata[i]);
    }
    Serial.println();
    */
    inMenu = true;
  }

}

//convert sideways, rotation and softdrop into button presses/pulses
void convertInput()
{
  if (sideways == -1)
  {
    l = true;
    r = false;
  }
  if (sideways == 1)
  {
    l = false;
    r = true;
  }
  if (sideways == 0)
  {
    l = false;
    r = false;
  }
  if (rotation == 1)
  {
    a = true;
  }
  if (rotation == 0)
  {
    a = false;
  }
  if (softdrop == 1)
  {
    d = true;
  }
  if (softdrop == 0)
  {
    d = false;
  }
}

void decodeData()
{
  for (int i = INPUTSIZE-8; i < INPUTSIZE; i++)
  {
    
    if (rdata[i])
    {
      piecePixels[i-(INPUTSIZE-8)] = true;
    }
    else
    {
      piecePixels[i-(INPUTSIZE-8)] = false;
    }
 }


  for (int x = 0; x < GRID_W; x++)
  {
    for (int y = 0; y < GRID_H; y++)
    {
      if (rdata[y*GRID_W + x])
      {
        grid_with_block[x][y] = true;
        //Serial.print(1);

      }
      else
      {
        grid_with_block[x][y] = false;
        //Serial.print(0);
      }
    }
    //Serial.println();
  }  
  //Serial.println();
  
}

int piece_can_fit(int px, int py, int pr) {
  if ( piece_off_edge(px, py, pr) ) return 0;
  if ( piece_hits_rubble(px, py, pr) ) return 0;
  return 1;
}

int piece_off_edge(int px, int py, int pr) {
  int x, y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      int ny = py + y;
      if (ny < 0) continue; //Off top, don't care
      if (piece[y * PIECE_W + x] > 0) {
        if (nx < 0) return 1; //Yes: off left side
        if (nx >= GRID_W ) return 1; //Yes: off right side
      }
    }
  }
  return 0;  //Inside limits
}

int piece_hits_rubble(int px, int py, int pr) {
  int x, y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    int ny = py + y;
    if (ny < 0) continue;
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      if (piece[y * PIECE_W + x] > 0) {
        if (ny >= GRID_H ) return 1; //Yes: goes off bottom of grid
        if (grid[nx][ny] != 0 ) return 1; //Yes: grid already full in this space
      }
    }
  }
  return 0;  //Doesn't hit
}

void ai_calculate_all_placements()
{
  //clear the current poslist
  for (int i = 0; i < POS_LIST_SIZE; i++)
  {
    pos_list[i] = {};
  }

  //compute all possible locations for the next block
  int pos_counter = 0;
  int px, py, pr;
  for (pr = 0; pr < 4; pr++) {
    for (px = -4; px < GRID_W; px++) {
      for (py = 0; py < GRID_H; py++) {
        //check if it is a valid resulting place
        if (piece_can_fit(px, py, pr) && !piece_can_fit(px, py + 1, pr))
        {
          //check if place can be reached by just falling from above
          boolean no_rubble_above = true;
          for (int y = py; y > 0; y--)
          {
            if (!piece_can_fit(px, y, pr))
            {
              no_rubble_above = false;
            }
          }
          if (no_rubble_above)
          {
            //then it is a valid placement, so store it in the poslist
            pos_list[pos_counter].x = px;
            pos_list[pos_counter].y = py;
            pos_list[pos_counter].r = pr;
            pos_counter++;
          }
        }
      }
    }
  }

  /*
  for (int i = 0; i < POS_LIST_SIZE; i++)
  {
    if (!(pos_list[i].x == 0 && pos_list[i].y == 0 && pos_list[i].r == 0))
    {
      Serial.print(pos_list[i].x);
      Serial.print(' ');
      Serial.print(pos_list[i].y);
      Serial.print(' ');
      Serial.print(pos_list[i].r);
      Serial.println();
    }
  }
  Serial.println();
  */
}

//adds piece (eg. from poslist) to a board, so we can calculate the score
void add_custom_piece_to_grid(boolean newgrid[GRID_W][GRID_H], int px, int py, int pr) {
  int x, y;

  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      int ny = py + y;
      if (ny < 0 || ny > GRID_H) continue;
      if (nx < 0 || nx > GRID_W) continue;
      if (piece[y * PIECE_W + x] == 1) {
        newgrid[nx][ny] = 1;
      }
    }
  }
}

//calculate scores for all positions
void ai_calculate_scores(double scores[POS_LIST_SIZE]) {
  //Serial.println();
  //Serial.println();
  for (int i = 0; i < POS_LIST_SIZE; i++)
  {
    scores[i] = 0;
    if (!(pos_list[i].x == 0 && pos_list[i].y == 0 && pos_list[i].r == 0))
    {
      //copy of board for calculation purposes
      boolean gridcopy[GRID_W][GRID_H];

      //create board with piece
      for (int j = 0; j < GRID_W; j++)
        memcpy(gridcopy[j], grid[j], sizeof(grid[j]));

      add_custom_piece_to_grid(gridcopy, pos_list[i].x, pos_list[i].y, pos_list[i].r);

      /*
      for (int y = 0; y < GRID_H; y++)
      {
        for (int x = 0; x < GRID_W; x++)
        {
          Serial.print(gridcopy[x][y]);
        }
        Serial.println();
      }
      Serial.println();
      */

      scores[i] = calculate_score(gridcopy);
    }
  }
}

//select pos with highest score
int ai_select_best_placement(double scores[POS_LIST_SIZE]) {
  double highest_score = INT_MIN;
  int best_pos_index = 0;
  for (int i = 0; i < POS_LIST_SIZE; i++)
  {
    if (!(pos_list[i].x == 0 && pos_list[i].y == 0 && pos_list[i].r == 0))
    {
      if (scores[i] > highest_score)
      {
        highest_score = scores[i];
        best_pos_index = i;
      }
    }
  }
  return best_pos_index;
}

boolean safe_softdrop()
{

  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  int xlist[4];
  int counter = 0;
  //get x positions of current block
  for (int y = 0; y < PIECE_H; ++y) {
    for (int x = 0; x < PIECE_W; ++x) {
      int nx = piece_x + x;
      int ny = piece_y + y;
      if (ny < 0 || ny > GRID_H) continue;
      if (nx < 0 || nx > GRID_W) continue;
      if (piece[y * PIECE_W + x] == 1) {
        xlist[counter] = nx;
        counter++;
      }
    }
  }
/*
  for (int i = 0; i < 4; i++)
  {
    Serial.print(xlist[i]);
    Serial.print(' ');
  }
  Serial.println();
*/


  int holes = 0;
  for (int x = 0; x < GRID_W; x++)
  {
    if (x == xlist[0] || x == xlist[1] || x == xlist[2] || x == xlist[3])
    {
      int col_holes = 0;
      boolean found_block = false;
      for (int y = 0; y < GRID_H; y++)
      {
        if (grid[x][y])
        {
          found_block = true;
        }
        else
        {
          if (found_block)
          {
            col_holes = col_holes + 1;
          }
        }
      }
      holes = holes + col_holes;
    }
  }
  if (holes > 0)
  {
    boolean isRowFull[GRID_H];

    for (int y = 0; y < GRID_H; y++)
    {
      boolean empty = true;
      for (int x = 0; x < GRID_W; x++)
      {
        {
          if (grid_with_block[x][y])
          {
            empty = false;
          }
        }
      }
      isRowFull[y] = !empty;
    }
  
  /*
    for (int y = 0; y < GRID_H; y++)
    {
      Serial.println(isRowFull[y]);
    }*/
    boolean safe = false;
    boolean emptyFound = false;
    boolean secondEmptyFound = false;
    for (int y = GRID_H -1; y >=0 ; y--)
    {
      //Serial.println(isRowFull[y]);
      if ((!isRowFull[y]) && !emptyFound )
      {
        emptyFound = true; 
      }
      else if ((!isRowFull[y]) && emptyFound )
      {
        secondEmptyFound = true; 
      }
      else if ((isRowFull[y]) && secondEmptyFound)
      {
        safe = true;
      }
      
    }
    //Serial.println();
    
    return safe;
  }
  


  
  boolean isRowFull[GRID_H];

  for (int y = 0; y < GRID_H; y++)
  {
    boolean empty = true;
    for (int x = 0; x < GRID_W; x++)
    {
      if (x == xlist[0] || x == xlist[1] || x == xlist[2] || x == xlist[3])
      {
        if (grid_with_block[x][y])
        {
          empty = false;
        }
      }
    }
    isRowFull[y] = !empty;
  }

/*
  for (int y = 0; y < GRID_H; y++)
  {
    Serial.println(isRowFull[y]);
  }*/
  boolean safe = false;
  boolean emptyFound = false;
  boolean secondEmptyFound = false;
  for (int y = GRID_H -1; y >=0 ; y--)
  {
    //Serial.println(isRowFull[y]);
    if ((!isRowFull[y]) && !emptyFound )
    {
      emptyFound = true; 
    }
    else if ((!isRowFull[y]) && emptyFound )
    {
      secondEmptyFound = true; 
    }
    else if ((isRowFull[y]) && secondEmptyFound)
    {
      safe = true;
    }
    
  }
  //Serial.println();
  
  return safe;
}


//generate plan for the next piece
void ai_generate_plan() {

  //Create board without current falling piece
  //create board with piece
  for (int j = 0; j < GRID_W; j++)
    memcpy(grid[j], grid_with_block[j], sizeof(grid_with_block[j]));

  /*
  Serial.println(blockname[piece_id]);
  for (int y = 0; y < GRID_H; y++)
  {
    for (int x = 0; x < GRID_W; x++)
    {
      Serial.print(grid[x][y]);
    }
    Serial.println();
  }
  Serial.println();
  */

  grid[3][1] = false;
  grid[4][1] = false;
  grid[5][1] = false;
  grid[6][1] = false;
  grid[3][2] = false;
  grid[4][2] = false;
  grid[5][2] = false;
  grid[6][2] = false;

  
  ai_calculate_all_placements();

  

  double scores[POS_LIST_SIZE];
  ai_calculate_scores(scores);


  /*
  for (int i = 0; i < POS_LIST_SIZE; i++)
  {
    if (!(pos_list[i].x == 0 && pos_list[i].y == 0 && pos_list[i].r == 0))
      Serial.println(scores[i]);
  }
  */

  

  
  

  int best_pos_index = ai_select_best_placement(scores);

  piece_rotation_target = pos_list[best_pos_index].r;
  piece_x_target = pos_list[best_pos_index].x;
  piece_y_target = pos_list[best_pos_index].y;

  /*
  Serial.println(piece_rotation_target);
  Serial.println(piece_x_target);
  Serial.println(piece_y_target);

  Serial.println();
  */
}

//execute generated plan
void ai_execute_plan() {
  //first: reset the input
  sideways = 0;
  rotation = 0;
  softdrop = 0;

  /*
  Serial.println(piece_x);
  Serial.println(piece_y);
  Serial.println(piece_rotation);
  Serial.println();
  */

  //true if no adjustments to the current block is needed (so we can harddrop)
  boolean no_adjust = true;
  //adjust to target position

    if (piece_rotation != piece_rotation_target)
    {
      rotation = 1;
      if (piece_rotation >= 3)
      {
        piece_rotation = 0;
      }
      else
      {
        piece_rotation++;
      }
      no_adjust = false;
    }
    if (piece_x < piece_x_target)
    {
      piece_x++;
      sideways = 1;
      no_adjust = false;
    }
    else if (piece_x > piece_x_target)
    {
      piece_x--;
      sideways = -1;
      no_adjust = false;
    }
    if (no_adjust)
    {
      //softdrop = 1;
      plan = false;
    }
}

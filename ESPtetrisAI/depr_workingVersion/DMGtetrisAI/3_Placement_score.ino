/*
   PLACEMENT SCORE
   This file contains the functions that calculate the score of a certain placement
*/

//score calculation
int sum_of_heights(boolean newgrid[GRID_W][GRID_H])
{
  int heightsum = 0;
  for (int x = 0; x < GRID_W; x++)
  {
    int lowest_y = GRID_H;
    for (int y = 0; y < GRID_H; y++)
    {
      if (newgrid[x][y])
      {
        if (y < lowest_y)
        {
          lowest_y = y;
        }
      }
    }
    heightsum = heightsum + (GRID_H - lowest_y);
  }
  return heightsum;
}

//score calculation
int bumpiness(boolean newgrid[GRID_W][GRID_H])
{
  int avg_sum = 0;
  for (int x = 0; x < GRID_W - 1; x++)
  {
    int lowest_y = GRID_H;
    for (int y = 0; y < GRID_H; y++)
    {
      if (newgrid[x][y])
      {
        if (y < lowest_y)
        {
          lowest_y = y;
        }
      }
    }
    int lowest_y_next = GRID_H;
    for (int y = 0; y < GRID_H; y++)
    {
      if (newgrid[x + 1][y])
      {
        if (y < lowest_y_next)
        {
          lowest_y_next = y;
        }
      }
    }
    avg_sum = avg_sum + abs((GRID_H - lowest_y) - (GRID_H - lowest_y_next));
  }
  return avg_sum;
}

//score calculation
int holes(boolean newgrid[GRID_W][GRID_H])
{
  int holes = 0;
  for (int x = 0; x < GRID_W; x++)
  {
    int col_holes = 0;
    boolean found_block = false;
    for (int y = 0; y < GRID_H; y++)
    {
      if (newgrid[x][y])
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
  return holes;
}

//score calculation
int lines(boolean newgrid[GRID_W][GRID_H])
{
  int lines = 0;
  for (int y = 1; y < GRID_H; y++)
  {
    boolean full_line = true;
    for (int x = 0; x < GRID_W; x++)
    {
      if (!newgrid[x][y])
      {
        full_line = false;
      }
    }
    if (full_line)
    {
      lines = lines + 1;
    }
  }
  return lines;
}

//calculates score of gridcopy array
double calculate_score(boolean newgrid[GRID_W][GRID_H])
{
  double score = 0;

  score = score + w_heights   * sum_of_heights(newgrid);
  score = score + w_bumpiness * bumpiness(newgrid);
  score = score + w_holes     * holes(newgrid);
  score = score + w_lines     * lines(newgrid);

  return score;
}

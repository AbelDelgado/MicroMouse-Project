/** 
 *  @file maze.h
 *  @brief Maze application - uses Flood Fill Algorithm to solve a maze.
 *  @author Madzivire Chikwanda
 *  @bugs No known bugs.
 *  @date 21 March 2015
 *
 *  This a C adaptation for the HCS12 microcontroler of Maze.class posted on
 *  the Udemy website for the online class'Artificial Intelligence with Arduino.'
 *  
 *  The application simulates a maze which is mapped and solve by a 'mouse.' Each
 *  square on the maze is refered to as cell, walls along the y-axis are 
 *  considered horizontal, and walls along the x-axis are vertical. Maze size is
 *  is defined by the ROWS & COLUMNS macros.
 *
 *  The maze is solved by using a flood-fill algorithm to map and find a path to 
 *  the Target Cell.
 *
 *  @see https://www.udemy.com/nanomouse/
 */



/* -------------------- Defines --------------------  */

#define NORTH   0
#define EAST    1
#define SOUTH   2
#define WEST    3

#define UNKNOWN 255

#define ROWS    4
#define COLUMNS 3

#define TRUE    1
#define FALSE   0


/* -------------------- Global Variables -------------------- */

/**
 *  @brief  2x4 Array containing coordinates for neigboring cells.
 */
const short neighborCells[4][2] = {
   {-1,0},     /**< NORTH cell coordinates. */
   {0,1},      /**< EAST cell coordinates. */
   {1,0},      /**< SOUTH cell coordinates. */
   {0,-1}      /**< WEST cell coordinates. */
};


/*
 *  @brief  2x4 Array containing coordinates for neigboring walls.
 */
const short neighborWalls[4][2] = { 
   {0,0},	/**< NORTH wall coordinates. */
   {0,1},	/**< EAST cell coordinates. */
   {1,0},	/**< SOUTH cell coordinates. */
   {0,0}      	/**< WEST cell coordinates. */
};

typedef unsigned short boolean;

/**
 *  @brief Cell structure representing a square on the maze.
 */
struct Cell
{
   boolean isExplored;			/**< Cell exploration status flag. */
   unsigned short parentDirection;      /**< Direction of parent Cell. EAST, WEST, NORTH, or SOUTH */
};


/**
 *  @brief  Maze structure instance.
 */
struct Maze
{
   boolean verticalWalls[ROWS][COLUMNS+1]; 	/**< Vertical, (y-axis), walls array. */
   boolean horizontalWalls[ROWS+1][COLUMNS]; 	/**< Horizontal, (x-axis), walls array. */
   short values[ROWS][COLUMNS]; 		/**< Cell Flood-Fill values array. */
   struct Cell cell[ROWS][COLUMNS];

   short mouseRow;  	/**< Row number of the cell occupied by the mouse. */
   short mouseColumn; 	/**< Column number of the cell occupied by the mouse. */
   short mouseHeading;  /**< The direction the mouse is facing/heading. */

   short targetRow; 	/**< Row number of the solution cell. */
   short targetColumn; 	/**< Column number of the solution cell. */

   boolean mapComplete; /**< Maze mapping complete flag. */
}newMaze;



// --------------- FUNCTION DEFINITIONS -----------------



/**
 *  @brief Adds a wall to the maze.
 *  @param unsigned short
 *  @return void
 *
 *  The input parameter is the direction of the wall relative to the mouse. The direction,
 *  NORTH, EAST, SOUTH, or WEST, is expressed as an unsigned short.
 */
void addWall (unsigned short direction)
{
   switch(direction)
   {  // add northern horizontal wall
      case NORTH: newMaze.horizontalWalls[newMaze.mouseRow][newMaze.mouseColumn] = TRUE;
		  break;
		  // add eastern vertical wall            
      case EAST: newMaze.verticalWalls[newMaze.mouseRow][newMaze.mouseColumn + 1] = TRUE;
		 break;
		 // add western vertical all            
      case WEST: newMaze.verticalWalls[newMaze.mouseRow][newMaze.mouseColumn] = TRUE;
		 break;
		 // add southern horizontal wall            
      case SOUTH: newMaze.horizontalWalls[newMaze.mouseRow + 1][newMaze.mouseRow] = TRUE;
		  break;
   }
}

/**
 *  Add walls to maze cells
 */ 
void addVirtualWalls (void)
{
   newMaze.verticalWalls[4][1] = TRUE;
   newMaze.verticalWalls[4][1] = TRUE;
   newMaze.verticalWalls[2][2] = TRUE;
   newMaze.horizontalWalls[2][2] = TRUE;
   newMaze.horizontalWalls[2][0] = TRUE;
   newMaze.horizontalWalls[3][0] = TRUE;
}


/** 
 *  @brief Initializes maze structure. 
 *  @param void
 *  @param void
 *
 *  Sets the initial mouse position & heading on the maze & target cell.
 *  Exterior walls are also
 *   
 */
void initMaze (void)
{
   unsigned short row,column;
   newMaze.mapComplete = FALSE;
   newMaze.mouseHeading = NORTH;
   newMaze.mouseColumn = 0;
   newMaze.mouseRow = 1;
   newMaze.targetRow = 2;
   newMaze.targetColumn = 2;

   //initialize verticalWalls (add exterior walls)
   for(row = 0; row < ROWS; row++)
   {
      for(column = 0; column < COLUMNS+1; column++)
      {
	 if(column == 0 || column == COLUMNS)
	 {
	    newMaze.verticalWalls[row][column] = TRUE;
	 }
	 else// no interior vertical walls
	    newMaze.verticalWalls[row][column] = FALSE;
      }
   }

   //initialize horizontalWalls (add exterior walls)
   for(row = 0 ; row < ROWS+1; row++)
   {
      for(column = 0; column <COLUMNS; column++)
      {
	 if(row == 0 || row == ROWS)
	 {
	    newMaze.horizontalWalls[row][column] = TRUE;
	 }
	 else// no interior horizontal walls
	    newMaze.horizontalWalls[row][column] = FALSE;
      }
   }

   //initialize flood fill values and cells visitation status
   // initialize flood-fill values for all newMaze.cells
   for(row = 0; row < ROWS; row++)
   {
      for(column = 0; column < COLUMNS; column++)
      {
	 newMaze.values[row][column] = UNKNOWN;
	 newMaze.cell[row][column].isExplored = FALSE;
	 newMaze.cell[row][column].parentDirection = UNKNOWN;
      }
   }
}


/**
 *  @brief Prints a maze on terminal
 *  @param void
 *  @return void
 */
void print(void)
{
   unsigned short i,j,value; 

   for(i = 0;i < 2*ROWS+1;i++)
   {
      for(j = 0;j < 2*COLUMNS+1;j++)
      {
	 //Add Horizontal Walls
	 if(i%2 == 0 && j%2 == 1)
	 {
	    if(newMaze.horizontalWalls[i/2][j/2] == TRUE)
	    {
	       printf(" ---");
	    }
	    else
	    {
	       printf("    ");
	    }
	 }

	 //Add Vertical Walls
	 if(i%2 == 1 && j%2 == 0)
	 {
	    if(newMaze.verticalWalls[i/2][j/2] == TRUE)
	    {
	       printf("|");
	    }
	    else
	    {
	       printf(" ");
	    }
	 }

	 //Add Flood Fill Values
	 if(i%2 == 1 && j%2== 1)
	 {
	    if((i-1)/2 == newMaze.mouseRow && (j-1)/2 == newMaze.mouseColumn)
	    {
	       if(newMaze.mouseHeading == NORTH)
	       {
		  printf(" ^ ");
	       }
	       else if(newMaze.mouseHeading == EAST)
	       {
		  printf(" > ");
	       }
	       else if(newMaze.mouseHeading == SOUTH)
	       {
		  printf(" v ");
	       }
	       else if(newMaze.mouseHeading == WEST)
	       {
		  printf(" < ");
	       }
	    }
	    else
	    {
	       value = newMaze.values[(i-1)/2][(j-1)/2];
	       if(value >= 100)
	       {
		  printf("%u",value);
	       }
	       else
	       {
		  printf(" ");
		  printf("%u",value);
	       }
	       if(value < 10)
	       {
		  printf(" ");
	       }
	    }
	 }
      }
      printf("\n\r");
   }
   printf("\n\r");
}



/**
 *  @brief Applies a Flood-Fill algorithim to the maze structure.
 *  @param void
 *  @return void
 *
 *  Applies a Flood-Fill technique to all maze cells inorder to find a path to the
 *  target cell. 
 */
void solveMaze (void)
{
   short neighborCellrow, neighborCellColumn, row, column;
   short neighborWallrow, neighborWallColumn, direction;
   boolean wallExists;
   boolean continueSolving;

   // initialize flood-fill values for all newMaze.cells
   for(row = 0; row < ROWS; row++)
   {
      for(column = 0; column < COLUMNS; column++)
      {
	 newMaze.values[row][column] = 255;
      }
   }

   // set the flood fill value for the target cell
   newMaze.values[newMaze.targetRow][newMaze.targetColumn] = 0;

   do{
      continueSolving = FALSE;

      for(row = 0; row < ROWS; row++)
      {
	 for(column = 0; column < COLUMNS; column++)
	 {// check neighboring cells
	    if(newMaze.values[row][column] < 255)
	    {  
	       for(direction = 0; direction < 4; direction++)
	       {
		  // get neighboring cell coordinates
		  neighborCellrow = row + neighborCells[direction][0];
		  neighborCellColumn = column + neighborCells[direction][1];

		  // get neighboring wall coordinates
		  neighborWallrow = row + neighborWalls[direction][0];
		  neighborWallColumn = column + neighborWalls[direction][1];

		  if(direction == NORTH || direction == SOUTH)
		  {// check for horizontal north wall or south wall
		     wallExists = newMaze.horizontalWalls[neighborWallrow][neighborCellColumn];
		  }
		  else
		  {// check for vertical east wall or west wall
		     wallExists = newMaze.verticalWalls[neighborWallrow][neighborWallColumn];
		  }

		  // update flood fill value for neighbor cell if not separated by a wall
		  if(newMaze.values[neighborCellrow][neighborCellColumn] == 255 && !wallExists) 
		  {
		     newMaze.values[neighborCellrow][neighborCellColumn] = newMaze.values[row][column] + 1;
		     continueSolving = TRUE;
		  }

	       }
	    }
	 }
      }
   }while(continueSolving);
}

/**
 *  @brief Polls neighboring cells to find the best cell.
 *  @param void
 *  @return unsigned short 
 *
 *  Searches all cells neighboring the mouse's current cell to find the best cell to move to.
 *  The return value reperesents direction of the best cell.
 */
unsigned short getBestNeighbor(void)
{
   unsigned short neighborCellrow, neighborCellColumn, neighborWallrow, neighborWallColumn;
   unsigned short bestNeighborValue, preferedDirection, direction;
   boolean wallExists = FALSE;

   bestNeighborValue = 255;
   preferedDirection = NORTH;

   for(direction = NORTH; direction < WEST+1; direction++)
   {
      // get neighboring cell coordinates
      neighborCellrow = newMaze.mouseRow + neighborCells[direction][0];
      neighborCellColumn = newMaze.mouseColumn + neighborCells[direction][1];

      // get neighboring wall coordinates
      neighborWallrow = newMaze.mouseRow + neighborWalls[direction][0];
      neighborWallColumn = newMaze.mouseColumn + neighborWalls[direction][1];

      if(direction == NORTH || direction == SOUTH)
      {// check for horizontal north wall or south wall
	 wallExists = newMaze.horizontalWalls[neighborWallrow][neighborCellColumn];
      }
      else
      {// check for vertical east wall or west wall
	 wallExists = newMaze.verticalWalls[neighborWallrow][neighborWallColumn];
      }

      // only check cell not seperated by a wall
      if(!wallExists)
      {// update to best neighbor if lower cell value
	 if(newMaze.values[neighborCellrow][neighborCellColumn] < bestNeighborValue)
	 {    
	    bestNeighborValue = newMaze.values[neighborCellrow][neighborCellColumn];
	    preferedDirection = direction;
	 }
	 else if((newMaze.values[neighborCellrow][neighborCellColumn] == bestNeighborValue) && 
	       (direction == newMaze.mouseHeading))
	 {// maintain mouse heading if cell value is the same as the best neighbor
	    bestNeighborValue = newMaze.values[neighborCellrow][neighborCellColumn];
	    preferedDirection = direction;
	 }
      }
   }
   // print the prefered direction
   switch(preferedDirection)
   {
      case NORTH: printf("NORTH\n\r");
		  break;
      case  EAST: printf("EAST\n\r");
		  break;
      case SOUTH: printf("SOUTH\n\r");
		  break;
      case  WEST: printf("WEST\n\r");
		  break;
   }
   return preferedDirection;
}

/**
 * @brief Updates mouse position coordinates to one cell forward.
 * @param none
 */
void nextCell (void)
{
   newMaze.mouseRow = newMaze.mouseRow + neighborCells[newMaze.mouseHeading][0];
   newMaze.mouseColumn = newMaze.mouseColumn + neighborCells[newMaze.mouseHeading][1];
}


unsigned short getUnexploredCell (void)
{
   unsigned short neighborCellrow, neighborCellColumn, neighborWallrow, neighborWallColumn;
   unsigned short preferedDirection, direction;
   boolean preferedDirectionSet;

   boolean wallExists = FALSE;
   preferedDirectionSet = FALSE;

   for(direction = NORTH; direction < WEST+1; direction++)
   {
      // get neighboring cell coordinates
      neighborCellrow = newMaze.mouseRow + neighborCells[direction][0];
      neighborCellColumn = newMaze.mouseColumn + neighborCells[direction][1];

      // get neighboring wall coordinates
      neighborWallrow = newMaze.mouseRow + neighborWalls[direction][0];
      neighborWallColumn = newMaze.mouseColumn + neighborWalls[direction][1];

      if(direction == NORTH || direction == SOUTH)
      {// check for horizontal north wall or south wall
	 wallExists = newMaze.horizontalWalls[neighborWallrow][neighborCellColumn];
      }
      else
      {// check for vertical east wall or west wall
	 wallExists = newMaze.verticalWalls[neighborWallrow][neighborWallColumn];
      }

      // only check cells not seperated by a wall
      if(!wallExists)
      {// set the current cell as the parent for neighboring cells
	 if(newMaze.cell[neighborCellrow][neighborCellColumn].parentDirection == UNKNOWN)
	    switch(direction)
	    {
	       case NORTH: newMaze.cell[neighborCellrow][neighborCellColumn].parentDirection = SOUTH; 
			   break;
	       case  EAST: newMaze.cell[neighborCellrow][neighborCellColumn].parentDirection = WEST;
			   break;
	       case  WEST: newMaze.cell[neighborCellrow][neighborCellColumn].parentDirection = EAST;
			   break;
	       case SOUTH: newMaze.cell[neighborCellrow][neighborCellColumn].parentDirection = NORTH;
			   break;
	    }

	 if(!preferedDirectionSet)
	 {    
	    preferedDirection = direction;
	    preferedDirectionSet = TRUE;
	 }
      }
   }
   if(preferedDirectionSet)
      return preferedDirection;
   else return newMaze.cell[newMaze.mouseRow][newMaze.mouseColumn].parentDirection;
}

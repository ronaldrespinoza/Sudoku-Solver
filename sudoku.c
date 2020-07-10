/********************************************************/
/*Ronald Espinoza                                       */
/*4/8/2020                                              */
/*CS-241 Section 005                                    */
/*                                                      */
/*Purpose:                                              */
/*  This program tries to solve each sudoku puzzle in   */
/*  two seconds or less.                                */
/*How to Use this source file:                          */
/*  This program compiles using gcc -o ss SudokuSolver.c*/
/*  and can be run using ./ss testSudoku.in             */
/*  Please make sure to include the following header    */
/*  files in the same root directory before compilation */
/*  setCellIndex.h, validateLine.h, tryPuzzleSolutions.h*/
/*  checkBoxValues.h                                    */
/*  This will produce either a completed puzzle or an   */
/*  error message.                                      */
/*NOTE: some puzzles may be solvable in more than two   */
/*      seconds;                                        */
/*  backtracking algorithm was initially sourced from:  */
/*  http://shadowhackit.blogspot.com/2015/10/C6.html    */
/********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*definitions for ASCII values used*/
#define BASE_TEN_START  48/*0*/
#define BASE_TEN_END    57/*9*/
#define UNKNOWN         46/*.*/
#define CARRIAGE_RETURN 13/*CR*/
#define NEW_LINE        10/*LF*/
#define LINE_SIZE       81/*line size limit*/
#define BOARD_SIZE      9/*length/width of board*/
#define RUN_TIME_LIMIT  2000000/*2 seconds*/

/*global container for the current puzzle in play*/
char parsedBoard[BOARD_SIZE][BOARD_SIZE] = 
      {/*default value sets unknown for each index*/
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
      };

/*the current point of index on the board is tracked*/
/*with the row col variables                        */
int row = 0;
int col = 0;
/*global tracker for how long we are trying to solve*/
/*the current puzzle in play                        */
long int runTimeLimit = 0;

/*static values collected from box indicies*/
static int box1[] = {1,2,3,10,11,12,19,20,21};
static int box2[] = {4,5,6,13,14,15,22,23,24};
static int box3[] = {7,8,9,16,17,18,25,26,27};
static int box4[] = {28,29,30,37,38,39,46,47,48};
static int box5[] = {31,32,33,40,41,42,49,50,51};
static int box6[] = {34,35,36,43,44,45,52,53,54};
static int box7[] = {55,56,57,64,65,66,73,74,75};
static int box8[] = {58,59,60,67,68,69,76,77,78};
static int box9[] = {61,62,63,70,71,72,79,80,81};

/********************************************************/
/*Name: setRowCellStartIndex                            */
/*Params:int rowLocation                                */
/*Datatype:Int, input or output: output                 */
/*  - Def: adjust cell index for row                    */
/*  - range of values: 0,9,18,27,36,45,54,63,72         */
/*Functions' Data-Type: int                             */
/*Functions' return value: the rows starting index      */
/*Description:                                          */
/*This function sets the rows starting cell index       */
/*Functions' Algorithm                                  */
/*increment a count until a match for the point is found*/
/********************************************************/
int setRowCellStartIndex(int rowLocation)
{
  int indexValue = 0;
  switch(rowLocation)
  {
    case 1:
      indexValue = 0;
      break;
    case 2:
      indexValue = 9;
      break;
    case 3:
      indexValue = 18;
      break;
    case 4:
      indexValue = 27;
      break;
    case 5:
      indexValue = 36;
      break;
    case 6:
      indexValue = 45;
      break;
    case 7:
      indexValue = 54;
      break;
    case 8:
      indexValue = 63;
      break;
    case 9:
      indexValue = 72;
      break;
    default:
      indexValue = 0;
  }
  return indexValue;
}

/********************************************************/
/*Name: getRowValues                                    */
/*Params:int rowCount, char buff                        */
/*Datatype:char, input or output: output                */
/*  - Def: find values for row                          */
/*  - range of values: '.' and '0-9'                    */
/*Functions' Data-Type: char                            */
/*Functions' return value: the rows values              */
/*Description:                                          */
/*This function gets all values in the row              */
/*Functions' Algorithm                                  */
/*for all positions in the row collect the values and   */
/*return the row values                                 */
/********************************************************/
char *getRowValues(int rowCount, char buff[])
{
  int cellIndex = 0;
  char rowValues[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
  char *rValues = rowValues;
  int rowCellCount = 0;
  rowCount++;
  /* printf("values in row %d \n", rowCount); */
  cellIndex = setRowCellStartIndex(rowCount);

  for(rowCellCount = 0; rowCellCount < 9; rowCellCount++)
  {
    /* printf(" %c ", buff[cellIndex]); */
    rowValues[rowCellCount] = buff[cellIndex];
    cellIndex++;
  }
  /* printf("\n"); */
  return rValues;
}

/********************************************************/
/*Name: getColValues                                    */
/*Params:int colCount, char buff                        */
/*Datatype:char, input or output: output                */
/*  - Def: find values for row                          */
/*  - range of values: '.' and '0-9'                    */
/*Functions' Data-Type: char                            */
/*Functions' return value: the collumns values          */
/*Description:                                          */
/*This function gets all values in the collumn          */
/*Functions' Algorithm                                  */
/*for all positions in the collumn collect the values   */
/* and return the collumn values                        */
/********************************************************/
char *getColValues(int colCount, char buff[])
{
  int cellIndex = colCount;
  char colValues[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
  char *cValues = colValues;
  int colCellCount = 0;
  colCount++;
  /* printf("values in column %d \n", colCount); */
  for(colCellCount = 0; colCellCount < 9; colCellCount++)
  {
    /* printf(" %c ", buff[cellIndex]); */
    colValues[colCellCount] = buff[cellIndex];
    cellIndex += 9;
  }
  /* printf("\n"); */
  return cValues;
}

/********************************************************/
/*Name: setBoxCellStartIndex                            */
/*Params:int boxLocation                                */
/*Datatype:Int, input or output: output                 */
/*  - Def: adjust cell index for row                    */
/*  - range of values: 0,3,6,27,30,33,54,57,60          */
/*Functions' Data-Type: int                             */
/*Functions' return value: the box starting index       */
/*Description:                                          */
/*This function sets the box starting cell index        */
/*Functions' Algorithm                                  */
/*return the starting cell index based on current box   */
/********************************************************/
int setBoxCellStartIndex(int boxLocation)
{
  int indexValue = 0;
  switch(boxLocation)
  {
    case 1:
      indexValue = 0;
      break;
    case 2:
      indexValue = 3;
      break;
    case 3:
      indexValue = 6;
      break;
    case 4:
      indexValue = 27;
      break;
    case 5:
      indexValue = 30;
      break;
    case 6:
      indexValue = 33;
      break;
    case 7:
      indexValue = 54;
      break;
    case 8:
      indexValue = 57;
      break;
    case 9:
      indexValue = 60;
      break;
    default:
      indexValue = 0;
  }
  return indexValue;
}

/********************************************************/
/*Name: getBoxValues                                    */
/*Params:int boxCount, char buff                        */
/*Datatype:char, input or output: output                */
/*  - Def: find values for box                          */
/*  - range of values: '.' and '0-9'                    */
/*Functions' Data-Type: char                            */
/*Functions' return value: the boxes values             */
/*Description:                                          */
/*This function gets all values in the box              */
/*Functions' Algorithm                                  */
/*for all positions in the box collect the values and   */
/*return the boxes values                               */
/********************************************************/
char *getBoxValues(int boxLocation, char buff[])
{
  int cellIndex = 0;
  char boxValues[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
  char *bValues = boxValues;
  int boxCellCount = 0;
  boxLocation++;
  /* printf("values in box %d location\n", boxLocation); */
  cellIndex = setBoxCellStartIndex(boxLocation);
  for(boxCellCount = 0; boxCellCount < 9; boxCellCount++)
  {
    if(boxCellCount == 3 || boxCellCount == 6)
    {
      cellIndex = cellIndex + 6;
    }
    /* printf(" %c ", buff[cellIndex]); */
    boxValues[boxCellCount] = buff[cellIndex];
    cellIndex++;
  }
  /* printf("\n"); */

  return bValues;
}

/********************************************************/
/*Name: getBoxCount                                     */
/*Params:int charCount                                  */
/*Datatype:Int, input or output: output                 */
/*  - Def: get the current box index                    */
/*  - range of values: total count of boxes             */
/*Functions' Data-Type: int                             */
/*Functions' return value: the current box index        */
/*Description:                                          */
/*This function finds the current box Index             */
/*Functions' Algorithm                                  */
/*for all 9 indicies in the box                         */
/*  see if the charCount index is in the range of a box */
/*  if so then return the index for the current box     */
/********************************************************/
int getBoxCount(int charCount)
{/*this is because our logical count starts at 1*/
  charCount = charCount + 1;
  int cellCount = 0;
  for(cellCount = 0; cellCount < 9; cellCount++)
  {
    if(charCount == box1[cellCount])
    {
      return 1;
    }
    if(charCount == box2[cellCount])
    {
      return 2;
    }
    if(charCount == box3[cellCount])
    {
      return 3;
    }
    if(charCount == box4[cellCount])
    {
      return 4;
    }
    if(charCount == box5[cellCount])
    {
      return 5;
    }
    if(charCount == box6[cellCount])
    {
      return 6;
    }
    if(charCount == box7[cellCount])
    {
      return 7;
    }
    if(charCount == box8[cellCount])
    {
      return 8;
    }
    if(charCount == box9[cellCount])
    {
      return 9;
    }
  }/*unreachable error Box*/
  return 10;
}

/********************************************************/
/*Name: setBoxCellColIndex                              */
/*Params:int boxLocation                                */
/*Datatype:Int, input or output: output                 */
/*  - Def: get the current cell collumn index           */
/*  - range of values: 0,3,6                            */
/*Functions' Data-Type: int                             */
/*Functions' return value: the cell collumn index       */
/*Description:                                          */
/*This function finds the current cell Collumn Index    */
/*Functions' Algorithm                                  */
/*switch the returned cell collumn index based on our   */
/*  current box location                                */
/*  or default return the cell collumn for the first box*/
/********************************************************/
int setBoxCellColIndex(int boxLocation)
{
  int indexValue = 0;
  switch(boxLocation)
  {
    case 1:/*first row of boxes is always 0*/
      indexValue = 0;
      break;
    case 2:/*second row of boxes is always 3*/
      indexValue = 3;
      break;
    case 3:/*third row of boxes is always 6*/
      indexValue = 6;
      break;
    case 4:
      indexValue = 0;
      break;
    case 5:
      indexValue = 3;
      break;
    case 6:
      indexValue = 6;
      break;
    case 7:
      indexValue = 0;
      break;
    case 8:
      indexValue = 3;
      break;
    case 9:
      indexValue = 6;
      break;
    default:/*error box*/
      indexValue = 0;
  }
  return indexValue;
}

/********************************************************/
/*Name: setBoxCellRowIndex                              */
/*Params:int boxLocation                                */
/*Datatype:Int, input or output: output                 */
/*  - Def: get the current cell row index               */
/*  - range of values: 0,3,6                            */
/*Functions' Data-Type: int                             */
/*Functions' return value: the cell row index           */
/*Description:                                          */
/*This function finds the current cell row Index        */
/*Functions' Algorithm                                  */
/*switch the returned cell row index based on our       */
/*  current box location                                */
/*  or default return the cell row for the first box    */
/********************************************************/
int setBoxCellRowIndex(int boxLocation)
{
  int indexValue = 0;
  switch(boxLocation)
  {
    case 1:/*first column of boxes*/
      indexValue = 0;
      break;
    case 2:
      indexValue = 0;
      break;
    case 3:
      indexValue = 0;
      break;
    case 4:/*second column of boxes*/
      indexValue = 3;
      break;
    case 5:
      indexValue = 3;
      break;
    case 6:
      indexValue = 3;
      break;
    case 7:/*third column of boxes*/
      indexValue = 6;
      break;
    case 8:
      indexValue = 6;
      break;
    case 9:
      indexValue = 6;
      break;
    default:/*error index default for first box collumn*/
      indexValue = 0;
  }
  return indexValue;
}

/********************************************************/
/*Name: getCellIndex                                    */
/*Params:int width, int length                          */
/*Datatype:Int, input or output: output                 */
/*  - Def: convert point(x,y) to single integer         */
/*  - range of values: BOARD_SIZE^2                     */
/*Functions' Data-Type: int                             */
/*Functions' return value: the converted value          */
/*Description:                                          */
/*This function finds the current cell Index            */
/*Functions' Algorithm                                  */
/*increment a count until a match for the point is found*/
/********************************************************/
int getCellIndex(int width, int length)
{
  int cellIndex = 0;
  int cellCount = 0;
  int cellRow = 0;
  int cellCol = 0;

  for(cellRow = 0; cellRow < BOARD_SIZE; cellRow++)
  {
    for(cellCol = 0; cellCol < BOARD_SIZE; cellCol++)
    {
      if(width == cellRow && length == cellCol)
      {
        cellIndex = cellCount;
        return cellIndex;
      }
      cellCount++;
    }
  }
  return cellIndex;
}

/********************************************************/
/*Name: getBaseTenInt                                   */
/*Params: int currChar                                  */
/*Datatype:char, input or output: output                */
/*  - Def: convert current int value to a char value    */
/*  - range of values: 0-9                              */
/*Functions' Data-Type: char                            */
/*Functions' return value: the digit as a character     */
/*Description:                                          */
/*Return the current digit as a character.              */
/*Functions' Algorithm                                  */
/*our range is 0-9 we add 0 to convert currChar to a hex*/
/*value. This value then is typecast to char and is a   */
/*valid conversion only for (0-9)                       */
/********************************************************/
char getBaseTenInt(int currChar)
{
  char theDigitAsChar = 'X';
  theDigitAsChar = '0' + currChar;
  return theDigitAsChar;
}

/********************************************************/
/*Name:isNewLine                                        */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is a new line feed*/
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a new line feed?    */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a new line feed                                  */
/*Functions' Algorithm                                  */
/*if(char is new line or carriage return)               */
/*  return found new line feed                          */
/*else                                                  */
/*  default return, there was no new line feed          */
/********************************************************/
bool isNewLine(char currChar)
{/*to check the end of the records*/
  if(/*this is because the file end lines with CRLF*/
      (currChar == NEW_LINE) ||
      (currChar == CARRIAGE_RETURN)
    )
  {
    return true;
  }
  else if(currChar == '\0')
  {/*happens at the last line or EOF*/
    return true;
  }
  else
  {/*default to not a new line character*/
    return false;
  }
}

/********************************************************/
/*Name: printInvalidBuff                                */
/*Params: char buff[]                                   */
/*Datatype:Void, input or output: output                */
/*  - Def: print an unparsed sudoku board               */
/*  - range of values: all ASCII chars                  */
/*Functions' Data-Type: void                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*print out all the values in our passed in string.     */
/*Functions' Algorithm                                  */
/*For a reasonable length of our string                 */
/*  if we are at a newline print a newline and exit this*/
/*  function                                            */
/*  if not then print the current character             */
/*  and if we run out of room print a new line and exit */
/*  this function.                                      */
/********************************************************/
void printInvalidBuff(char buff[])
{
  int count = 0;
  char currChar = 'X';
  for(count = 0; count < 128; count++)
  {/*get the newest character from our string buffer*/
    currChar = buff[count];
    if(isNewLine(currChar))
    {
      printf("\n");
      return;/*exit the function*/
    }
    printf("%c", currChar);
  }/*ran out of room exit the function*/
  printf("\n");
  return;
}

/********************************************************/
/*Name: parseSudokuBoard                                */
/*Params: char sudokuBoard                              */
/*Datatype:Void, input or output: output                */
/*  - Def: parse the sudoku board                       */
/*  - range of values: N/A                              */
/*Functions' Data-Type: void                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*manipulate our global parsedBoard object              */
/*Functions' Algorithm                                  */
/*For row and collumn size(BOARD_SIZE) split the sudoku */
/* board string buffer into a two dimensional array     */
/********************************************************/
void parseSudokuBoard(char sudokuBoard[])
{
  int charCount =0;
  int rCount = 0;
  int cCount = 0;
  char currChar = 'X';
  /*loop for all indicies in the board*/
  for(rCount = 0; rCount < BOARD_SIZE; rCount++)
  {
    for(cCount = 0; cCount < BOARD_SIZE; cCount++)
    {/*access and manipulate our global pointer object*/
      currChar = sudokuBoard[charCount];
      parsedBoard[rCount][cCount] = currChar;
      charCount++;
    }
  }/*nothing needs to be returned due to global access*/
  return;
}

/********************************************************/
/*Name:isBaseTenChar                                    */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is base ten (0-9) */
/*  - ASCII range of values: TRUE or FALSE              */
/*Functions' return value: is there a base ten value?   */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a base ten value                                 */
/*Functions' Algorithm                                  */
/*if(char is base ten ASCII value)                      */
/*  return found base ten value                         */
/*else                                                  */
/*  default return, there was no base ten value         */
/********************************************************/
bool isBaseTenChar(char currChar)
{
  if(/*condition for base ten (0-9) ASCII int values*/
      (currChar >= BASE_TEN_START) &&
      (currChar <= BASE_TEN_END)
    )
  {/*occurs when currChar is a digit 0-9*/
    return true;
  }
  else
  {/*default is not a base ten character*/
    return false;
  }
}

/********************************************************/
/*Name: adjustCellValue                                 */
/*Params: int parsedValue                               */
/*Datatype:char, input or output: output                */
/*  - Def: adjust the parsed global value               */
/*  - range of values: 0-9                              */
/*Functions' Data-Type: char                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*adjust the parsed value                               */
/*Functions' Algorithm                                  */
/*if the value is negative then get the absolute value  */
/* for the integer.                                     */
/* then return the character value for this parsed value*/
/********************************************************/
char adjustCellValue(int parsedValue)
{
  char theValue = 'X';
  if(parsedValue < 0)
  {/*this was discovered poking at the pointer for the */
  /* parsed board actively in play                     */
    parsedValue = (parsedValue * (-1));
  }
  theValue = getBaseTenInt(parsedValue);
  if(isBaseTenChar(theValue))
  {
    return theValue;
  }/*it will not always be the case that this value  */
  /*was negative.*/
  else
  {/*this occurs for board values that do not update*/
    return parsedValue;
  }
}

/********************************************************/
/*Name:isEmptyCell                                      */
/*Params: char parsedBoard                              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is empty          */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a empty cell?       */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is an empty cell                                    */
/*Functions' Algorithm                                  */
/*for all cells on the board look for an empty cell     */
/*  if an empty cell is found return true               */
/*  else return false                                   */
/********************************************************/
bool isEmptyCell(char parsedBoard[BOARD_SIZE][BOARD_SIZE])
{
  int width = 0;
  int length = 0;
  char currChar = 'X';
  int theChar = 0;
  /*width = row because we may come back after leaving*/
  for(width=row; width < BOARD_SIZE; width++)
  {/*but we will always want to search the whole length*/
    for(length=0; length < BOARD_SIZE; length++)
    {
      theChar = parsedBoard[width][length];
      currChar = adjustCellValue(theChar);
      if(currChar == UNKNOWN)
      {/*A blank cell value has been found*/
        row = width;
        col = length;
        return true;
      }
    }
  }/*default return for when a puzzle was solved*/
  return false;
}

/********************************************************/
/*Name:matchInBox                                       */
/*Params: int cellRow, int cellCol, char num            */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if we have a match in the box          */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a match in the box? */
/*Description:                                          */
/*This function returns true if the character in play   */
/*  matches with any of the cells in its' box           */
/*Functions' Algorithm                                  */
/*for all cells on the board look for an empty cell     */
/*  if an empty cell is found return true               */
/*  else return false                                   */
/********************************************************/
bool matchInBox(int cellRow, int cellCol, char num)
{
  int bWidth = 0;
  int boxWidth = 0;
  int bLength = 0;
  int boxLength = 0;
  int cellIndex = 0;
  int boxIndex = 0;
  /*collect box properties*/
  cellIndex = getCellIndex(cellRow, cellCol);
  boxIndex = getBoxCount(cellIndex);
  bWidth = setBoxCellRowIndex(boxIndex);

  for(boxWidth = 0; boxWidth < 3; boxWidth++)
  {/*find out which box count we start at*/
    bLength = setBoxCellColIndex(boxIndex);
    for(boxLength = 0; boxLength < 3; boxLength++)
    {/*check the values in the box*/
      if(parsedBoard[bWidth][bLength]==num)
      {
        return true;
      }
      bLength++;
    }
    bWidth++;
  }/*default return no matches in the box*/
  return false;
}

/********************************************************/
/*Name:noMatchInRowCol                                  */
/*Params: int cellRow, int cellCol, char num            */
/*Datatype:Bool, input or output: output                */
/*  - Def: check for a match in the row or collumn      */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a match?            */
/*Description:                                          */
/*This function returns true if the character in play   */
/*  matches with any of the cells in the corresponding  */
/*  row or collumn.                                     */
/*Functions' Algorithm                                  */
/*for all cells in row/collumn look for a match         */
/*  if a match is found return false                    */
/*  else return true                                    */
/********************************************************/
bool noMatchInRowCol(int cellRow, int cellCol, char num)
{
  int length = 0;
  if(matchInBox(cellRow, cellCol, num))
  {/*safety check to prevent extra processing*/
    return false;
  }
  for(length=0; length < BOARD_SIZE; length++)
  {
    if(parsedBoard[cellRow][length] == num)
    {/*checking row*/
      return false;
    }
    if(parsedBoard[length][cellCol] == num)
    {/*checking collumn*/
      return false;
    }
  }/*default return no match was found*/
  return true;
}

/********************************************************/
/*Name:isBaseTenInt                                     */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is base ten (0-9) */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a base ten value?   */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a base ten value                                 */
/*Functions' Algorithm                                  */
/*if(char is base ten value)                            */
/*  return found base ten value                         */
/*else                                                  */
/*  default return, there was no base ten value         */
/********************************************************/
bool isBaseTenInt(char currChar)
{
  if(/*condition for base ten (0-9) int values*/
      (currChar >= 1) &&
      (currChar <= 9)
    )
  {/*occurs when currChar is a digit 0-9*/
    return true;
  }
  else
  {/*default is not a base ten character*/
    return false;
  }
}

/********************************************************/
/*Name: printSolution                                   */
/*Params: char parsedBoard                              */
/*Datatype:void, input or output: output                */
/*  - Def: print the solved puzzle                      */
/*  - range of values: all ASCII                        */
/*Functions' Data-Type: void                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*print the puzzles solution                            */
/*Functions' Algorithm                                  */
/*for all cells on the board print the value            */
/********************************************************/
void printSolution(char parsedBoard[BOARD_SIZE][BOARD_SIZE])
{
  int rbCount = 0;
  int cbCount = 0;

  for(rbCount = 0; rbCount < BOARD_SIZE; rbCount++)
  {
    for(cbCount = 0; cbCount < BOARD_SIZE; cbCount++)
    {/*this occurs for globally modified value*/
      if(isBaseTenInt(parsedBoard[rbCount][cbCount]))
      {
        printf("%i", parsedBoard[rbCount][cbCount]);
      }/*this occurs for preset board values*/
      else if(isBaseTenChar(parsedBoard[rbCount][cbCount]))
      {
        printf("%c", parsedBoard[rbCount][cbCount]);
      }
    }
  }/*we are done printing the solution*/
  printf("\n");
  return;
}

/********************************************************/
/*Name:hasSolution                                      */
/*Params: char parsedBoard                              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check for a solution                         */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a match?            */
/*Description:                                          */
/*This function returns true if there is a solution     */
/*Functions' Algorithm                                  */
/*increment and check our runTime limit                 */
/*  if at limit quit processing                         */
/*if we have no empty cells on the board                */
/*  return a solution does exist                        */
/*else continue checking for solution to blank piece    */
/* if at an error puzzle state then rollit back         */
/*  by using backtracking to walk back to the previous  */
/*  row and collumn                                     */
/********************************************************/
bool hasSolution(char parsedBoard[BOARD_SIZE][BOARD_SIZE])
{
  int digit = 0;
  int prevRow = 0;
  int prevCol = 0;
  char digitAsChar = 'X';
  /*increment is here due to recursive calls*/
  runTimeLimit++;
  if(runTimeLimit > RUN_TIME_LIMIT)
  {/*hardcoded constraint for 2 million iterations*/
    return false;
  }
  if(!isEmptyCell(parsedBoard))
  {/*The puzzle has been solved*/
    return true;
  }
  /*check all digits 1-9 recursively*/
  for(digit=1; digit <= 9; digit++)
  {/*control digit datatype for comparison*/
    digitAsChar = getBaseTenInt(digit);
    if(noMatchInRowCol(row,col,digitAsChar))
    {/*setup variables for backtracking*/
      parsedBoard[row][col]=digitAsChar;
      prevRow=row;
      prevCol=col;
      if(hasSolution(parsedBoard))
      {/*execute our recursive callback*/
        return true;/*for safety this value is returned*/
      }/*but it is not used conditionally*/
      /*reset globals during no solution found*/
      row = prevRow;
      col = prevCol;
      /*reset the board piece on rollback*/
      parsedBoard[row][col]=UNKNOWN;
    }
  }
  return false;
}

/********************************************************/
/*Name:resetParsedBoard                                 */
/*Params:                                               */
/*Datatype:Void, input or output: output                */
/*  - Def: reset global parsedBoard object              */
/*  - range of values: None                             */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*This function resets the parsedBoard                  */
/*Functions' Algorithm                                  */
/*for all pieces in the board set the cell to UNKNOWN   */
/********************************************************/
void resetParsedBoard()
{
  int rowCount = 0;
  int colCount = 0;
  for(rowCount = 0; rowCount < BOARD_SIZE; rowCount++)
  {
    for(colCount = 0; colCount < BOARD_SIZE; colCount++)
    {
      parsedBoard[rowCount][colCount] = UNKNOWN;
    }
  }
}

/********************************************************/
/*Name:tryToSolveThePuzzle                              */
/*Params: sudokuBoard                                   */
/*Datatype:Void, input or output: output                */
/*  - Def: see if we can solve the puzzle               */
/*  - range of values: None                             */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*This function lets us know if a solution exists       */
/*Functions' Algorithm                                  */
/*Setup the global object for puzzle solving            */
/*If the puzzle can be solved then print the solution   */
/*Else print there is no solution                       */
/********************************************************/
void tryToSolveThePuzzle(char sudokuBoard[])
{/*cleanup of globals for a new puzzle solution*/
  resetParsedBoard();
  row = 0;
  col = 0;
  runTimeLimit=0;
  /*set parsedBoard global to sudokuBoard line from file*/
  parseSudokuBoard(sudokuBoard);

  if(hasSolution(parsedBoard))
  {/*a solution has been found!!!*/
    printSolution(parsedBoard);
    printf("\n");
    return;
  }
  else
  {/*We ran out of time and there is no solution*/
    printf("No solution\n\n");
    return;
  }
}

/********************************************************/
/*Name: boxHasDuplicate                                 */
/*Params: char buffer of record in process              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check boxes on sudoku board for duplicates   */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a duplicate?        */
/*Description:                                          */
/*This function returns true if there is a duplicate    */
/*Functions' Algorithm                                  */
/*for all boxes on the board                            */
/*  look for all entries in a box to see if there is a  */
/*  duplicate                                           */
/*  if a duplicate is found return true else default    */
/*  return a false value indicating no duplicates found */
/********************************************************/
bool boxHasDuplicate(char buff[])
{
  char *boxValues;
  int i = 0;/*TODO: change this*/
  int j = 0;
  int boxCount = 0;
  char bAlpha = 'x';
  char bBeta = 'x';

  for(boxCount = 0; boxCount < 9; boxCount++)
  {/*loop through all boxes on the board*/
    boxValues = getBoxValues(boxCount, buff);
    for(i = 0; i < 9; i++)
    {/*For all values in the box*/
      bAlpha = boxValues[i];
      if(bAlpha == '.')
      {/*skip because there are bound to be*/
      }/*duplicates for this*/
      else
      {/*the alpha comparison is an integer*/
        for(j = 0; j < 9; j++)
        {/*compare against all other values in the box*/
          if(i == j)
          {/*skip because we do not want to compare*/
          }/*something against itself*/
          else
          {/*this is a valid comparison*/
            bBeta = boxValues[j];
            if(bAlpha == bBeta)
            {/*found match in box*/
              return true;
            }
          }
        }/*end compare against all other values in box*/
      }
    }/*end alpha comparison*/
  }
  return false;
}

/********************************************************/
/*Name: rowHasDuplicate                                 */
/*Params: char buffer of record in process              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check rows on sudoku board for duplicates    */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a duplicate?        */
/*Description:                                          */
/*This function returns true if there is a duplicate    */
/*Functions' Algorithm                                  */
/*for all rows on the board                             */
/*  look for all entries in a row to see if there is a  */
/*  duplicate                                           */
/*  if a duplicate is found return true else default    */
/*  return a false value indicating no duplicates found */
/********************************************************/
bool rowHasDuplicate(char buff[])
{
  char *rowValues = " ";
  int rowCount = 0;
  int i = 0;/*TODO*/
  int j = 0;
  char bAlpha = 'x';
  char bBeta = 'x';

  for(rowCount = 0; rowCount < 9; rowCount++)
  {
    rowValues = getRowValues(rowCount, buff);
    for(i = 0; i < 9; i++)
    {/*For all values in the row*/
      bAlpha = rowValues[i];
      if(bAlpha == '.')
      {/*skip because there are bound to be*/
      }/*duplicates for this*/
      else
      {/*the alpha comparison is an integer*/
        for(j = 0; j < 9; j++)
        {/*compare against all other values in the row*/
          if(i == j)
          {/*skip because we do not want to compare*/
          }/*something against itself*/
          else
          {/*this is a valid comparison*/
            bBeta = rowValues[j];
            if(bAlpha == bBeta)
            {/*found match in row*/
              return true;
            }
          }
        }/*end compare against all other values in row*/
      }
    }/*end alpha comparison*/
  }
  return false;
}

/********************************************************/
/*Name: colHasDuplicate                                 */
/*Params: char buffer of record in process              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check collumns on sudoku board for duplicates*/
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a duplicate?        */
/*Description:                                          */
/*This function returns true if there is a duplicate    */
/*Functions' Algorithm                                  */
/*for all collumns on the board                         */
/*  look for all entries in collumn to see if there is a*/
/*  duplicate                                           */
/*  if a duplicate is found return true else default    */
/*  return a false value indicating no duplicates found */
/********************************************************/
bool colHasDuplicate(char buff[])
{
  char *colValues = " ";
  int colCount = 0;
  int i = 0;/*TODO*/
  int j = 0;
  char bAlpha = 'x';
  char bBeta = 'x';

  for(colCount = 0; colCount < 9; colCount++)
  {
    colValues = getColValues(colCount, buff);
    for(i = 0; i < 9; i++)
    {/*For all values in the col*/
      bAlpha = colValues[i];
      if(bAlpha == '.')
      {/*skip because there are bound to be*/
      }/*duplicates for this*/
      else
      {/*the alpha comparison is an integer*/
        for(j = 0; j < 9; j++)
        {/*compare against all other values in the col*/
          if(i == j)
          {/*skip because we do not want to compare*/
          }/*something against itself*/
          else
          {/*this is a valid comparison*/
            bBeta = colValues[j];
            if(bAlpha == bBeta)
            {/*found match in col*/
              return true;
            }
          }
        }/*end compare against all other values in col*/
      }
    }/*end alpha comparison*/
  }
  return false;
}

/********************************************************/
/*Name: noDuplicates                                    */
/*Params: char buffer of record in process              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check duplicates on sudoku board             */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a duplicate?        */
/*Description:                                          */
/*This function returns true if there are no duplicates */
/*Functions' Algorithm                                  */
/*check if box or row or collumn have duplicates        */
/********************************************************/
bool noDuplicates(char buff[])
{
  if(boxHasDuplicate(buff) ||
     rowHasDuplicate(buff) ||
     colHasDuplicate(buff)
    )
  {
    return false;
  }
  else
  {
    return true;
  }
}

/********************************************************/
/*Name:isUnknown                                        */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is a unknown(.)   */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a unknown?          */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a unknown(;)                                     */
/*Functions' Algorithm                                  */
/*if(char is unknown)                                   */
/*  return found unknown                                */
/*else                                                  */
/*  default return, there was no unknown                */
/********************************************************/
bool isUnknown(char currChar)
{
  if(currChar == UNKNOWN)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/********************************************************/
/*Name:isValidCharacter                                 */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is valid          */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a valid character?  */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is '(0-9)', ';', 'CR', 'LF', or '\0'                */
/*Functions' Algorithm                                  */
/*if(char is is in range)                               */
/*  return found a valid character                      */
/*else                                                  */
/*  default return, there was no valid character        */
/********************************************************/
bool validCharacter(char currChar)
{
  if(/*a valid record is ('.', CRLF or 0-9)*/
      isBaseTenChar(currChar) ||
      isUnknown(currChar) ||
      isNewLine(currChar)
    )
  {/*when currChar is a valid char for a valid record*/
    return true;
  }
  else
  {/*default not a valid character or record*/
    return false;
  }
}

/********************************************************/
/*Name:validSize                                        */
/*Params: int charCount                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is valid          */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a valid character?  */
/*Description:                                          */
/*This function returns true if count equals LINE_SIZE  */
/*Functions' Algorithm                                  */
/*if(count is LINE_SIZE)                                */
/*  return count is currently at LINE_SIZE              */
/*else                                                  */
/*  default return, count is not at LINE_SIZE           */
/********************************************************/
bool validSize(int charCount)
{
  if(charCount == LINE_SIZE)
  {/*check for charCount less than LINE_SIZE*/
    return true;
  }
  else
  {
    return false;
  }
}

/********************************************************/
/*Name: validLine                                       */
/*Params: char buffer of record in process              */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current line is valid               */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is this a valid line?        */
/*Description:                                          */
/*This function returns true if the line is valid       */
/*Functions' Algorithm                                  */
/*while looking at each character in the line           */
/*  are we at a valid size                              */
/*  if so is this a valid character                     */
/*  if so are we at a newline                           */
/*    and are we still within a valid size              */
/*    then return this was a valid line                 */
/*  else                                                */
/*    default return, this was not a valid line         */
/********************************************************/
bool validLine(char buff[])
{
  int charCount = 0;

  while(charCount <= LINE_SIZE)
  {/*get the characters in the line*/
    char currChar = buff[charCount];
    /*This print statement is useful for building an  */
    /*index file of the origional puzzle being solved */
    /* printf("%d:", charCount + 1); */
    /* printf("value: %c", currChar); */
    /* printf(" \n"); */
    if(charCount > LINE_SIZE)
    {/*this is so we can leave validLine() quicker*/
      return false;
    }
    if(!validCharacter(currChar))
    {
      return false;
    }
    if(isNewLine(currChar))
    {/*re-check for valid size to prevent smashing*/
      if(validSize(charCount))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    charCount++;
  }/*unreachable state included for warning supression*/
  return false;
}

/********************************************************/
/*Name: readRecord                                      */
/*Params: char buff[]                                   */
/*Datatype:Void, input or output: input                 */
/*  - Def: read the record and print the result         */
/*  - range of values: N/A                              */
/*Functions' Data-Type: Void                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*This function checks if the line is a valid sudoku    */
/*  board. Then tries to solve the puzzle.              */
/*Functions' Algorithm                                  */
/*if(at a valid puzzle line)                            */
/*  print sudoku board before changes                   */
/*  and try to solve the puzzle                         */
/*else at an invalid puzzle line                        */
/*  print data in the line and Error                    */
/********************************************************/
void readRecord(char buff[])
{
  char *sudokuBoard = buff;
  if(validLine(buff) && noDuplicates(buff))
  {
    printInvalidBuff(buff);
    tryToSolveThePuzzle(sudokuBoard);
  }
  else
  {
    printInvalidBuff(buff);
    printf("Error\n\n");
  }
  return;
}

/********************************************************/
/*Name: main                                            */
/*Params:int arg_count, char *file_name[]               */
/*Datatype:Int, input or output: input                  */
/*  - Def: main entry point for input to program        */
/*  - range of values: 0,1                              */
/*Functions' Data-Type: int                             */
/*Functions' return value: 1 if something bad occurs    */
/*Description:                                          */
/*This function opens and reads the records from a file */
/*Functions' Algorithm                                  */
/*while reading the file till the end of the file       */
/*    read the records in the file                      */
/********************************************************/
int main(int arg_count, char *cmdLine_params[])
{
  char buff[128];
  /*loop through the text in the file till EOF*/
  while(fgets(buff, sizeof(buff), stdin) != NULL)
  {/*this will hang if stdin has no input*/
    readRecord(buff);
  }
  return 0;
}
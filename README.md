# Sudoku-Solver

<h2>Purpose:</h2>                                              
*  This program tries to solve each sudoku puzzle in   
 two seconds or less.    
  
<h2>How to Use this source file:</h2>                          
*  This program compiles using gcc -o sudoku sudoku.c
  and can be run using ./sudoku testSudoku.in             
*  This will produce either a completed puzzle or an   
  error message.                                      
NOTE: some puzzles may be solvable in more than two   
      seconds;                                        
*  The backtracking algorithm was initially sourced from:  
  http://shadowhackit.blogspot.com/2015/10/C6.html    
  
  This software works by parsing a single dimensional array into a two dimensional array.
   
  ![parsed board image](https://github.com/ronaldrespinoza/Sudoku-Solver/blob/master/img/sudoku_ParsedBoard.png)
  
  This array is then recursively tried for acceptable solutions to the puzzle
  
  ![example output](https://github.com/ronaldrespinoza/Sudoku-Solver/blob/master/img/sudoku_ExampleOutput.png)

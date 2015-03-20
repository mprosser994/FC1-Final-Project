// Mason Prosser
// final.c
// Tetris
// Allows the user to play a game of Tetris in a graphics window. Uses an array
// of characters to store the locations of blocks as the game is played.

#include <stdio.h>
#include <stdlib.h>
#include "gfx3.h"
#include <time.h>

struct Tetr{
   int type;
   char letter;
   int element[4][2];
};

char startup(int, int);
int play(char[20][10], struct Tetr *, int, int);
int maketetr(char[20][10], struct Tetr *);
void makeboard(char[20][10]);
void display(char[20][10]);
void choosecolor(char, int[3]);
int advance(char[20][10], struct Tetr *);
int move(char[20][10], struct Tetr *, char);
int rotate(char[20][10], struct Tetr *, char);
void tobottom(char[20][10], struct Tetr *);
void isrowfull(char[20][10], int *);
void rowclear(char[20][10], int);
int isgameover(char[20][10], int);
char gameover(int, int);

int main()
{
   int wd = 600, ht = 600;
   struct Tetr block;
   char board[20][10];
   char c = '0';

   gfx_open(wd, ht, "TETRIS");

   // Wait for user to press space to start game   
   while(c != ' '){
      c = startup(wd, ht);
   } // End while

   while(!play(board, &block, wd, ht));
} // End main

// Function startup
// Displays a startup screen for the game. Returns the character
// that the user presses.
char startup(int wd, int ht)
{
   char c;

   gfx_text(200, 25, "TETRIS: Please press [space] to start!");
   c = gfx_wait();

   return c;
} // End startup

// Function play
// This function runs the game by calling other functions and quits when
// the function isgameover returns true. Returns 1 if user quits the game.
int play(char board[20][10], struct Tetr *p, int wd, int ht)
{
   int t = 0, tmax = 400000; // Time between frames
   int g = 0; // Flag for game over: 1 if game ends
   char c = '0';

   makeboard(board);
   maketetr(board, p);

   while(!g){
      display(board);
      
      while(t < tmax){
         t++;
         if(gfx_event_waiting()){
            c = gfx_wait();
            if(c == 'z' || c == 'x'){ // Don't rotate a square block
               if(p->letter != 'O') rotate(board, p, c);
            }else if(c == 'Q' || c == 'S' || c == 'T'){
               move(board, p, c);
            }else if(c == ' '){
               tobottom(board, p);
               break;
            }else if(c == 'q'){
               return 1;
            } // End if
         } // End if
      } // End while

      t = 0;

      if(advance(board, p)){       // If the active block has been placed
         isrowfull(board, &tmax);  // Check if any rows have been filled
         g = maketetr(board, p);   // Make a new block
      } // End if
   } // End while

   if((c = gameover(wd, ht)) == 'q') return 1;

   return 0;
} // End play

// Function maketetr
// Randomly generates a tetromino (block) and places it on the board.
int maketetr(char board[20][10], struct Tetr *p)
{
   int i;

   srand(time(NULL));
   p->type = rand() % 7;

   // Check if block can be placed; if not, game over
   if(isgameover(board, p->type)) return 1;

   switch(p->type){
      case 0:
         for(i = 0; i <= 3; i++) p->element[i][0] = 0;
         for(i = 0; i <= 3; i++) p->element[i][1] = i + 3;
         for(i = 0; i <= 3; i++) board[0][i+3] = 'I';
         p->letter = 'I';
         break;
      case 1:
         p->element[0][0] = 1; p->element[0][1] = 4;
         p->element[1][0] = 1; p->element[1][1] = 5;
         p->element[2][0] = 0; p->element[2][1] = 4;
         p->element[3][0] = 0; p->element[3][1] = 5;
         board[0][4] = 'O'; board[0][5] = 'O';
         board[1][4] = 'O'; board[1][5] = 'O';
         p->letter = 'O';
         break;
      case 2:
         p->element[0][0] = 1; p->element[0][1] = 4;
         p->element[1][0] = 1; p->element[1][1] = 5;
         p->element[2][0] = 0; p->element[2][1] = 5;
         p->element[3][0] = 0; p->element[3][1] = 6;
         board[1][4] = 'S'; board[1][5] = 'S';
         board[0][5] = 'S'; board[0][6] = 'S';
         p->letter = 'S';
         break;
      case 3:
         p->element[0][0] = 0; p->element[0][1] = 4;
         p->element[1][0] = 0; p->element[1][1] = 5;
         p->element[2][0] = 1; p->element[2][1] = 5;
         p->element[3][0] = 1; p->element[3][1] = 6;
         board[0][4] = 'Z'; board[0][5] = 'Z';
         board[1][5] = 'Z'; board[1][6] = 'Z';
         p->letter = 'Z';
         break;
      case 4:
         p->element[0][0] = 0; p->element[0][1] = 5;
         p->element[1][0] = 1; p->element[1][1] = 4;
         p->element[2][0] = 1; p->element[2][1] = 5;
         p->element[3][0] = 1; p->element[3][1] = 6;
         board[0][5] = 'T'; board[1][4] = 'T';
         board[1][5] = 'T'; board[1][6] = 'T';
         p->letter = 'T';
         break;
      case 5:
         p->element[0][0] = 0; p->element[0][1] = 4;
         p->element[1][0] = 2; p->element[1][1] = 4;
         p->element[2][0] = 1; p->element[2][1] = 4;
         p->element[3][0] = 2; p->element[3][1] = 5;
         board[0][4] = 'L'; board[1][4] = 'L';
         board[2][4] = 'L'; board[2][5] = 'L';
         p->letter = 'L';
         break;
      case 6:
         p->element[0][0] = 0; p->element[0][1] = 4;
         p->element[1][0] = 2; p->element[1][1] = 4;
         p->element[2][0] = 1; p->element[2][1] = 4;
         p->element[3][0] = 0; p->element[3][1] = 5;
         board[0][4] = 'R'; board[0][5] = 'R';
         board[1][4] = 'R'; board[2][4] = 'R';
         p->letter = 'R';
         break;
      default:
         maketetr(board, p);
         break;
   } // End switch

   return 0;
} // End maketetr

// Function makeboard
// Initializes the playing board
void makeboard(char board[20][10])
{
   int i, j;

   for(i = 0; i < 20; i++)
      for(j = 0; j < 10; j++)
         board[i][j] = ' ';
} // End makeboard

// Function display
// Displays the game in the graphics window
void display(char board[20][10])
{
   int i, j;
   int bw = 25, bh = 25; // Dimensions of each square of the board
   int rgb[3];

   gfx_flush();
   gfx_clear();

   // Borders of the board (rectangle 5 px wide around board)
   gfx_color(255, 255, 255);
   gfx_rectangle(170, 45, 260, 510);
   gfx_rectangle(171, 46, 258, 508);
   gfx_rectangle(172, 47, 256, 506);
   gfx_rectangle(173, 48, 254, 504);
   gfx_rectangle(174, 49, 252, 502);

   // Draw the current board
   for(i = 0; i < 20; i++){
      for(j = 0; j < 10; j++){
         if(isalpha(board[i][j])){
            choosecolor(board[i][j], rgb);
            gfx_color(rgb[0], rgb[1], rgb[2]);
            gfx_rectangle(j*bw + 175, i*bh + 50, bw, bh);
            gfx_fill_rectangle(j*bw + 175, i*bh + 50, bw, bh);
         } // End if
      } // End for
   } // End for

   // Draw a grid on top of the blocks so divisions can be seen
   gfx_color(40, 40, 40);
   
   for(j = 1; j < 10; j++){
      gfx_line(j*bw + 175, 50, j*bw+175, 550);
   } // End for

   for(i = 1; i < 20; i++){
      gfx_line(175, i*bh + 50, 425, i*bh + 50);
   } // End for

   gfx_flush();
   
} // End display

// Function choosecolor
// Picks what color a square should be based on the tetromino's type
void choosecolor(char c, int rgb[3])
{
   c = c - 'A';

   switch(c){
      case 8:    // An I-tetromino
         rgb[0] = 0; 
         rgb[1] = 255; 
         rgb[2] = 222;
         break;
      case 11:   // An L-tetromino
         rgb[0] = 255; 
         rgb[1] = 122; 
         rgb[2] = 0;
         break;
      case 14:   // An O-tetromino
         rgb[0] = 255; 
         rgb[1] = 235; 
         rgb[2] = 47;
         break;
      case 17:   // An R-tetromino
         rgb[0] = 0; 
         rgb[1] = 43; 
         rgb[2] = 255;
         break;
      case 18:   // An S-tetromino
         rgb[0] = 7; 
         rgb[1] = 222; 
         rgb[2] = 0;
         break;
      case 19:   // A T-tetromino
         rgb[0] = 136; 
         rgb[1] = 0; 
         rgb[2] = 203;
         break;
      case 25:   // A Z-tetromino
         rgb[0] = 233;
         rgb[1] = 0;
         rgb[2] = 0;
         break;
      default:
         break;
   } // End switch
         
} // End choosecolor

// Function move
// Moves the active block in a user-specified direction
int move(char board[20][10], struct Tetr *p, char x)
{
   char a[2], b[2], c[2], d[2]; // Temps to store location of moved block

   // Set up temporary variables to check the board
   a[0] = p->element[0][0]; a[1] = p->element[0][1];
   b[0] = p->element[1][0]; b[1] = p->element[1][1];
   c[0] = p->element[2][0]; c[1] = p->element[2][1];
   d[0] = p->element[3][0]; d[1] = p->element[3][1];
   
   x = x - 'Q';

   switch(x){
      case 0:
         // If the block is being moved past an edge, quit before moving it
         if(a[1]-1 < 0 || b[1]-1 < 0 || c[1]-1 < 0 || d[1]-1 < 0)
            return 0;

         // Check the spaces near each element to make sure they are not
         // part of the block and are not occupied
         if(!(a[1]-1 == b[1] && a[0] == b[0])
         && !(a[1]-1 == c[1] && a[0] == c[0])
         && !(a[1]-1 == d[1] && a[0] == d[0])){
            if(isalpha(board[a[0]][a[1]-1]))
               return 0;
         } // End if
         if(!(b[1]-1 == a[1] && b[0] == a[0])
         && !(b[1]-1 == c[1] && b[0] == c[0])
         && !(b[1]-1 == d[1] && b[0] == d[0])){
            if(isalpha(board[b[0]][b[1]-1]))
               return 0;
         } // End if
         if(!(c[1]-1 == a[1] && c[0] == a[0])
         && !(c[1]-1 == b[1] && c[0] == b[0])
         && !(c[1]-1 == d[1] && c[0] == d[0])){
            if(isalpha(board[c[0]][c[1]-1]))
               return 0;
         } // End if
         if(!(d[1]-1 == a[1] && d[0] == a[0])
         && !(d[1]-1 == b[1] && d[0] == b[0])
         && !(d[1]-1 == c[1] && d[0] == c[0])){
            if(isalpha(board[d[0]][d[1]-1]))
               return 0;
         } // End if

         // Otherwise, move in the direction specified
         p->element[0][1]--;
         p->element[1][1]--;
         p->element[2][1]--;
         p->element[3][1]--;
         break;
      case 2:
         // If the block is being moved past an edge, quit before moving it
         if(a[1]+1 > 9 || b[1]+1 > 9 || c[1]+1 > 9 || d[1]+1 > 9)
            return 0;

         // Check the spaces near each element to make sure they are not
         // part of the block and are not already occupied
         if(!(a[1]+1 == b[1] && a[0] == b[0])
         && !(a[1]+1 == c[1] && a[0] == c[0])
         && !(a[1]+1 == d[1] && a[0] == d[0])){
            if(isalpha(board[a[0]][a[1]+1]))
               return 0;
         } // End if
         if(!(b[1]+1 == a[1] && b[0] == a[0])
         && !(b[1]+1 == c[1] && b[0] == c[0])
         && !(b[1]+1 == d[1] && b[0] == d[0])){
            if(isalpha(board[b[0]][b[1]+1]))
               return 0;
         } // End if
         if(!(c[1]+1 == a[1] && c[0] == a[0])
         && !(c[1]+1 == b[1] && c[0] == b[0])
         && !(c[1]+1 == d[1] && c[0] == d[0])){
            if(isalpha(board[c[0]][c[1]+1]))
               return 0;
         } // End if
         if(!(d[1]+1 == a[1] && d[0] == a[0])
         && !(d[1]+1 == b[1] && d[0] == b[0])
         && !(d[1]+1 == c[1] && d[0] == c[0])){
            if(isalpha(board[d[0]][d[1]+1]))
               return 0;
         } // End if
         
         // Otherwise, move it in the direction specified
         p->element[0][1]++;
         p->element[1][1]++;
         p->element[2][1]++;
         p->element[3][1]++;
         break;
      case 3:
         // If the block is being moved past an edge, quit before moving it
         if(a[0]+1 > 19 || b[0]+1 > 19 || c[0]+1 > 19 || d[0]+1 > 19)
            return 0;

         // Check the spaces near each element to make sure they are not part
         // of the active block and are not already occupied
         if(!(a[0]+1 == b[0] && a[1] == b[1])
         && !(a[0]+1 == c[0] && a[1] == c[1])
         && !(a[0]+1 == d[0] && a[1] == d[1])){
            if(isalpha(board[a[0]+1][a[1]]))
               return 0;
         } // End if
         if(!(b[0]+1 == a[0] && b[1] == a[1])
         && !(b[0]+1 == c[0] && b[1] == c[1])
         && !(b[0]+1 == d[0] && b[1] == d[1])){
            if(isalpha(board[b[0]+1][b[1]]))
               return 0;
         } // End if
         if(!(c[0]+1 == a[0] && c[1] == a[1])
         && !(c[0]+1 == b[0] && c[1] == b[1])
         && !(c[0]+1 == d[0] && c[1] == d[1])){
            if(isalpha(board[c[0]+1][c[1]]))
               return 0;
         } // End if
         if(!(d[0]+1 == a[0] && d[1] == a[1])
         && !(d[0]+1 == b[0] && d[1] == b[1])
         && !(d[0]+1 == c[0] && d[1] == c[1])){
            if(isalpha(board[d[0]+1][d[1]]))
               return 0;
         } // End if

         // Otherwise, move the block in the direction specified
         p->element[0][0]++;
         p->element[1][0]++;
         p->element[2][0]++;
         p->element[3][0]++;
         break;
      default:
         break;
   } // End switch

   // Clear the spaces where the block used to be
   board[a[0]][a[1]] = ' ';
   board[b[0]][b[1]] = ' ';
   board[c[0]][c[1]] = ' ';
   board[d[0]][d[1]] = ' ';

   // Re-assign temps to the block's new position
   a[0] = p->element[0][0]; a[1] = p->element[0][1];
   b[0] = p->element[1][0]; b[1] = p->element[1][1];
   c[0] = p->element[2][0]; c[1] = p->element[2][1];
   d[0] = p->element[3][0]; d[1] = p->element[3][1];

   // Place the elements in their new locations
   board[a[0]][a[1]] = p->letter;
   board[b[0]][b[1]] = p->letter;
   board[c[0]][c[1]] = p->letter;
   board[d[0]][d[1]] = p->letter;

   display(board);

} // End move

// Function rotate
// Rotates the active block in a user-specified direction
int rotate(char board[20][10], struct Tetr *p, char x)
{
   char a[2], b[2], c[2], d[2]; // Temps for rotated block
   int dis[3][2]; // Displacement from the 'center element' (element[2])

   // Set up temporary variables to check the board
   a[0] = p->element[0][0]; a[1] = p->element[0][1];
   b[0] = p->element[1][0]; b[1] = p->element[1][1];
   c[0] = p->element[2][0]; c[1] = p->element[2][1];
   d[0] = p->element[3][0]; d[1] = p->element[3][1];

   // Find each element's displacement from the 'center element'
   dis[0][0] = a[0] - c[0]; dis[0][1] = a[1] - c[1];
   dis[1][0] = b[0] - c[0]; dis[1][1] = b[1] - c[1];
   dis[2][0] = d[0] - c[0]; dis[2][1] = d[1] - c[1];

   x = x - 'x';

   switch(x){
      case 0:
         // Check if each element's new location is already occupied by an
         // element that is not a part of the active block
         if(((c[0]+dis[0][1] != a[0]) || (c[1]-dis[0][0] != a[1]))
         && ((c[0]+dis[0][1] != b[0]) || (c[1]-dis[0][0] != b[1]))
         && ((c[0]+dis[0][1] != d[0]) || (c[1]-dis[0][0] != d[1]))){
            if(isalpha(board[c[0]+dis[0][1]][c[1]-dis[0][0]])) return 1;
           
            // If an element would be moved past an edge, push the entire
            // block away from the edge so that it can be rotated
            if(c[0]+dis[0][1] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][0]++;
               p->element[1][0]++;
               p->element[2][0]++;
               p->element[3][0]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[1]-dis[0][0] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]++;
               p->element[1][1]++;
               p->element[2][1]++;
               p->element[3][1]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if

            // Or, if this would mean pushing the block up, quit without
            // rotating
            if(c[0]+dis[0][1] > 19) return 1;
            if(c[1]-dis[0][0] > 9){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]--;
               p->element[1][1]--;
               p->element[2][1]--;
               p->element[3][1]--;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
         } // End if

         // Check spaces where elements will move to see if they are occupied
         // by elements not in the active block
         if(((c[0]+dis[1][1] != a[0]) || (c[1]-dis[1][0] != a[1]))
         && ((c[0]+dis[1][1] != b[0]) || (c[1]-dis[1][0] != b[1]))
         && ((c[0]+dis[1][1] != d[0]) || (c[1]-dis[1][0] != d[1]))){
            if(isalpha(board[c[0]+dis[1][1]][c[1]-dis[1][0]])) return 1;

            // Push the block away from the wall if necessary
            if(c[0]+dis[1][1] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][0]++;
               p->element[1][0]++;
               p->element[2][0]++;
               p->element[3][0]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[1]-dis[1][0] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]++;
               p->element[1][1]++;
               p->element[2][1]++;
               p->element[3][1]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[0]+dis[1][1] > 19) return 1;
            if(c[1]-dis[1][0] > 9){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]--;
               p->element[1][1]--;
               p->element[2][1]--;
               p->element[3][1]--;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
         } // End if

         // Check each element's new location
         if(((c[0]+dis[2][1] != a[0]) || (c[1]-dis[2][0] != a[1]))
         && ((c[0]+dis[2][1] != b[0]) || (c[1]-dis[2][0] != b[1]))
         && ((c[0]+dis[2][1] != d[0]) || (c[1]-dis[2][0] != d[1]))){
            if(isalpha(board[c[0]+dis[2][1]][c[1]-dis[2][0]])) return 1;

            // Push block away from wall if necessary
            if(c[0]+dis[2][1] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][0]++;
               p->element[1][0]++;
               p->element[2][0]++;
               p->element[3][0]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[1]-dis[2][0] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]++;
               p->element[1][1]++;
               p->element[2][1]++;
               p->element[3][1]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[0]+dis[2][1] > 19) return 1;
            if(c[1]-dis[2][0] > 9){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]--;
               p->element[1][1]--;
               p->element[2][1]--;
               p->element[3][1]--;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
         } // End if

         // If no problems arise, assign new element positions
         p->element[0][0] = c[0] + dis[0][1];
         p->element[0][1] = c[1] - dis[0][0];
         p->element[1][0] = c[0] + dis[1][1];
         p->element[1][1] = c[1] - dis[1][0];
         p->element[3][0] = c[0] + dis[2][1];
         p->element[3][1] = c[1] - dis[2][0];
         break;
      case 2:
         // Check new positions of elements
         if(((c[0]-dis[0][1] != a[0]) || (c[1]+dis[0][0] != a[1]))
         && ((c[0]-dis[0][1] != b[0]) || (c[1]+dis[0][0] != b[1]))
         && ((c[0]-dis[0][1] != d[0]) || (c[1]+dis[0][0] != d[1]))){
            if(isalpha(board[c[0]-dis[0][1]][c[1]+dis[0][0]])) return 1;

            // Push block away from wall if necessary
            if(c[0]-dis[0][1] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][0]++;
               p->element[1][0]++;
               p->element[2][0]++;
               p->element[3][0]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[1]+dis[0][0] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]++;
               p->element[1][1]++;
               p->element[2][1]++;
               p->element[3][1]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[0]-dis[0][1] > 19) return 1;
            if(c[1]+dis[0][0] > 9){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]--;
               p->element[1][1]--;
               p->element[2][1]--;
               p->element[3][1]--;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
         } // End if
         // Check new element positions
         if(((c[0]-dis[1][1] != a[0]) || (c[1]+dis[1][0] != a[1]))
         && ((c[0]-dis[1][1] != b[0]) || (c[1]+dis[1][0] != b[1]))
         && ((c[0]-dis[1][1] != d[0]) || (c[1]+dis[1][0] != d[1]))){
            if(isalpha(board[c[0]-dis[1][1]][c[1]+dis[1][0]])) return 1;

            // Push block away from wall if necessary
            if(c[0]-dis[1][1] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][0]++;
               p->element[1][0]++;
               p->element[2][0]++;
               p->element[3][0]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[1]+dis[1][0] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]++;
               p->element[1][1]++;
               p->element[2][1]++;
               p->element[3][1]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[0]-dis[1][1] > 19) return 1;
            if(c[1]+dis[1][0] > 9){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]--;
               p->element[1][1]--;
               p->element[2][1]--;
               p->element[3][1]--;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
         } // End if

         // Check new element positions
         if(((c[0]-dis[2][1] != a[0]) || (c[1]+dis[2][0] != a[1]))
         && ((c[0]-dis[2][1] != b[0]) || (c[1]+dis[2][0] != b[1]))
         && ((c[0]-dis[2][1] != d[0]) || (c[1]+dis[2][0] != d[1]))){
            if(isalpha(board[c[0]-dis[2][1]][c[1]+dis[2][0]])) return 1;

            // Push block away from wall if necessary
            if(c[0]-dis[2][1] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][0]++;
               p->element[1][0]++;
               p->element[2][0]++;
               p->element[3][0]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[1]+dis[2][0] < 0){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]++;
               p->element[1][1]++;
               p->element[2][1]++;
               p->element[3][1]++;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
            if(c[0]-dis[2][1] > 19) return 1;
            if(c[1]+dis[2][0] > 9){
               board[a[0]][a[1]] = ' ';
               board[b[0]][b[1]] = ' ';
               board[c[0]][c[1]] = ' ';
               board[d[0]][d[1]] = ' ';
               p->element[0][1]--;
               p->element[1][1]--;
               p->element[2][1]--;
               p->element[3][1]--;
               rotate(board, p, x + 'x');
               return 1;
            } // End if
         } // End if

         // If no problems arise, assign new element locations
         p->element[0][0] = c[0] - dis[0][1];
         p->element[0][1] = c[1] + dis[0][0];
         p->element[1][0] = c[0] - dis[1][1];
         p->element[1][1] = c[1] + dis[1][0];
         p->element[3][0] = c[0] - dis[2][1];
         p->element[3][1] = c[1] + dis[2][0];
         break;
      default:
         break;
   } // End switch


   // Clear the space where the block used to be
   board[a[0]][a[1]] = ' ';
   board[b[0]][b[1]] = ' ';
   board[c[0]][c[1]] = ' ';
   board[d[0]][d[1]] = ' ';

   // Re-assign temps to new element positions
   a[0] = p->element[0][0]; a[1] = p->element[0][1];
   b[0] = p->element[1][0]; b[1] = p->element[1][1];
   c[0] = p->element[2][0]; c[1] = p->element[2][1];
   d[0] = p->element[3][0]; d[1] = p->element[3][1];

   // Fill in the new location of the block
   board[a[0]][a[1]] = p->letter;
   board[b[0]][b[1]] = p->letter;
   board[c[0]][c[1]] = p->letter;
   board[d[0]][d[1]] = p->letter;

   display(board);
   
} // End rotate

// Function tobottom
// Repeatedly calls the advance function until the block is placed
void tobottom(char board[20][10], struct Tetr *p)
{
   while(!advance(board, p));
   display(board);
} // End tobottom

// Function isrowfull
// Checks to see if any rows have been filled and calls the rowclear function
// accordingly
void isrowfull(char board[20][10], int *pt)
{
   static int x; // Number of lines cleared
   int i, j;

   // Check each row, beginning from the bottom
   for(i = 19; i >= 0; i--){
      for(j = 0; j < 10; j++){
         if(!isalpha(board[i][j])){
            j = 9;   // If a row has a space, skip it
         }else if(j == 9){
            // If the end of a row has been reached, clear it
            rowclear(board, i);
            i++; // See explanation below loops
            x++; // Increase counter of cleared rows
         } // End if
      } // End for
   } // End for

// The rowclear function automatically moves the higher rows down to fill
// the space that the cleared row took, so i is incremented so that the loop
// checks the row that was just moved down

   // If 10 rows have been cleared, decrease the time between advancements
   if(x == 10){
      *pt = *pt - 20000;
      x = 0;
   } // End if

   display(board);

} // End isrowfull

// Function rowclear
// Clears a row that is full and moves down the rows above it.
void rowclear(char board[20][10], int i)
{
   int j, k;

   // Clear the current row
   for(j = 0; j < 10; j++)
      board[i][j] = ' ';

   // Starting from the cleared row, assign each element the value of the
   // element above it and move up the board until the top row is reached
   for(j = i; j > 0; j--)
      for(k = 0; k < 10; k++)
         board[j][k] = board[j-1][k];

   // Clear the top row
   for(k = 0; k < 10; k++)
      board[0][k] = ' ';

   display(board);

} // End rowclear

// Function advance
// Moves the active block down one row, or places the block if
// another block is below it. Returns 1 if the active block has been
// placed, and returns 0 otherwise.
int advance(char board[20][10], struct Tetr *p)
{
   int a[2], b[2], c[2], d[2]; // The locations of the elements
                               // of the active block

   // Assign temps to use while checking the board
   a[0] = p->element[0][0]; a[1] = p->element[0][1];
   b[0] = p->element[1][0]; b[1] = p->element[1][1];
   c[0] = p->element[2][0]; c[1] = p->element[2][1];
   d[0] = p->element[3][0]; d[1] = p->element[3][1];

   // If the block is at the bottom of the board, return 1
   if((a[0]==19) || (b[0]==19) || (c[0]==19) || (d[0]==19))
      return 1;

   // If the space below block a isn't active and is filled, return 1
   if( ((a[0]+1 != b[0]) || (a[1] != b[1])) 
    && ((a[0]+1 != c[0]) || (a[1] != c[1]))
    && ((a[0]+1 != d[0]) || (a[1] != d[1])) ){
      if(isalpha(board[a[0]+1][a[1]])){
         return 1;
      } // End if
   } // End if

   // If the space below block b isn't active and is filled, return 1
   if( ((b[0]+1 != a[0]) || (b[1] != a[1])) 
    && ((b[0]+1 != c[0]) || (b[1] != c[1]))
    && ((b[0]+1 != d[0]) || (b[1] != d[1])) ){
      if(isalpha(board[b[0]+1][b[1]])){
         return 1;
      } // End if
   } // End if

   // If the space below block c isn't active and is filled, return 1
   if( ((c[0]+1 != a[0]) || (c[1] != a[1])) 
    && ((c[0]+1 != b[0]) || (c[1] != b[1]))
    && ((c[0]+1 != d[0]) || (c[1] != d[1])) ){
      if(isalpha(board[c[0]+1][c[1]])){
         return 1;
      } // End if
   } // End if

   // If the space below block d isn't active and is filled, return 1
   if( ((d[0]+1 != a[0]) || (d[1] != a[1])) 
    && ((d[0]+1 != b[0]) || (d[1] != b[1]))
    && ((d[0]+1 != c[0]) || (d[1] != c[1])) ){
      if(isalpha(board[d[0]+1][d[1]])){
         return 1;
      } // End if
   } // End if

   // If there are no problems, move each element down one row
   p->element[0][0]++;
   p->element[1][0]++;
   p->element[2][0]++;
   p->element[3][0]++;

   // Clear the space where the block used to be
   board[a[0]][a[1]] = ' ';
   board[b[0]][b[1]] = ' ';
   board[c[0]][c[1]] = ' ';
   board[d[0]][d[1]] = ' ';

   // Re-assign temps to the block's new position
   a[0] = p->element[0][0];
   b[0] = p->element[1][0];
   c[0] = p->element[2][0];
   d[0] = p->element[3][0];

   // Fill the board with the block's new position
   board[a[0]][a[1]] = p->letter;
   board[b[0]][b[1]] = p->letter;
   board[c[0]][c[1]] = p->letter;
   board[d[0]][d[1]] = p->letter;

   return 0;
} // End advance

// Function isgameover
// Checks to see if the game is over by seeing if a newly generated block
// can be properly placed. Returns 1 if the game is over, 0 otherwise.
int isgameover(char board[20][10], int type)
{
   int i;

   // Check the first 3 rows based on the block to be placed
   switch(type){
      case 0:
         for(i = 0; i <= 3; i++)
            if(isalpha(board[0][i+3])) return 1;
         break;
      case 1:
         for(i = 4; i < 6; i++){
            if(isalpha(board[0][i])) return 1;
            if(isalpha(board[1][i])) return 1;
         } // End for
         break;
      case 2:
         if(isalpha(board[1][4]) || isalpha(board[1][5]))
            return 1;
         if(isalpha(board[0][5]) || isalpha(board[0][6]))
            return 1;
         break;
      case 3:
         if(isalpha(board[0][4]) || isalpha(board[0][5]))
            return 1;
         if(isalpha(board[1][5]) || isalpha(board[1][6]))
            return 1;
         break;
      case 4:
         if(isalpha(board[0][5]) || isalpha(board[1][4]))
            return 1;
         if(isalpha(board[1][5]) || isalpha(board[1][6]))
            return 1;
         break;
      case 5:
         for(i = 0; i < 3; i++)
            if(isalpha(board[i][4])) return 1;
         if(isalpha(board[2][5])) return 1;
         break;
      case 6:
         for(i = 0; i < 3; i++)
            if(isalpha(board[i][4])) return 1;
         if(isalpha(board[0][5])) return 1;
         break;
      default:
         break;
   } // End switch

   return 0;
} // End isgameover

// Function gameover
// Displays the 'Game Over' screen and prompts the user to either
// play again or quit the program.
char gameover(int wd, int ht)
{
   char c = '0';

   gfx_flush();
   gfx_clear();

   gfx_color(255, 255, 255);
   gfx_text(275, 50, "GAME OVER");
   gfx_text(150, 200, "Press [space] to play again, or press 'q' to quit.");

   while(1){
      c = gfx_wait();
      if(c == ' ' || c == 'q') return c;
   } // End while

} // End gameover

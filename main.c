/* Made by Michael W. Wilson (MichaelPineapple) on 8/4/19 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

/* Defined board symbols */
#define SYM_WATER '~'
#define SYM_SHIP 'O'
#define SYM_SEL '*'
#define SYM_HIT 'X'
#define SYM_UNKNOWN '?'
#define SYM_BLANK ' '

/* Defined contants */
#define GRID_SIZE 10
#define INVALID_COORDS cls(); printf("Invalid input! (Example: A0)\n\nTry again...");

/* Coord struct - Contains a 'row' and 'column' value */
typedef struct Coord
{
    int row, col;
} Coord;


/* Clear the console (It's a little jankie) */
void cls()
{
    //system("cls");
    for (int i = 0; i < 50; i++) printf("\n");
}

/* Flush the input stream */
void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Print the given grid to the console */
void printGrid(char _grid[GRID_SIZE][GRID_SIZE])
{
    printf("\n\n  ");
    int tmp =  'A'+GRID_SIZE;
    for (int a = 'A'; a < tmp; a++) printf("%c ", a);
    printf("\n");
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf("%d ", i);
        for (int k = 0; k < GRID_SIZE; k++) printf("%c ", _grid[i][k]);
        printf("\n");
    }
    printf("\n\n");
}

/* A function which displays the given grid, prompts the user to enter a coordinate value, and returns the chosen grid element */
Coord getCoords(char _grid[GRID_SIZE][GRID_SIZE])
{
    char col;
    int row, loop = 1;
    do
    {
        printGrid(_grid);
        printf("Select Coordinates:");

        char* input;
        if (scanf("%2s", input))
        {
            col = toupper(input[0]);
            row = input[1] - '0';

            if (row > -1 && row < GRID_SIZE)
            {
                if (col >= 'A' && col < 'A'+GRID_SIZE) loop = 0;
                else { INVALID_COORDS }
            }
            else { INVALID_COORDS }
        }
        else { INVALID_COORDS }

        flush();
    }
    while (loop);

    Coord exp;
    exp.col = col - 'A';
    exp.row = row;

    return exp;
}

/* Clears the given grid */
void clearGrid(char _grid[2][GRID_SIZE][GRID_SIZE], char waterSym)
{
    for (int p = 0; p < 2; p++)
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int k = 0; k < GRID_SIZE; k++) _grid[p][i][k] = waterSym;
        }
    }
}

/* Main function, duh! */
int main()
{
    // Declare game variables
    int loop, player;
    int health[2] = {0, 0};
    char grid[2][GRID_SIZE][GRID_SIZE];
    clearGrid(grid, SYM_WATER);

    for (player = 0; player < 2; player++)
    {
        // Introduce player
        cls();
        printf("\n_________________________________________________________\nPLAYER %d\n", player);

        for (int ship = 0; ship < 3; ship++)
        {

            // Inform user of ship num
            printf("\n**Place your ");
            switch (ship)
            {
                case 0: printf("FIRST"); break;
                case 1: printf("SECOND"); break;
                case 2: printf("THIRD"); break;
                default: printf("!ERROR!");
            }
            printf(" ship**\n");


            // Get first coord
            loop = 1;
            Coord b;
            do
            {
                b = getCoords(grid[player]);
                if (grid[player][b.row][b.col] == SYM_SHIP) { cls(); printf("You already have a ship in that spot!\nTry again..."); }
                else loop = 0;
            }
            while (loop);
            grid[player][b.row][b.col] = SYM_SHIP;
            health[player]++;


            // Highlight adjacent coords
            char tmpGrid[GRID_SIZE][GRID_SIZE];

            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int k = 0; k < GRID_SIZE; k++)
                {
                    char e = grid[player][i][k];
                    if (e == SYM_WATER) tmpGrid[i][k] = SYM_BLANK;
                    else tmpGrid[i][k] = e;
                }
            }

            for (int i = -1; i < 2; i++)
            {
                for (int k = -1; k < 2; k++)
                {
                   int r = b.row+i, c = b.col+k;
                   if (r < GRID_SIZE && r >= 0 && c < GRID_SIZE && c >= 0 && tmpGrid[r][c] != SYM_SHIP) tmpGrid[r][c] = SYM_SEL;
                }
            }


            // Get second coord
            cls();
            loop = 1;
            do
            {
                printf("Pick one of the *");
                b = getCoords(tmpGrid);
                if (tmpGrid[b.row][b.col] == SYM_SEL) loop = 0;
                else { cls(); printf("Invalid coordinates!\nTry again...\n\n"); }
            }
            while (loop);
            grid[player][b.row][b.col] = SYM_SHIP;
            health[player]++;
        }

        // Display final grid
        cls();
        printGrid(grid[player]);
        printf("Press ENTER to end turn...");
        getch();
        cls();
    }

    // Initialize values for battle sequence
    loop = 1;
    player = 0;
    char enemyGrid[2][GRID_SIZE][GRID_SIZE];
    clearGrid(enemyGrid, SYM_BLANK);

    // Battle sequence
    do
    {
        // Introduce player
        cls();
        printf("\n_________________________________________________________\nPLAYER %d\nPick coordniates to attack.", player);

        // Get coords to attack from player
        Coord b = getCoords(enemyGrid[player]);
        cls();

        // determine if the player hit a ship or missed. React accordingly
        if (grid[!player][b.row][b.col] == SYM_SHIP)
        {
            printf("\nHIT!");
            enemyGrid[player][b.row][b.col] = SYM_HIT;
            grid[!player][b.row][b.col] = SYM_HIT;
            health[!player]--;
        }
        else
        {
            printf("\nMISS...");
            enemyGrid[player][b.row][b.col] = SYM_WATER;
        }

        printGrid(enemyGrid[player]);

        // Finsh turn
        printf("Press ENTER to end turn...");
        getch();
        cls();
        player = !player;

        // If a player has lost all ships, the other player wins
        if (health[player] <= 0)
        {
            loop = 0;
            printf("\n\nPLAYER %d WINS!!", !player);
        }

    }
    while (loop);

    return 0;
}




















/*
char orient = 'x';


do
{
    printf("\n(H)orizontal or (V)ertical: ");
    if (scanf("%c", &orient))
    {
        orient = toupper(orient);
        if (orient == 'V' || orient == 'H') loop0 = 0;
        else INV_ORIENT
    }
    else INV_ORIENT
    flush();
}
while (loop0);

printf("\nYou chose ");
if (orient == 'H') printf("HORIZONTAL");
else if (orient == 'V')  printf("VERTICAL");
else printf("ERROR");
printf("!\n");

printf("\nPlace your SYM_SHIP.\n\n");

*/

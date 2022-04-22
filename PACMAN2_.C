#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define SIZE_R 13
#define SIZE_C 47
#define ITEM_TOTAL 137

typedef enum { false, true } bool;

typedef struct {
    int row;
    int column;
} position;

void screenupdate(char maze[][SIZE_C + 1], int rows, int items)
{
    int i, j;
    system("cls"); // for turbo c compiler use clrscr();
    if (items)
        printf("\nw = move up, s = move down, a = move left, d = move right, q = quit game, items left: %d \n", items);

    for (i = 0; i < rows; i++)  {
        printf("%s\n", maze[i]);
    }
}

bool hasBlock(char direction, position *playerPos, char maze[SIZE_R][SIZE_C + 1], bool isGhost) {
    bool val;
    switch (direction) {
        case 'a':
            val = (maze[playerPos->row][playerPos->column - 1] == '#' || ((playerPos->column - 2) < 0) || maze[playerPos->row][playerPos->column - 2] == '#');
            if (isGhost) {val = val || maze[playerPos->row][playerPos->column - 2] == '$';}
            return val;
        case 'd':
            val = (maze[playerPos->row][playerPos->column + 1] == '#' || ((playerPos->column + 2) > SIZE_C) || maze[playerPos->row][playerPos->column + 2] == '#');
            if (isGhost) {val = val || maze[playerPos->row][playerPos->column + 2] == '$';}
            return val;
        case 'w':
            val = (((playerPos->row - 1) < 0) || maze[playerPos->row - 1][playerPos->column] == '#');
            if (isGhost) {val = val || maze[playerPos->row - 1][playerPos->column] == '$';}
            return val;
        case 's':
            val = (((playerPos->row + 1) < 0) || maze[playerPos->row + 1][playerPos->column] == '#');
            if (isGhost) {val = val || maze[playerPos->row + 1][playerPos->column] == '$';}
            return val;
        default:
            return true;
    }
}

int playerMove(char direction, position *playerPos, int *deathFlag, int items, int *printFlag, char maze[SIZE_R][SIZE_C + 1]) {
    if (hasBlock(direction, playerPos, maze, false)) {
        *printFlag = -1;
    } else {
        int nextBlockRow, nextBlockCol;
        switch (direction) {
        case 'a':
            nextBlockRow = playerPos->row;
            nextBlockCol = playerPos->column - 2;
            break;
        case 'd':
            nextBlockRow = playerPos->row;
            nextBlockCol = playerPos->column + 2;
            break;
        case 'w':
            nextBlockRow = playerPos->row - 1;
            nextBlockCol = playerPos->column;
            break;
        case 's':
            nextBlockRow = playerPos->row + 1;
            nextBlockCol = playerPos->column;
            break;
        default:
            break;
        }
        if (maze[nextBlockRow][nextBlockCol] == '*') {
            maze[nextBlockRow][nextBlockCol] == ' ';
            items--;
        }
        if (maze[nextBlockRow][nextBlockCol] == '$') {
            maze[playerPos->row][playerPos->column] == ' ';
            maze[nextBlockRow][nextBlockCol] == '$';
            *deathFlag = 1;
        }
        else {
            maze[playerPos->row][playerPos->column] = ' ';
            maze[nextBlockRow][nextBlockCol] = '@';
            switch (direction)
            {
            case 'a':
                playerPos->column -= 2;
                break;
            case 'd':
                playerPos->column += 2;
                break;
            case 'w':
                playerPos->row -= 1;
                break;
            case 's':
                playerPos->row += 1;
                break;
            default:
                break;
            }
        }
    }
    return items;
}

void ghostMove(char direction, position *ghostPos, int *changeFlag, int *deathFlag, int *itemFlag, char maze[SIZE_R][SIZE_C + 1]) {
    if (*changeFlag == 0 && !hasBlock(direction, ghostPos, maze, true)) {
        int nextBlockRow, nextBlockCol;
        switch (direction) {
            case 'a':
                nextBlockRow = ghostPos->row;
                nextBlockCol = ghostPos->column - 2;
                break;
            case 'd':
                nextBlockRow = ghostPos->row;
                nextBlockCol = ghostPos->column + 2;
                break;
            case 'w':
                nextBlockRow = ghostPos->row - 1;
                nextBlockCol = ghostPos->column;
                break;
            case 's':
                nextBlockRow = ghostPos->row + 1;
                nextBlockCol = ghostPos->column;
                break;
            default:
                break;
        }
        if (maze[nextBlockRow][nextBlockCol] == '@') 
            *deathFlag = 1;
        
        if (*itemFlag)
            maze[ghostPos->row][ghostPos->column] = '*';
        else
            maze[ghostPos->row][ghostPos->column] = ' ';

        if (maze[nextBlockRow][nextBlockCol] == '*')
            *itemFlag = 1;
        else
            *itemFlag = 0;

        maze[nextBlockRow][nextBlockCol] = '$';
        switch (direction) {
            case 'a':
                ghostPos->column -= 2;
                break;
            case 'd':
                ghostPos->column += 2;
                break;
            case 'w':
                ghostPos->row -= 1;
                break;
            case 's':
                ghostPos->row += 1;
                break;
            default:
                break;
        }
        *changeFlag = 1;
    }
}

int main()
{

    int i, user_input, flag_p, flag_d, flag_c, temp, flag_i1, flag_i2, items;
    position player;
    position ghost_1;
    position ghost_2;

    char maze[SIZE_R][SIZE_C + 1] = {
	"###############################################",
	"# * * $ * * * * * * * * * * * * * * * * $ * * #",
	"# * * ##### * * * ########### * * * ##### * * #",
	"# * * #   # * * * #         # * * * #   # * * #",
	"# * * #   # * * * #         # * * * #   # * * #",
	"# * * #   # * * * #         # * * * #   # * * #",
	"# * * ##### * * * ########### * * * ##### * * #",
	"# * * * * * * * * * * * @ * * * * * * * * * * #",
	"# * ####### * * * ########### * * * ####### * #",
	"# * #     # * * * #         # * * * #     # * #",
	"# * ####### * * * ########### * * * ####### * #",
	"# * * * * * * * * * * * * * * * * * * * * * * #",
	"###############################################"};
    i = user_input = flag_p = flag_d = flag_c = temp = flag_i1 = flag_i2 = 0;
    items = ITEM_TOTAL;
	
	printf("WELCOME TO PACMAN! \n");
    printf("'@' is PACMAN, '*' are objects, and '$' are the ghosts! \n");
    printf("Press any key to start...\n");
    getch();
    for (i = 0; i < SIZE_R; i++)
    {
	maze[i][SIZE_C] = '\0';
    }
    screenupdate(maze, SIZE_R, items);
    player.row = 7;
    player.column = 24;
    ghost_1.row = 1;
    ghost_1.column = 6;
    ghost_2.row = 1;
    ghost_2.column = 40;

    while (1)
    {
        if (user_input == 0)
            user_input = getch();
        else
            return -1;

        switch (user_input)
        {
            case 'a':
            case 'd':
            case 'w':
            case 's':
                items = playerMove(user_input, &player, &flag_d, items, &flag_p, maze);
                break;
            case 'q':
                printf("Thank You for playing this game, Bye!");
                return 0;
            default:
                flag_p = -1;
        }
        if ((flag_d == 0) && (flag_p == 0)) {
            temp = rand() % 10;
            if (temp == 0)
                ghostMove('d', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

            if (temp == 1)
                ghostMove('s', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

            if (temp == 2)
                ghostMove('w', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

            if (temp == 3)
                ghostMove('a', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

            if (temp > 3) {
                if ((ghost_1.column - player.column) < 0)
                    ghostMove('d', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

                if ((ghost_1.column - player.column) > 0)
                    ghostMove('a', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

                if ((ghost_1.row - player.row) < 0)
                    ghostMove('s', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);

                if ((ghost_1.row - player.row) > 0)
                    ghostMove('w', &ghost_1, &flag_c, &flag_d, &flag_i1, maze);
            }
        }
        flag_c = 0;

        if ((flag_d == 0) && (flag_p == 0)) {
            temp = rand() % 10;
            if (temp == 0)
                ghostMove('d', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);

            if (temp == 1)
                ghostMove('s', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);

            if (temp == 2)
                ghostMove('w', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);

            if (temp == 3)
                ghostMove('a', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);

            if (temp > 3) {
                if ((ghost_2.column - player.column) < 0)
                    ghostMove('d', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);

                if ((ghost_2.column - player.column) > 0)
                    ghostMove('a',&ghost_2, &flag_c, &flag_d, &flag_i2, maze);

                if ((ghost_2.row - player.row) < 0)
                    ghostMove('s', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);

                if ((ghost_2.row - player.row) > 0)
                    ghostMove('w', &ghost_2, &flag_c, &flag_d, &flag_i2, maze);
            }
        }
        flag_c = 0;
        if (flag_p != -1)
            screenupdate(maze, SIZE_R, items);
        if (flag_d) {
            printf("Aw... You Lose!\n");
            return 0;
        }
        if (!items) { 
            printf("Congratulations! You Win!\n");
            return 0;
         }
        user_input = 0;
        flag_p = 0;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "connect4.h"

// Display our gameboard as ASCII characters
void display_board_ASCII(int board[][COLUMNS]) {

    int i,j;
    char symbol;

    // Diplay the actual game board
    printf("\n");
    for (i=0;i<ROWS;i++) {

	printf("     ");
        for (j=0;j<COLUMNS;j++) {
	    if (board[i][j] == 1)
		symbol = 'X';
	    else if (board[i][j] == 2)
 		symbol = 'O';
	    else
		symbol = '.';
            printf(" %c ",symbol);
        }
        printf("\n");
    }
    // Display the column numbers
    printf("     ");
    for (i=0;i<COLUMNS;i++)
         printf("___");
    printf("\n");
    printf("     ");
    for (i=0;i<COLUMNS;i++)
         printf(" %d ",i+1);
    printf("\n\n");

}


// Start the simulation
int main() {	

	int board[ROWS][COLUMNS] = {0};

	// Seed the random number generator
	srand(time(NULL));


	// Inform the user
	printf("\n Program has started ... \n");
  	
	// Display the game board
	display_board_ASCII(board);

	printf("\n");
	
	int user_move, AI_move;
	while (1) {

		clock_t start, end;
    	double cpu_time_used;
    	start = clock();

		AI_move = computer_move(board, 1, 1);

		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("Time taken: %f seconds\n", cpu_time_used);


		add_move_board(board, AI_move, 1);
		if (check_winning(board, AI_move) != 0) {
			display_board_ASCII(board);
			printf("AI won! \n");
			return 1;
		}

		display_board_ASCII(board);
		

		user_move = player_move(board);
		add_move_board(board, user_move, 2);
		if (check_winning(board, user_move) != 0) {
			display_board_ASCII(board);
			printf("Player won! \n");
			return 1;
		}

	}
	
}


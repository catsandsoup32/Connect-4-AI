#include "connect4.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// Checks whether 'move' is valid given the current state of 'board'.
// It returns 1 when the move is valid, otherwise 0.
int valid_move(int board[][COLUMNS], int move) {
	if ( move < 7 && move >= 0 && board[0][move] == 0 ) {
		return 1;
	} else {
		return 0;
	}
}


// Update the board with a move for a specific player.
// Returns 0 in case of failure (the move is not a valid
// one); otherwise it returns 1. 
int add_move_board(int board[][COLUMNS], int move, int player) {
	int i = 5;
	if ( valid_move(board, move) ) {
		while ( board[i][move] != 0 ) {
			i--;
		}
		board[i][move] = player;
		return 1;
	} else {
		return 0;
	}
}
	

// It asks the user to pick a column to play, and repeats
// this until a valid one is chosen.
// The user should enter a number between 1 and COLUMNS (1-indexed).
// It returns the column in the board array, so a value between 
// 0 and COLUMNS-1 (0-indexed)
int player_move(int board[][COLUMNS]) {
	int move;
	printf("Enter an integer column 1-7: ");
	scanf("%d", &move);
	return move-1;
}


// Checks whether there is a winning player on the board.
// It gets passed the last move that was made.
// It returns 0 if there is no winner, or the otherwise the number
// of the player who won (1 or 2).

int check_straight(int board[][COLUMNS], int row, int col, int player) {

	// Check vertical
	// No need to go up
	if ( row < 3) // if row = 3, row+3 = 6 errors
		if ( board[row][col] == player && board[row+1][col] == player && board[row+2][col] == player && board[row+3][col] == player) {
			// printf("Vertical win for %d", player);
			return 1;
		}
	
	// Check horizontal 
	int i = 0; int counter = 0; // Needs to start at 0 or else initial column 7 and adding one to that will error
	while ( board[row][col+i] == player ) {
		counter++;
		i++;
		if ( col+i == 7 ) break;
	}
	i = 0;
	while ( board[row][col-i] == player ) {
		counter++;
		i++;
		if ( col-i == -1 ) break;
	}
	if ( counter-2 == 3 ) {
		// printf("Horizontal win");
		return 1; // Need to subtract 2 because col+-0 == player is a given, twice
	}

	return 0;
}

int check_diag(int board[][COLUMNS], int row, int col, int player) {

	// Check positive slope
	int i = 0, counter = 0;
	while ( board[row-i][col+i] == player ) { // NE
		counter++;
		i++;
		if ( row-i == -1 || col+i == 7 ) break;
	}
	i = 0;
	while ( board[row+i][col-i] == player ) { // SW
		counter++;
		i++;
		if ( row+i == 6 || col-i == -1 ) break;
	}
	if ( counter-2 == 3 ) {
		// printf("Positive slope win");
		return 1;
	}

	// Check negative slope
	i = 0; counter = 0;
	while ( board[row-i][col-i] == player ) { // NW
		counter++;
		i++;
		if ( row-i == -1 || col-i == -1 ) break;
	}
	i = 0;
	while ( board[row+i][col+i] == player ) { // SE
		counter++;
		i++;
		if ( row+i == 6 || col+i == 7 ) break;
	}
	if ( counter-2 == 3 ) {
		// printf("Negative slope win");
		return 1;
	}
	return 0;
}


int check_winning(int board[][COLUMNS], int last_move) {
	int i = 0;
	while ( board[i][last_move] == 0 ) {
		if (i == 5) break;
		i++;
	}
	int row = i;
	int player = board[row][last_move];
	
	if ( check_straight(board, row, last_move, player) || check_diag(board, row, last_move, player) ) {
		return player;
	} else {
		return 0;
	}
	
}

// --------------------------------------------- Minimax helpers ---------------------------------------

int eval_window(int w[], int AI_num, int player_num) { 
	int score = 0, AI_counter = 0, player_counter = 0, i;
	
	for (i=0; i<4; i++) {
		if ( w[i] == AI_num ) AI_counter++;
		if ( w[i] == player_num ) player_counter++;
	}

	// Scores of 3
	if ( AI_counter == 3 && player_counter == 0 ) score += 5;
	if ( player_counter == 3 && AI_counter == 0 ) score -= 30;

	
	// Scores of 2
	if ( AI_counter == 2 && player_counter == 0 ) score += 3;
	if ( player_counter == 2 && AI_counter == 0 ) score -= 3;

	return score;
}
	

int evaluate_position(int board[][COLUMNS], int AI_num) {
	int score = 0, i, j, offset;
	int player_num = (AI_num == 1) ? 2 : 1;
	
	// Center
	for (i=0; i<6; i++) {
		if ( board[i][3] == AI_num ) score += 2;
	}
	
	// Horizontal checks
	for (i=0; i<6; i++) {
		offset = 0;
		while (offset < 4) { // For each sliding window, less than 4 because 4+3 = 7 errors
			int window[4] = { board[i][0+offset], board[i][1+offset], board[i][2+offset], board[i][3+offset] };
			score += eval_window( window, AI_num, player_num );
			offset++;
		}
	}

	// Vertical checks
	for (j=0; j<7; j++) { 
		offset = 0;
		while (offset < 3) { // For each sliding window, less than 3 because 3+3 = 6 errors
			int window[4] = { board[i][0+offset], board[i][1+offset], board[i][2+offset], board[i][3+offset] };
			score += eval_window( window, AI_num, player_num );
			offset++;
		}
	}

	// Positive slope checks

	return score;
}
		


int* minimax(int board[][COLUMNS], int depth, int max, int last_move, int alpha, int beta, int AI_num, int opposing_num) {

	// Array for return
	static int return_arr[2]; 
	//printf("%d \t %d \n", return_arr[0], return_arr[1]);
	
	// Base case: reached max depth or leaf node
	int temp;
	if (depth == 0 || check_winning(board, last_move) ) { 
		if ( check_winning(board, last_move) == 1) { // First player is always maximizing
			return_arr[1] = 1000;
		} else if ( check_winning(board, last_move) == 2) { // Always minimizing
			return_arr[1] = -1000;
		} else {
			temp = evaluate_position(board, AI_num); // *** AI_num parameter 
			return_arr[1] = temp;
		}
		return return_arr;
	}

	if (max) {
		int value = -1000000; // negative infinity
		int i, new_score;
		for (i=0; i<COLUMNS; i++) {
		if (valid_move(board, i)) {
			int board_copy[ROWS][COLUMNS] = {{0}};
			int m, n;
			for (m=0; m<6; m++) 
				for (n=0; n<7; n++)
					board_copy[m][n] = board[m][n];
			
			add_move_board(board_copy, i, 1); // Will always be one (maximizing)
			new_score = minimax(board_copy, depth-1, 0, i, alpha, beta, AI_num, opposing_num)[1];
			if ( new_score > value ) {
				value = new_score;
				return_arr[0] = i;
			}

			if ( value >= alpha ) alpha = value;
			if (alpha >= beta) break;
		}}
		return_arr[1] = value;
		return return_arr;
		
	} else {
		int value = 1000000; 
		int i, new_score;
		for (i=0; i<COLUMNS; i++) {
		if (valid_move(board, i)) {
			
			int board_copy[ROWS][COLUMNS] = {{0}};
			int m, n;
			for (m=0; m<6; m++) 
				for (n=0; n<7; n++)
					board_copy[m][n] = board[m][n];
			
			add_move_board(board_copy, i, 2); // Will always be two for minimizing 
			new_score = minimax(board_copy, depth-1, 1, i, alpha, beta, AI_num, opposing_num)[1];
			if ( new_score < value ) {
				value = new_score;
				return_arr[0] = i;
			}

			if ( value <= beta ) beta = value;
			if (alpha >= beta) break;

		}}
		return_arr[1] = value;
		return return_arr;
		
	}
}
	

// This function chooses a move for the computer player,
// given the state of the board and the player number the
// computer is playing with.
// It supports two modes: 0 for making random valid moves
// and 1 for using a more advanced algorithm.
// It returns the move chosen (a value between 0 an COLUMNS-1).
int computer_move(int board[][COLUMNS], int player, int mode) {
	int random_move;
	static int state[ROWS][COLUMNS] = {{0}};
	
	if ( mode == 0 ) {
		random_move = rand() % 7;
		while ( valid_move(board, random_move) != 1 ) 
			random_move = rand() % 7;
		return random_move;
	} else {

		static int counter = 0, play_order = 0, opposing_num = 2;
		int i, j, last_col;
		
		if (counter == 0) {
			for (i=0; i<6; i++)
				for (j=0; j<7; j++) 
					if (board[i][j] != 0) counter++;
			if (counter > 0) {
				counter++;
				play_order = 1; // AI plays second
				opposing_num = 1; // Thus player plays first
			} else {
				counter++;
				return 3; // AI plays first, set move to center
			}
		}
		

		// Loop through board and get last move
		for (i=0; i<6; i++)
			for (j=0; j<7; j++) 
				if (board[i][j] == opposing_num && state[i][j] != 1) {
					last_col = j;
					state[i][j] = 1; // Holds already-seen moves
				}
		
		int move;	
		if (play_order) {
			move = minimax(board, 4, 1, last_col, -1000000, 1000000, 2, opposing_num)[0]; // If AI plays second 
		} else {
			move = minimax(board, 4, 1, last_col, -1000000, 1000000, 1, opposing_num)[0]; // If AI plays first
		}

		return move;

	}		
}	












	

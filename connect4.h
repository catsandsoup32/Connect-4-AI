// Constants
#define ROWS		6
#define COLUMNS		7

// Supporting functions for connect four
int valid_move(int board[][COLUMNS], int move);
int check_winning(int board[][COLUMNS], int last_move);
int player_move(int board[][COLUMNS]);
int computer_move(int board[][COLUMNS], int player, int mode);
int add_move_board(int board[][COLUMNS], int move, int player);



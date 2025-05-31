#ifndef BOARD_H
#define BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

int init_board_display();
void display_board(char board[8][8]);
void cleanup_board_display();

#ifdef __cplusplus
}
#endif

#endif

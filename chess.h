#ifndef CHESS_H
#define CHESS_H

#include "board.h"

// Defines a player
class Player {
private:
    std::string name;
    int num_pieces;
    bool in_check;
    bool is_white;
public:
    Player(std::string name_in, bool is_white_in);
    //bool in_checkmate();
    bool valid_move(std::string move, Board &board);
    void make_turn(Board &board);
};

void welcome_message();
void print_rules();

#endif

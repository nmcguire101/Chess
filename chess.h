#ifndef CHESS_H
#define CHESS_H

#include "board.h"

// Defines a player
class Player {
private:
    std::string name;
    std::string pieces;
    int num_pieces;
    bool in_check;
    bool is_white;
    Position king;
public:
    Player(std::string name_in, bool is_white_in);
    bool in_checkmate(Board &board);
    bool valid_start(Position &start, Board &board);
    bool valid_move(Position &start, Position &end, Board &board);
    void make_turn(Board &board);
    void update_check(Board &board);
};

void welcome_message();
void print_rules();

#endif

#ifndef CHESS_H
#define CHESS_H

#include "board.h"

// Defines a player
class Player {
private:
    std::string name;
    bool in_check;
    bool is_white;
    std::vector<Piece> pieces;
    Position king;
    Piece current;

public:
    Player(std::string name_in, bool is_white_in);
    std::vector<Piece> & get_pieces();
    void set_pieces(std::vector<Piece> &pieces_in);
    bool valid_start(Position &start);
    bool valid_move(Board &board, Position &end, std::vector<Piece> &opponents);
    bool make_turn(Board &board, std::vector<Piece> &opponents_pieces);
};

void welcome_message();
void print_rules();

#endif

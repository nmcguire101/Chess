#ifndef BOARD_H
#define BOARD_H

constexpr auto WHITE_SQUARE = 0xDB;
constexpr auto BLACK_SQUARE = 0xFF;

#include <vector>
#include <string>

// Defines a position on the board
struct Position {
    int row;
    int column;
};

/* Prints 8x8 simple lined board. Pieces defined as follows: King (K), Queen (Q),
   Bishop (B), Knight (N), Rook (R), Pawn (P) */
class Board {
private:
    std::vector<std::string> board;
    // attackable positions
    std::vector<std::vector<bool>> white_check;
    std::vector<std::vector<bool>> black_check;
public: 
    Board();
    void print_line(int color1, int color2, int line_in);
    void print_board();
    char board_at(Position &position);
    bool white_contains(Position &position);
    bool black_contains(Position &position);
    bool opponents_piece(bool is_white, int row, int column);
    // checks for valid piece movement for specified piece type
    bool pawn_movement(bool is_white, Position &start, Position &end);
    bool knight_movement(bool is_white, Position &start, Position &end);
    bool rook_movement(bool is_white, Position &start, Position &end);
    bool bishop_movement(bool is_white, Position &start, Position &end);
    bool queen_movement(bool is_white, Position &start, Position &end);
    bool king_movement(bool is_white, Position &start, Position &end);
    void update_white_check();
    void update_black_check();
    void perform_move(bool is_white, char piece, Position &start, Position &end);
};

#endif
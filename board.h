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
    std::vector<std::vector<bool>> white_check;
    std::vector<std::vector<bool>> black_check;
public: 
    Board();
    void print_line(int color1, int color2, int line_in);
    void print_board();
    char board_at(Position &position);
    bool opponents_piece(char piece, int row, int column);
    bool piece_movement(char piece, Position &start, Position &end);
};

#endif
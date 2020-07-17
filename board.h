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

struct Compare_position {
public:
    bool operator() (const Position &position1, const Position &position2) const {
        if (position1.row == position2.row && position1.column == position2.column) {
            return true;
        }
        return false;
    }
};

// Defines a piece
enum struct PieceName : char { Pawn, Knight, Rook, Bishop, Queen, King };

inline const char* to_string(PieceName name) {
    switch (name) {
    case PieceName::Pawn:     return "Pawn";
    case PieceName::Knight:   return "Knight";
    case PieceName::Rook:     return "Rook";
    case PieceName::Bishop:   return "Bishop";
    case PieceName::Queen:    return "Queen";
    case PieceName::King:     return "King";
    default:                  return "Unknown piece";
    }
}

struct Piece {
    PieceName name;
    Position pos;
    char letter;
    bool is_white;
    std::vector<Position> valid_moves;
};

/* Prints 8x8 simple lined board. Pieces defined as follows: King (K), Queen (Q),
   Bishop (B), Knight (N), Rook (R), Pawn (P) */
class Board {
private:
    std::vector<std::string> board;
    // attackable positions
public: 
    Board();
    void print_line(int color1, int color2, int line_in);
    void print_board();
    char board_at(Position &position);
    bool opponents_piece(bool is_white, int row, int column);
    bool do_move(Piece &piece, int end_row, int end_col, std::vector<Piece> &opponents);
    std::vector<Position> & pawn_movement(bool in_check, Piece &piece,
        std::vector<Piece> &opponents);
    std::vector<Position> & knight_movement(bool in_check, Piece &piece,
        std::vector<Piece> &opponents);
    std::vector<Position> & rook_movement(bool in_check, Piece &piece,
        std::vector<Piece> &opponents);
    std::vector<Position> & bishop_movement(bool in_check, Piece &piece,
        std::vector<Piece> &opponents);
    std::vector<Position> & queen_movement(bool in_check, Piece &piece,
        std::vector<Piece> &opponents);
    std::vector<Position> & king_movement(bool in_check, Piece &piece, 
        std::vector<Piece> &opponents);
    bool in_check_test(Position &king, std::vector<std::string> &temp_board, 
        std::vector<Piece> &opponents);
    bool in_check(Position &king, std::vector<Piece> &opponents);
    void perform_move(Piece &piece, Position &end);
};

#endif
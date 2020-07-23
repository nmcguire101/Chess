#ifndef BOARD_H
#define BOARD_H

constexpr auto WHITE_SQUARE = 0xDB;
constexpr auto BLACK_SQUARE = 0xFF;

#include <vector>
#include <string>

// Defines a position on the board
struct Position {
    int row;
    int col;
};

struct Compare_position {
public:
    bool operator() (const Position &position1, const Position &position2) const {
        if (position1.row == position2.row && position1.col == position2.col) {
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
};

/* Prints 8x8 simple lined board. Pieces defined as follows: King (K), Queen (Q),
   Bishop (B), Knight (N), Rook (R), Pawn (P) */
class Board {
private:
    Position prev;
    Piece moved;
    std::vector<std::string> board_prev;
    std::vector<std::string> board;
public: 
    Board();
    void print_line(int color1, int color2, int line_in);
    void print_board();
    bool opponents_piece(bool is_white, int row, int column);
    bool do_move(Piece &piece, Position &king, int end_row, int end_col, 
        std::vector<Piece> &opponents);

    // Functions below used to check for checkmate
    bool pawn_movement(Position &king, Piece &piece, std::vector<Piece> &opponents);
    bool knight_movement(Position &king, Piece &piece, std::vector<Piece> &opponents);
    bool rook_movement(Position &king, Piece &piece, std::vector<Piece> &opponents);
    bool bishop_movement(Position &king, Piece &piece, std::vector<Piece> &opponents);
    bool queen_movement(Position &king, Piece &piece, std::vector<Piece> &opponents);
    bool king_movement(Position &king, Piece &piece, std::vector<Piece> &opponents);

    // Movement helper functions
    bool move_forward(Piece &piece, int col, int row, Position &king,
        std::vector<Piece> &opponents);
    bool ch_move_forward(Piece &piece, int col, int row, Position &king);
    bool nothing_in_way(Piece &piece, int col, int row, int end_col, int end_row);

    bool in_check(bool is_white, Position &king, std::vector<Piece> &opponents);
    bool can_move(Position &king, std::vector<Piece> &pieces, std::vector<Piece> &opponents);
    bool valid_movement(PieceName name, Piece &piece, int end_row, int end_col);
    bool perform_move(Piece &piece, Position &king, Position &end,
        std::vector<Piece> &opponents);
};

#endif
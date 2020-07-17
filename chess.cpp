
#include "chess.h"
#include <string>
#include <iostream>

/* Order of Events:
     - Prints welcome message with rules option
     - Asks player 1 for name and to pick a color
     - Asks player 2 for name and assigns them other color
     - Prints board
     - Goes back and forth and asks each player to make a move
     - Determines if move is valid, if so performs it. If not, asks for new move
     - Keeps going until checkpoint is achieved
*/

// Player constructor
Player::Player(std::string name_in, bool is_white_in)
    : name(name_in), in_check(false), is_white(is_white_in) {
    std::vector<Piece> pieces_in;
    pieces_in.reserve(16);
    int row = 0;
    int ch = 0;
    if (!is_white_in) {
        row = 7;
        ch = 32;
    }
    std::vector<Position> valid_moves_in;
    // adds king
    Position pos{ row, 3 };
    king = pos;
    Piece temp_king{ PieceName::King, pos, 'K' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(temp_king);
    // adds queen
    pos.column = 4;
    Piece queen{ PieceName::Queen, pos, 'Q' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(queen);
    // adds rooks
    pos.column = 0;
    Piece rook1{ PieceName::Rook, pos, 'R' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(rook1);
    pos.column = 7;
    Piece rook2{ PieceName::Rook, pos, 'R' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(rook2);
    // adds knights
    pos.column = 1;
    Piece knight1{ PieceName::Knight, pos, 'N' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(knight1);
    pos.column = 6;
    Piece knight2{ PieceName::Knight, pos, 'N' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(knight2);
    // adds bishops
    pos.column = 2;
    Piece bishop1{ PieceName::Bishop, pos, 'B' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(bishop1);
    pos.column = 5;
    Piece bishop2{ PieceName::Bishop, pos, 'B' + ch, is_white_in, valid_moves_in };
    pieces_in.push_back(bishop2);
    row = 1;
    if (!is_white_in) {
        row = 6;
    }
    // adds pawns
    for (int i = 0; i < 8; ++i) {
        Position pos{ row, i };
        Piece temp{ PieceName::Pawn, pos, 'P' + ch, is_white_in, valid_moves_in };
        pieces_in.push_back(temp);
    }
    pieces = pieces_in;
}

std::vector<Piece> & Player::get_pieces() {
    return pieces;
}

// Checks that start position is within range and is a moveable piece
bool Player::valid_start(Position &start) {
    if (start.row < 0 || start.row > 7 || start.column < 0 || start.column > 7) {
        std::cout << "Start position out of board range.\n";
        return false;
    }
    Compare_position compare;
    for (int i = 0; (int)(pieces.size()); ++i) {
        if (compare(start, pieces[i].pos)) {
            std::cout << "Moving " << to_string(pieces[i].name) << "\n";
            current = pieces[i];
            return true;
        }
    }
    std::cout << "No movable piece at start position.\n";
    return false;
}

// Returns true if player's move is valid. If not, returns false
bool Player::valid_move(Position &end) {
    if (end.row < 0 || end.row > 7 || end.column < 0 || end.column > 7) {
        std::cout << "End position out of board range.\n";
        return false;
    }
    Compare_position compare;
    for (int i = 0; i < (int)(current.valid_moves.size()); ++i) {
        if (compare(end, current.valid_moves[i])) {
            return true;
        }
    }
    std::cout << "Invalid " << to_string(current.name) << " movement.\n";
    return false;
}

// Generates all of the valid moves for each piece
void Player::generate_valid_moves(bool in_check, Board &board, 
    std::vector<Piece> &opponents_pieces) {
    for (auto it : pieces) {
        switch (it.name) {
        case PieceName::Pawn:
            it.valid_moves = board.pawn_movement(in_check, it, opponents_pieces);
            break;
        case PieceName::Knight:
            it.valid_moves = board.knight_movement(in_check, it, opponents_pieces);
            break;
        case PieceName::Rook:
            it.valid_moves = board.rook_movement(in_check, it, opponents_pieces);
            break;
        case PieceName::Bishop:
            it.valid_moves = board.bishop_movement(in_check, it, opponents_pieces);
            break;
        case PieceName::Queen:
            it.valid_moves = board.queen_movement(in_check, it, opponents_pieces);
            break;
        case PieceName::King:
            it.valid_moves = board.king_movement(in_check, it, opponents_pieces);
            break;
        }
    }
}

// Returns true if player has no valid moves; checkmate/stalemate
bool Player::no_moves() {
    for (auto it : pieces) {
        if (!it.valid_moves.empty()) {
            return false;
        }
    }
    return true;
}

bool Player::make_turn(Board &board, std::vector<Piece> &opponents_pieces) {
    if (board.in_check(king, opponents_pieces)) {
        in_check = true;
    }
    generate_valid_moves(in_check, board, opponents_pieces);
    if (no_moves()) {
        return false;
    }
    std::string start_mes = ", please enter location of piece to move (ex: A2): ";
    std::string end_mes = ", please enter end location or type \"undo\" (ex: E5): ";
    // Gets start location
    std::string start;
    std::cout << name << start_mes;
    std::cin >> start;
    Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
    // Checks if valid, requests new position until valid one is given
    while (!valid_start(start_pos)) {
        std::cout << name << start_mes;
        std::cin >> start;
        Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
    }
    // Gets end location
    std::string end;
    std::cout << name << end_mes;
    std::cin >> end;
    Position end_pos{ (int)(end[0] - 'A'), (int)(end[1] - '1') };
    // If end location is not valid or says undo, starts over
    while (end == "undo" || !valid_move(end_pos)) {
        std::cout << name << start_mes;
        std::cin >> start;
        Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
        while (!valid_start(start_pos)) {
            std::cout << name << start_mes;
            std::cin >> start;
            Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
        }
        std::cout << name << end_mes;
        std::cin >> end;
        Position end_pos{ (int)(end[0] - 'A'), (int)(end[1] - '1') };
    }
    // Once valid move is given, perform move
    char piece = board.board_at(start_pos);
    board.perform_move(current, end_pos);
    if (current.name == PieceName::King) {
        king = end_pos;
    }
    return true;
}

void welcome_message() {
    std::string help;
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "Welcome to Chess! Type 'help' for rules or press any key to continue\n";
    std::cin >> help;
    if (help == "help") {
        print_rules();
    }
    std::cout << "---------------------------------------------------------------------\n";
    return;
}

void print_rules() {
    std::cout << "__________________________________________________________________\n";
    std::cout << "                        Chess: how-to-play                        \n";
    std::cout << "------------------------------------------------------------------\n\n";
    std::cout << "General guidelines:\n\n";
    std::cout << "      The ultimate aim is delivering a checkmate: trapping your\n";
    std::cout << "opponent's king. White is always first to move and players take\n";
    std::cout << "turns alternately moving one piece at a time. Movement is\n";
    std::cout << "required. If it's a player's turn to move and they're not in check\n";
    std::cout << "but have no legal moves, it is a \"Stalemate\" and it ends the game\n";
    std::cout << "in a draw. When a king is threatened with capture (but can protect\n";
    std::cout << "himself or escape), it's called check. If a king is in check, then\n";
    std::cout << "the player must make a move that eliminates the threat of capture\n";
    std::cout << "and cannot leave the king in check. Checkmate happens when a king\n";
    std::cout << "is in check and there is no legal move to escape. It ends the game.\n\n";
    std::cout << "------------------------------------------------------------------\n\n";
    std::cout << "Piece movement:\n\n";
    std::cout << "      Each type of piece has its own method of movement. A piece may be\n";
    std::cout << "moved to another position or may capture an opponent's piece, \n";
    std::cout << "replacing it on its square. With the exception of the knight, a\n";
    std::cout << "piece may not move over or through any of the other pieces.\n\n";
    std::cout << "King: can move exactly one square horizontally, vertically, or \n";
    std::cout << "      diagonally. Once in every game, each king is allowed to make\n";
    std::cout << "      a special move, known as castling.\n\n";
    std::cout << "Queen: can move any number of vacant squares diagonally, \n";
    std::cout << "       horizontally, or vertically.\n\n";
    std::cout << "Rook: can move any number of vacant squares vertically or \n";
    std::cout << "      horizontally. It also is moved while castling.\n\n";
    std::cout << "Bishop: can move any number of vacant squares in any diagonal \n";
    std::cout << "        direction.\n\n";
    std::cout << "Knight: can move one square along any rank or file and then at an \n";
    std::cout << "        angle. The knight's movement can also be viewed as an “L”\n";
    std::cout << "        or \"7\" laid out at any horizontal or vertical angle.\n\n";
    std::cout << "Pawn: can move forward one square, if that square is unoccupied. \n";
    std::cout << "      If it has not yet moved, the pawn has the option of moving\n";
    std::cout << "      two squares forward provided both squares in front of the pawn\n";
    std::cout << "      are unoccupied. A pawn cannot move backward. Pawns are the\n";
    std::cout << "      only pieces that capture differently from how they move. They\n";
    std::cout << "      can capture an enemy piece on either of the two spaces\n";
    std::cout << "      diagonal in front of them but cannot move to these spaces if\n";
    std::cout << "      they are vacant.\n\n";
    std::cout << "Rules from chesscoachonline.com\n";
    std::cout << "------------------------------------------------------------------\n\n";
    return;
}

int main() {
    welcome_message();

    // Create players
    std::string player1_name;
    std::string player2_name;

    std::cout << "First player, please enter your name: \n";
    std::cin >> player1_name;
    std::cout << "Great! " << player1_name << ", you are assigned the color white\n\n";
    Player player1(player1_name, true);

    std::cout << "Second player, please enter your name: \n";
    std::cin >> player2_name;
    std::cout << "Great! " << player2_name << ", you are assigned the color black\n\n";
    Player player2(player2_name, false);

    std::cout << "Let's begin the game...\n\n";

    // Construct board
    Board board;
    board.print_board();

    // Checks for stalemate: player whose turn it is has no legal moves
    bool game_over = false;
    do {
        std::vector<Piece> player2_pieces = player2.get_pieces();
        if (!player1.make_turn(board, player2_pieces)) {
            std::cout << "Game over. " << player2_name << " has won!\n";
            break;
        }
        std::vector<Piece> player1_pieces = player1.get_pieces();
        if (!player2.make_turn(board, player1_pieces)) {
            std::cout << "Game over. " << player1_name << " has won!\n";
            break;
        }
    } while (!game_over);

    return 0;
}
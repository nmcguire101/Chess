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
    : name(name_in), is_white(is_white_in) {
    if (is_white) {
        pieces = "PNRBQK";
        Position king_in{ 7, 3 };
        king = king_in;
    }
    else {
        pieces = "pnrbqk";
        Position king_in{ 0, 3 };
        king = king_in;
    }
    num_pieces = 16;
    in_check = false;
}

// Checks if player is in checkmate
bool Player::in_checkmate(Board &board) {
    // check if king's position is being attacked
    if (in_check) {
        // check if king can move to any surrounding position
        Position temp{ king.row - 1, king.column };
        Position temp1{ king.row + 1, king.column };
        Position temp2{ king.row, king.column + 1 };
        Position temp3{ king.row, king.column - 1};
        Position temp4{ king.row - 1, king.column - 1 };
        Position temp5{ king.row - 1, king.column + 1 };
        Position temp6{ king.row + 1, king.column - 1 };
        Position temp7{ king.row + 1, king.column + 1 };
        if (board.king_movement(is_white, king, temp) || 
            board.king_movement(is_white, king, temp1) ||
            board.king_movement(is_white, king, temp2) ||
            board.king_movement(is_white, king, temp3) ||
            board.king_movement(is_white, king, temp4) ||
            board.king_movement(is_white, king, temp5) ||
            board.king_movement(is_white, king, temp6) ||
            board.king_movement(is_white, king, temp7)) {
            return false;
        }
        // check if piece can block/capture attacker
    }
    return false;
}

// Checks that start position is within range and is a moveable piece
bool Player::valid_start(Position &start, Board &board) {
    if (start.row < 0 || start.row > 7 || start.column < 0 || start.column > 7) {
        std::cout << "Start position out of board range.\n";
        return false;
    }
    char piece = board.board_at(start);
    if (piece == pieces[0] || piece == pieces[1] || piece == pieces[2] 
        || piece == pieces[3] || piece == pieces[4] || piece == pieces[5]) {
        return true;
    }
    std::cout << "No movable piece at start position.\n";
    return false;
}

// Returns true if player's move is valid. If not, returns false
bool Player::valid_move(Position &start, Position &end, Board &board) {
    if (end.row < 0 || end.row > 7 || end.column < 0 || end.column > 7) {
        std::cout << "End position out of board range.\n";
        return false;
    }
    char piece = board.board_at(start);
    if (in_check) {

    }
    // pawn
    if (piece == pieces[0]) {
        if (!board.pawn_movement(is_white, start, end)) {
            std::cout << "Invalid pawn movement.\n";
            return false;
        }
        return true;
    }
    // knight
    else if (piece == pieces[1]) {
        if (!board.knight_movement(is_white, start, end)) {
            std::cout << "Invalid knight movement.\n";
            return false;
        }
        return true;
    }
    // rook
    else if (piece == pieces[2]) {
        if (!board.rook_movement(is_white, start, end)) {
            std::cout << "Invalid rook movement.\n";
            return false;
        }
        return true;
    }
    // bishop
    else if (piece == pieces[3]) {
        if (!board.bishop_movement(is_white, start, end)) {
            std::cout << "Invalid bishop movement.\n";
            return false;
        }
        return true;
    }
    // queen
    else if (piece == pieces[4]) {
        if (!board.queen_movement(is_white, start, end)) {
            std::cout << "Invalid queen movement.\n";
            return false;
        }
        return true;
    }
    // king
    else {
        if (!board.king_movement(is_white, start, end)) {
            std::cout << "Invalid king movement.\n";
            return false;
        }
        king = end;
        return true;
    }
}

void Player::update_check(Board &board) {
    if (is_white && board.white_contains(king)) {
        in_check = true;
    }
    else if (!is_white && board.black_contains(king)) {
        in_check = true;
    }
    in_check = false;
}

void Player::make_turn(Board &board) {
    // Gets start location
    std::string start;
    std::cout << name << ", please enter location of piece to move (ex: A2): ";
    std::cin >> start;
    Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
    while (!valid_start(start_pos, board)) {
        std::cout << name << ", please enter location of piece to move (ex: A2): ";
        std::cin >> start;
        Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
    }
    // Gets end location
    std::string end;
    std::cout << name << ", please enter location of where to move piece (ex: E5): ";
    std::cin >> end;
    Position end_pos{ (int)(end[0] - 'A'), (int)(end[1] - '1') };
    while (!valid_move(start_pos, end_pos, board)) {
        Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
        while (!valid_start(start_pos, board)) {
            std::cout << name << ", please enter location of piece to move (ex: A2): ";
            std::cin >> start;
            Position start_pos{ (int)(start[0] - 'A'), (int)(start[1] - '1') };
        }
        std::cout << name << ", please enter location of where to move piece (ex: E5): ";
        std::cin >> end;
        Position end_pos{ (int)(end[0] - 'A'), (int)(end[1] - '1') };
    }
    // Once valid move is given, perform move
    char piece = board.board_at(start_pos);
    board.perform_move(is_white, piece, start_pos, end_pos);
    update_check(board);
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

    while (!player1.in_checkmate(board) && !player2.in_checkmate(board)) {
        player1.make_turn(board);
        player2.make_turn(board);
    }

    return 0;
}
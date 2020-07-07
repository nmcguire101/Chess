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
    num_pieces = 16;
    in_check = false;
}

/*bool Player::in_checkmate() {

}*/

bool Player::valid_move(std::string move, Board &board) {
    Position start{ (int)(move[0] - 'A'), (int)(move[1] - '1') };
    if (start.row < 'A' || start.row > 'H' || start.column < 1 || start.column > 8) {
        std::cout << "Start position out of board range.\n";
        return false;
    }
    char piece = board.board_at(start);
    if (is_white && !(piece == 'P' || piece == 'R' || piece == 'K' || piece == 'Q'
        || piece == 'N' || piece == 'B')) {
        std::cout << "No movable piece at start position.\n";
        return false;
    }
    if (!is_white && !(piece == 'p' || piece == 'r' || piece == 'k' || piece == 'q'
        || piece == 'n' || piece == 'b')) {
        std::cout << "No movable piece at start position.\n";
        return false;
    }
    Position end{ (int)(move[3] - 'A'), (int)(move[4] - '1') };
    if (end.row < 'A' || end.row > 'H' || end.column < 1 || end.column > 8) {
        std::cout << "End position out of board range.\n";
        return false;
    }
    if (!board.piece_movement(piece, start, end)) {
        return false;
    }
    return true;
}

void Player::make_turn(Board &board) {
    std::string move;
    std::cout << name << ", please enter your move: ";
    std::cin >> move;
    while (!valid_move(move, board)) {
        std::cout << name << ", please enter your move: ";
        std::cin >> move;
    }
    // perform move
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

    /*while (!player1.in_checkmate() && !player2.in_checkmate()) {
        player1.make_turn(board);
        player2.make_turn(board);
    }*/

    return 0;
}
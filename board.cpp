#include "board.h"
#include <iostream>
#include <vector>
#include <string>


Board::Board() {
    board.resize(8);
    std::vector<std::string> board_in{ "RNBKQBNR",
                                       "PPPPPPPP",
                                       "        ",
                                       "        ",
                                       "        ",
                                       "        ",
                                       "pppppppp",
                                       "rnbkqbnr" };
    board = board_in;
    std::vector<std::vector<bool>> check_in(8, std::vector<bool> (8, false));
    white_check = check_in;
    black_check = check_in;
}

void Board::print_line(int color1, int color2, int line_in) {
    int square = 6;
    std::cout << (char)(line_in + 'A') << " ";
    for (int line = 0; line < square / 2; line++) {
        // 4 pairs of black and white
        if (line != 0) {
            std::cout << "  ";
        }
        for (int pair = 0; pair < 4; pair++) {
            for (int col = 0; col < square; col++) {
                // piece should be in the "middle" of the cell
                if (line == 1 && col == 3) {
                    std::cout << char(board[line_in][pair * 2] != 0x20 ?
                        board[line_in][pair * 2] : color1);
                }
                else {
                    std::cout << char(color1);
                }
            }
            // second cell
            for (int col = 0; col < square; col++) {
                if (line == 1 && col == 3) {
                    std::cout << char(board[line_in][pair * 2 + 1] != 0x20 ?
                        board[line_in][pair * 2 + 1] : color2);
                }
                else {
                    std::cout << char(color2);
                }
            }
        }
        std::cout << "\n";
    }
}

void Board::print_board() {
    std::cout << "____________________________________________________\n";
    std::cout << "| Reminder! uppercase = white, lowercase = black   |\n";
    std::cout << "| Enter moves in the format start-end. Ex: A1-A2   |\n";
    std::cout << "----------------------------------------------------\n\n";
    for (int i = 0; i < 8; ++i) {
        // line starting with black
        if (i % 2 == 0) {
            print_line(WHITE_SQUARE, BLACK_SQUARE, i);
        }
        else {
            // Line starting with WHITE
            print_line(BLACK_SQUARE, WHITE_SQUARE, i);
        }
    }
    std::cout << "     1     2     3     4     5     6     7     8  \n";
}

char Board::board_at(Position &position) {
    return board[position.row][position.column];
}

bool Board::opponents_piece(char piece, int row, int column) {
    // player_num 1 is white
    if (piece == 'P' || piece == 'Q' || piece == 'K' || piece == 'B' ||
        piece == 'N' || piece == 'R') {
        if (board[row][column] == 'p' || board[row][column] == 'q' ||
            board[row][column] == 'k' || board[row][column] == 'b' ||
            board[row][column] == 'n' || board[row][column] == 'r') {
            return true;
        }
        return false;
    }
    // player_num 2 is black
    else {
        if (board[row][column] == 'P' || board[row][column] == 'Q' ||
            board[row][column] == 'K' || board[row][column] == 'B' ||
            board[row][column] == 'N' || board[row][column] == 'R') {
            return true;
        }
        return false;
    }
}

bool Board::piece_movement(char piece, Position &start, Position &end) {
    // Pawn on bottom
    if (piece == 'p') {
        if (board[end.row][end.column] == '_' &&
            ((start.row == 2 && end.row == 4) || (end.row == start.row + 1))) {
            return true;
        }
        else if ((end.row == start.row + 1) && (end.column == start.column + 1 ||
            end.column == start.column - 1) 
            && opponents_piece(piece, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    // Pawn on top
    else if (piece == 'P') {
        if (board[end.row][end.column] == '_' 
            && ((start.row == 7 && end.row == 5) || (end.row == start.row - 1))) {
            return true;
        }
        else if ((end.row == start.row - 1) && (end.column == start.column + 1 ||
            end.column == start.column - 1)
            && opponents_piece(piece, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (piece == 'b' || piece == 'B') {
        if (board[end.row][end.column] == '_'
            && (abs(end.row - start.row) == abs(end.column - start.column))) {
            return true;
        }
        else if (abs(end.row - start.row) == abs(end.column - start.column) &&
            opponents_piece(piece, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (piece == 'n' || piece == 'N') {
        if (board[end.row][end.column] == '_'
            && ((abs(end.row - start.row) == 2 && abs(end.column - start.column == 1))
                || (abs(end.row - start.row) == 1 && abs(end.column - start.column == 2)))) {
            return true;
        }
        else if (((abs(end.row - start.row) == 2 && abs(end.column - start.column == 1))
            || (abs(end.row - start.row) == 1 && abs(end.column - start.column == 2))) &&
            opponents_piece(piece, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (piece == 'r' || piece == 'R') {
        if (board[end.row][end.column] == '_'
            && (end.row == start.row || end.column == start.column)) {
            return true;
        }
        else if ((end.row == start.row || end.column == start.column) &&
            opponents_piece(piece, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (piece == 'q' || piece == 'Q') {
        if (board[end.row][end.column] == '_'
            && (end.row == start.row || end.column == start.column 
                || (abs(end.row - start.row) == abs(end.column - start.column)))) {
            return true;
        }
        else if ((end.row == start.row || end.column == start.column || 
            (abs(end.row - start.row) == abs(end.column - start.column))) &&
            opponents_piece(piece, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (piece == 'k') {
        if (board[end.row][end.column] == '_'
            && (abs(end.row - start.row) == 1 || abs(end.column - start.column) == 1)
            && black_check[end.column][end.row] == false) {
            return true;
        }
        else if ((abs(end.row - start.row) == 1 || abs(end.column - start.column) == 1) &&
            opponents_piece(piece, end.row, end.column)
            && black_check[end.column][end.row] == false) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (piece == 'K') {
        if (board[end.row][end.column] == '_'
            && (abs(end.row - start.row) == 1 || abs(end.column - start.column) == 1)
            && white_check[end.column][end.row] == false) {
            return true;
        }
        else if ((abs(end.row - start.row) == 1 || abs(end.column - start.column) == 1) &&
            opponents_piece(piece, end.row, end.column) 
            && white_check[end.column][end.row] == false) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}
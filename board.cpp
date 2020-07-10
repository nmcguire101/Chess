#include "board.h"
#include <iostream>
#include <vector>
#include <string>

/* Board vector layout (white pieces on top, black pieces on bottom)
              _______________
           7 |R N B K Q B N R|
           6 |P P P P P P P P|
           5 |               |
           4 |               |
           3 |               |
           2 |               |
           1 |p p p p p p p p|
           0 |r n b k q b n r|
             -----------------
              0 1 2 3 4 5 6 7

*/
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

// Prints one line of the chess board
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

// Prints chess board by repeatedly calling print_line
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

// Returns char at position specified
char Board::board_at(Position &position) {
    return board[position.row][position.column];
}

// Determines if piece specified is the opponent's piece
// Only use function if you know there's a piece in that location
bool Board::opponents_piece(bool is_white, int row, int column) {
    // player_num 1 is white
    if (is_white) {
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

// Checks for valid pawn movement
bool Board::pawn_movement(bool is_white, Position &start, Position &end) {
    // If pawn is in start position it can move one or two rows forward
    if (is_white) {
        // Moving straight forward
        if (board[end.row][end.column] == ' ' && end.column == start.column &&
            ((start.row == 6 && end.row == 4) || (end.row == start.row - 1))) {
            return true;
        }
        // Moving diagonal to capture opponent's piece
        else if ((end.row == start.row - 1) && (end.column == start.column + 1 ||
            end.column == start.column - 1)
            && opponents_piece(true, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
    // black
    else {
        if (board[end.row][end.column] == ' ' && end.column == start.column &&
            ((start.row == 1 && end.row == 3) || (end.row == start.row + 1))) {
            return true;
        }
        else if ((end.row == start.row + 1) && (end.column == start.column + 1 ||
            end.column == start.column - 1)
            && opponents_piece(false, end.row, end.column)) {
            return true;
        }
        else {
            return false;
        }
    }
}

// Checks for valid knight movement
bool Board::knight_movement(bool is_white, Position &start, Position &end) {
    // Empty space
    if (board[end.row][end.column] == ' ' && ((abs(end.row - start.row) == 2 && 
        abs(end.column - start.column == 1)) || (abs(end.row - start.row) == 1 && 
            abs(end.column - start.column == 2)))) {
        return true;
    }
    // Capture opponent's piece
    else if (((abs(end.row - start.row) == 2 && abs(end.column - start.column == 1))
        || (abs(end.row - start.row) == 1 && abs(end.column - start.column == 2))) &&
        opponents_piece(is_white, end.row, end.column)) {
        return true;
    }
    else {
        return false;
    }
}

// Checks for valid rook movement
bool Board::rook_movement(bool is_white, Position &start, Position &end) {
    // Moving to empty space or capturing opponent's piece
    if (board[end.row][end.column] == ' ' || 
        opponents_piece(is_white, end.row, end.column)) {
        if (end.row == start.row && end.column != start.column) {
            // make sure piece is not in the way
            if (end.column > start.column) {
                for (int i = start.column; i < end.row; ++i) {
                    if (board[start.row][i] != ' ') {
                        return false;
                    }
                }
                return true;
            }
            else {
                for (int i = end.row; i < start.row; ++i) {
                    if (board[start.row][i] != ' ') {
                        return false;
                    }
                }
                return true;
            }
        }
        else if (end.column == start.column && end.row != start.row) {
            if (end.row > start.row) {
                for (int i = start.row; i < end.row; ++i) {
                    if (board[i][start.column] != ' ') {
                        return false;
                    }
                }
                return true;
            }
            else {
                for (int i = end.row; i < start.row; ++i) {
                    if (board[i][start.column] != ' ') {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }
    return false;
}

// Checks for valid bishop movement
bool Board::bishop_movement(bool is_white, Position &start, Position &end) {
    // Moving to empty space
    if ((board[end.row][end.column] == ' ' || 
        opponents_piece(is_white, end.row, end.column)) &&
        (abs(end.row - start.row) == abs(end.column - start.column))) {
        // moving down and left
        if (end.row < start.row && end.column < start.column) {
            // make sure piece is not in the way
            for (int i = 1; i < abs(end.row - end.column) - 1; ++i) {
                if (board[start.row - i][start.column - i] != ' ') {
                    return false;
                }
            }
            return true;
        }
        // moving down and right
        else if (end.row < start.row && end.column > start.column) {
            for (int i = 1; i < abs(end.row - end.column) - 1; ++i) {
                if (board[start.row - i][start.column + i] != ' ') {
                    return false;
                }
            }
            return true;
        }
        // moving up and right
        else if (end.row > start.row && end.column > start.column) {
            for (int i = 1; i < abs(end.row - end.column) - 1; ++i) {
                if (board[start.row + i][start.column + i] != ' ') {
                    return false;
                }
            }
            return true;
        }
        // moving up and left
        else if (end.row > start.row && end.column < start.column) {
            for (int i = 1; i < abs(end.row - end.column) - 1; ++i) {
                if (board[start.row + i][start.column - i] != ' ') {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    return false;
}

// Checks for valid queen movement
bool Board::queen_movement(bool is_white, Position &start, Position &end) {
    // Moving in straight line
    if (end.row == start.row || end.column == start.column) {
        if (rook_movement(is_white, start, end)) {
            return true;
        }
        return false;
    }
    // Moving diagonally
    else if (abs(end.row - start.row) == abs(end.column - start.column)) {
        if (bishop_movement(is_white, start, end)) {
            return true;
        }
        return false;
    }
    return false;
}

// Checks for valid king movement
bool Board::king_movement(bool is_white, Position &start, Position &end) {
    if ((board[end.row][end.column] == ' '  || 
        opponents_piece(is_white, end.row, end.column))
        && (abs(end.row - start.row) == 1 || abs(end.column - start.column) == 1)) {
        return true;
    }
    return false;
}

// Updates vector given
void Board::update_check(bool is_white, std::vector<std::vector<bool>> &check) {
    std::vector<std::vector<bool>> check_in(8, std::vector<bool>(8, false));
    check = check_in;
    int lowercase = 0;
    if (!is_white) {
        lowercase += 20;
    }
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (is_white && board[i][j] == 'P') {
                if (i != 0) {
                    if (j + 1 < 9) {
                        board[i - 1][j + 1] = true;
                    }
                    if (j - 1 >= 0) {
                        board[i - 1][j - 1] = true;
                    }
                }
            }
            if (!is_white && board[i][j] == 'p') {
                if (i != 8) {
                    if (j + 1 < 9) {
                        board[i + 1][j + 1] = true;
                    }
                    if (j - 1 >= 0) {
                        board[i + 1][j - 1] = true;
                    }
                }
            }
            else if (board[i][j] == 'R' + lowercase) {
                // above
                int k = i + 1; 
                while (k <= 8 && board[k][j] == ' ') {
                    check[k][j] = true;
                    ++k;
                }
                // below
                k = i - 1;
                while (k >= 0 && board[k][j] == ' ') {
                    check[k][j] = true;
                    --k;
                }
                // right
                k = j + 1;
                while (k <= 8 && board[i][k] == ' ') {
                    check[i][k] = true;
                    ++k;
                }
                // left
                k = j - 1;
                while (k >= 0 && board[i][k] == ' ') {
                    check[i][k] = true;
                    --k;
                }
            }
            /*else if (board[i][j] == 'B' + lowercase) {
                while (board[k][l] == ' ')
            }
            else if (board[i][j] == 'Q' + lowercase) {

            }
            else if (board[i][j] == 'K' + lowercase) {

            }
            else if (board[i][j] == 'P' + lowercase) {

            }
            else if (board[i][j] == ' ') {
                
            }*/
            else {
                check[i][j] = true;
            }
        }
    }
}

void Board::perform_move(bool is_white, char piece, Position &start, Position &end) {
    // moves piece on board
    board[start.row][end.column] = ' ';
    board[end.row][end.column] = piece;
    // updates black check vector for other player
    if (is_white) {
        update_check(false, black_check);
    }
    // updates white check vector
    else {
        update_check(true, white_check);
    }
}
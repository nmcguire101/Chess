#include "board.h"
#include <iostream>
#include <vector>
#include <string>

/* Board vector layout (white pieces on top, black pieces on bottom)
              _______________
           0 |R N B K Q B N R|
           1 |P P P P P P P P|
           2 |               |
           3 |               |
           4 |               |
           5 |               |
           6 |p p p p p p p p|
           7 |r n b k q b n r|
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
        if (line == 0) {
            std::cout << " " << (char)(line_in + 'A') << " ";
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
    std::cout << "     1     2     3     4     5     6     7     8  \n";
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

// Performs move and then undoes it, but returns true if it takes K out of check
bool Board::do_move(Piece &piece, int end_row, int end_col, 
    std::vector<Piece> &opponents) {
    std::vector<std::string> temp_board = board;
    temp_board[piece.pos.row][piece.pos.column] = ' ';
    temp_board[end_row][end_col] = piece.letter;
    Position temp_king{ end_row, end_col };
    if (!in_check_test(temp_king, temp_board, opponents)) {
        return true;
    }
    return false;
}

std::vector<Position> & Board::pawn_movement(bool in_check, Piece &piece,
    std::vector<Piece> &opponents) {
    std::vector<Position> movements;
    int temp = 1;
    if (!piece.is_white) {
        temp = -1;
    }
    // if in starting row, can move one or two spaces
    if (board[piece.pos.row + temp][piece.pos.column] == ' ' && piece.pos.row + temp >= 0
        && piece.pos.row + temp <= 7) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row + temp,
            piece.pos.column, opponents))) {
            Position one{ piece.pos.row + temp, piece.pos.column };
            movements.push_back(one);
        }
        if ((piece.pos.row == 1 && piece.is_white) || (piece.pos.row == 6 && !piece.is_white)
            && board[piece.pos.row + (2 * temp)][piece.pos.column] == ' ' && (!in_check || 
            (in_check && do_move(piece, piece.pos.row + (2 * temp), piece.pos.column, opponents)))) {
            Position two{ piece.pos.row + (2 * temp), piece.pos.column };
            movements.push_back(two);
        }
    }
    // else can take piece on right or left diagonal
    if ((piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7 && piece.pos.column + 1 <= 7
        && opponents_piece(piece.is_white, piece.pos.row + temp, piece.pos.column + 1)) &&
        (!in_check || (in_check && do_move(piece, piece.pos.row + temp, 
            piece.pos.column + 1, opponents)))) {
        Position three{ piece.pos.row + temp, piece.pos.column + 1 };
        movements.push_back(three);
    }
    if ((piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7 && piece.pos.column - 1 >= 0
        && opponents_piece(piece.is_white, piece.pos.row + temp, piece.pos.column - 1)) &&
        (!in_check || (in_check && do_move(piece, piece.pos.row + temp,
            piece.pos.column - 1, opponents)))) {
        Position four{ piece.pos.row + temp, piece.pos.column - 1 };
        movements.push_back(four);
    }
    return movements;
}

std::vector<Position> & Board::knight_movement(bool in_check, Piece &piece,
    std::vector<Piece> &opponents) {
    std::vector<Position> movements;
    if (piece.pos.row + 1 <= 7 && piece.pos.column + 2 <= 7
        && (board[piece.pos.row + 1][piece.pos.column + 2] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row + 1, piece.pos.column + 2)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row + 1,
            piece.pos.column + 2, opponents))) {
            Position temp{ piece.pos.row + 1, piece.pos.column + 2 };
            movements.push_back(temp);
        }
    }
    if (piece.pos.row + 1 <= 7 && piece.pos.column - 2 <= 7
        && (board[piece.pos.row + 1][piece.pos.column - 2] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row + 1, piece.pos.column - 2)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row + 1,
            piece.pos.column - 2, opponents))) {
            Position temp{ piece.pos.row + 1, piece.pos.column - 2 };
            movements.push_back(temp);
        }
    }
    if (piece.pos.row - 1 <= 7 && piece.pos.column + 2 <= 7
        && (board[piece.pos.row - 1][piece.pos.column + 2] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row - 1, piece.pos.column + 2)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row - 1,
            piece.pos.column + 2, opponents))) {
            Position temp{ piece.pos.row - 1, piece.pos.column + 2 };
            movements.push_back(temp);
        }
    }
    if (piece.pos.row - 1 <= 7 && piece.pos.column - 2 <= 7
        && (board[piece.pos.row - 1][piece.pos.column - 2] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row - 1, piece.pos.column - 2)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row - 1,
            piece.pos.column - 2, opponents))) {
            Position temp{ piece.pos.row - 1, piece.pos.column - 2 };
            movements.push_back(temp);
        }
    }
    if (piece.pos.row + 2 <= 7 && piece.pos.column + 1 <= 7
        && (board[piece.pos.row + 2][piece.pos.column + 1] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row + 2, piece.pos.column + 1)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row + 2,
            piece.pos.column + 1, opponents))) {
            Position temp{ piece.pos.row + 2, piece.pos.column + 1 };
            movements.push_back(temp);
        }
    }
    if (piece.pos.row + 2 <= 7 && piece.pos.column - 1 <= 7
        && (board[piece.pos.row + 2][piece.pos.column - 1] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row + 2, piece.pos.column - 1)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row + 2,
            piece.pos.column - 1, opponents))) {
        Position temp{ piece.pos.row + 2, piece.pos.column - 1 };
        movements.push_back(temp);
        }
    }
    if (piece.pos.row - 2 <= 7 && piece.pos.column + 1 <= 7
        && (board[piece.pos.row - 2][piece.pos.column + 1] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row - 2, piece.pos.column + 1)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row - 2,
            piece.pos.column + 1, opponents))) {
            Position temp{ piece.pos.row - 2, piece.pos.column + 1 };
            movements.push_back(temp);
        }
    }
    if (piece.pos.row - 2 <= 7 && piece.pos.column - 1 <= 7
        && (board[piece.pos.row - 2][piece.pos.column - 1] == ' ') ||
        opponents_piece(piece.is_white, piece.pos.row - 2, piece.pos.column - 1)) {
        if (!in_check || (in_check && do_move(piece, piece.pos.row - 2,
            piece.pos.column - 1, opponents))) {
            Position temp{ piece.pos.row - 2, piece.pos.column - 1 };
            movements.push_back(temp);
        }
    }
    return movements;
}

std::vector<Position> & Board::rook_movement(bool in_check, Piece &piece,
    std::vector<Piece> &opponents) {
    std::vector<Position> movements;
    // up
    for (int i = piece.pos.row; i >= 0; --i) {
        if (board[i][piece.pos.column] == ' ') {
            if (!in_check || (in_check && do_move(piece, i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, piece.pos.column)) {
            if (!in_check || (in_check && do_move(piece, i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    // down
    for (int i = piece.pos.row; i <= 7; ++i) {
        if (board[i][piece.pos.column] == ' ') {
            if (!in_check || (in_check && do_move(piece, i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, piece.pos.column)) {
            if (!in_check || (in_check && do_move(piece, i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    // left
    for (int i = piece.pos.column; i >= 0; --i) {
        if (board[piece.pos.row][i] == ' ') {
            if (!in_check || (in_check && do_move(piece, piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, piece.pos.row, i)) {
            if (!in_check || (in_check && do_move(piece, piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    // right
    for (int i = piece.pos.column; i <= 7; ++i) {
        if (board[piece.pos.row][i] == ' ') {
            if (!in_check || (in_check && do_move(piece, piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, piece.pos.row, i)) {
            if (!in_check || (in_check && do_move(piece, piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    return movements;
}

std::vector<Position> & Board::bishop_movement(bool in_check, Piece &piece,
    std::vector<Piece> &opponents) {
    std::vector<Position> movements;
    // up-left
    int i = piece.pos.row - 1;
    int j = piece.pos.column - 1;
    while (i >= 0 && j >= 0) {
        if (board[i][j] == ' ') {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
        --i;
        --j;
    }
    // up-right
    i = piece.pos.row - 1;
    j = piece.pos.column + 1;
    while (i >= 0 && j <= 7) {
        if (board[i][j] == ' ') {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
        --i;
        ++j;
    }
    // down-left
    i = piece.pos.row + 1;
    j = piece.pos.column - 1;
    while (i <= 7 && j >= 0) {
        if (board[i][j] == ' ') {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
        ++i;
        --j;
    }
    // down-right
    i = piece.pos.row + 1;
    j = piece.pos.column + 1;
    while (i <= 7 && j <= 7) {
        if (board[i][j] == ' ') {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, i, j, opponents))) {
                Position temp{ i, j };
                movements.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
        ++i;
        ++j;
    }
    return movements;
}

std::vector<Position> & Board::queen_movement(bool in_check, Piece &piece,
    std::vector<Piece> &opponents) {
    std::vector<Position> temp = rook_movement(in_check, piece, opponents);
    std::vector<Position> movements(temp);
    std::vector<Position> temp2 = bishop_movement(in_check, piece, opponents);
    movements.insert(movements.end(), temp2.begin(), temp2.end());
    return movements;
}

std::vector<Position> & Board::king_movement(bool in_check, Piece &piece,
    std::vector<Piece> &opponents) {
    std::vector<Position> movements;
    // down
    int i = piece.pos.row + 1;
    if (i <= 7 && (opponents_piece(piece.is_white, i, piece.pos.column)
        || board[i][piece.pos.column] == ' ')) {
        // make sure piece is not moving into check
        if (do_move(piece, i, piece.pos.column, opponents)) {
            Position temp{ i, piece.pos.column };
            movements.push_back(temp);
        }
    }
    // up
    i = piece.pos.row - 1;
    if (i >= 0 && (opponents_piece(piece.is_white, i, piece.pos.column)
        || board[i][piece.pos.column] == ' ')) {
        if (do_move(piece, i, piece.pos.column, opponents)) {
            Position temp{ i, piece.pos.column };
            movements.push_back(temp);
        }
    }
    // right
    i = piece.pos.column + 1;
    if (i <= 7 && (opponents_piece(piece.is_white, piece.pos.row, i)
        || board[piece.pos.row][i] == ' ')) {
        if (do_move(piece, piece.pos.row, i, opponents)) {
            Position temp{ piece.pos.row, i };
            movements.push_back(temp);
        }
    }
    // left
    i = piece.pos.column - 1;
    if (i >= 7 && (opponents_piece(piece.is_white, piece.pos.row, i)
        || board[piece.pos.row][i] == ' ')) {
        if (do_move(piece, piece.pos.row, i, opponents)) {
            Position temp{ piece.pos.row, i };
            movements.push_back(temp);
        }
    }
    return movements;
}

// Returns true if king is in check on this board
bool Board::in_check_test(Position &king, std::vector<std::string> &temp_board,
    std::vector<Piece> &opponents) {
    Compare_position compare;
    for (int i = 0; i < (int)(opponents.size()); ++i) {
        for (int j = 0; j < (int)(opponents[i].valid_moves.size()); ++j) {
            if (compare(king, opponents[i].valid_moves[j])) {
                return false;
            }
        }
    }
    return true;
}

bool Board::in_check(Position &king, std::vector<Piece> &opponents) {
    Compare_position compare;
    for (int i = 0; i < (int)(opponents.size()); ++i) {
        for (int j = 0; j < (int)(opponents[i].valid_moves.size()); ++j) {
            if (compare(king, opponents[i].valid_moves[j])) {
                return false;
            }
        }
    }
    return true;
}

// Performs move
void Board::perform_move(Piece &piece, Position &end) {
    // moves piece on board
    board[piece.pos.row][piece.pos.column] = ' ';
    board[end.row][end.column] = piece.letter;
    print_board();
}
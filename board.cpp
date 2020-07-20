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
bool Board::do_move(Piece &piece, Position &king, int end_row, int end_col, 
    std::vector<Piece> &opponents) {
    std::vector<std::string> temp_board = board;
    temp_board[piece.pos.row][piece.pos.column] = ' ';
    temp_board[end_row][end_col] = piece.letter;
    Compare_position compare;
    if (compare(piece.pos, king)) {
        Position temp_king{ end_row, end_col };
        king = temp_king;
    }
    if (in_check_test(king, temp_board, opponents)) {
        return false;
    }
    return true;
}

void Board::pawn_movement(bool in_check, Position &king, Piece &piece,
    std::vector<Piece> &opponents, std::vector<Position> &pieces) {
    pieces.clear();
    int temp = 1;
    if (!piece.is_white) {
        temp = -1;
    }
    // if in starting row, can move one or two spaces
    if (board[piece.pos.row + temp][piece.pos.column] == ' ' && piece.pos.row + temp >= 0
        && piece.pos.row + temp <= 7) {
        if (!in_check || (in_check && do_move(piece, king, piece.pos.row + temp,
            piece.pos.column, opponents))) {
            Position one{ piece.pos.row + temp, piece.pos.column };
            pieces.push_back(one);
        }
        if ((piece.pos.row == 1 && piece.is_white) || (piece.pos.row == 6 && !piece.is_white)
            && board[piece.pos.row + (2 * temp)][piece.pos.column] == ' ' && (!in_check || 
            (in_check && do_move(piece, king, piece.pos.row + (2 * temp), piece.pos.column, opponents)))) {
            Position two{ piece.pos.row + (2 * temp), piece.pos.column };
            pieces.push_back(two);
        }
    }
    // else can take piece on right or left diagonal
    if ((piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7 && piece.pos.column + 1 <= 7
        && opponents_piece(piece.is_white, piece.pos.row + temp, piece.pos.column + 1)) &&
        (!in_check || (in_check && do_move(piece, king, piece.pos.row + temp, 
            piece.pos.column + 1, opponents)))) {
        Position three{ piece.pos.row + temp, piece.pos.column + 1 };
        pieces.push_back(three);
    }
    if ((piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7 && piece.pos.column - 1 >= 0
        && opponents_piece(piece.is_white, piece.pos.row + temp, piece.pos.column - 1)) &&
        (!in_check || (in_check && do_move(piece, king, piece.pos.row + temp,
            piece.pos.column - 1, opponents)))) {
        Position four{ piece.pos.row + temp, piece.pos.column - 1 };
        pieces.push_back(four);
    }
}

void Board::knight_movement(bool in_check, Position &king, Piece &piece,
    std::vector<Piece> &opponents, std::vector<Position> &pieces) {
    pieces.clear();
    if (piece.pos.row + 1 <= 7 && piece.pos.column + 2 <= 7) {
        if ((board[piece.pos.row + 1][piece.pos.column + 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 1, piece.pos.column + 2)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row + 1,
                piece.pos.column + 2, opponents))) {
                Position temp{ piece.pos.row + 1, piece.pos.column + 2 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row + 1 <= 7 && piece.pos.column - 2 >= 0) {
        if ((board[piece.pos.row + 1][piece.pos.column - 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 1, piece.pos.column - 2)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row + 1,
                piece.pos.column - 2, opponents))) {
                Position temp{ piece.pos.row + 1, piece.pos.column - 2 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row - 1 >= 0 && piece.pos.column + 2 <= 7) {
        if ((board[piece.pos.row - 1][piece.pos.column + 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 1, piece.pos.column + 2)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row - 1,
                piece.pos.column + 2, opponents))) {
                Position temp{ piece.pos.row - 1, piece.pos.column + 2 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row - 1 >= 0 && piece.pos.column - 2 >= 0) {
        if ((board[piece.pos.row - 1][piece.pos.column - 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 1, piece.pos.column - 2)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row - 1,
                piece.pos.column - 2, opponents))) {
                Position temp{ piece.pos.row - 1, piece.pos.column - 2 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row + 2 <= 7 && piece.pos.column + 1 <= 7) {
        if ((board[piece.pos.row + 2][piece.pos.column + 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 2, piece.pos.column + 1)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row + 2,
                piece.pos.column + 1, opponents))) {
                Position temp{ piece.pos.row + 2, piece.pos.column + 1 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row + 2 <= 7 && piece.pos.column - 1 >= 0) {
        if ((board[piece.pos.row + 2][piece.pos.column - 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 2, piece.pos.column - 1)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row + 2,
                piece.pos.column - 1, opponents))) {
                Position temp{ piece.pos.row + 2, piece.pos.column - 1 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row - 2 >= 0 && piece.pos.column + 1 <= 7) {
        if ((board[piece.pos.row - 2][piece.pos.column + 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 2, piece.pos.column + 1)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row - 2,
                piece.pos.column + 1, opponents))) {
                Position temp{ piece.pos.row - 2, piece.pos.column + 1 };
                pieces.push_back(temp);
            }
        }
    }
    if (piece.pos.row - 2 >= 0 && piece.pos.column - 1 >= 0) {
        if ((board[piece.pos.row - 2][piece.pos.column - 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 2, piece.pos.column - 1)) {
            if (!in_check || (in_check && do_move(piece, king, piece.pos.row - 2,
                piece.pos.column - 1, opponents))) {
                Position temp{ piece.pos.row - 2, piece.pos.column - 1 };
                pieces.push_back(temp);
            }
        }
    }
}

void Board::rook_movement(bool in_check, Position &king, Piece &piece,
    std::vector<Piece> &opponents, std::vector<Position> &pieces) {
    pieces.clear();
    // up
    for (int i = piece.pos.row - 1; i >= 0; --i) {
        if (board[i][piece.pos.column] == ' ') {
            if (!in_check || (in_check && do_move(piece, king, 
                i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, piece.pos.column)) {
            if (!in_check || (in_check && do_move(piece, king, 
                i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                pieces.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    // down
    for (int i = piece.pos.row + 1; i <= 7; ++i) {
        if (board[i][piece.pos.column] == ' ') {
            if (!in_check || (in_check && do_move(piece, king, 
                i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, piece.pos.column)) {
            if (!in_check || (in_check && do_move(piece, king, 
                i, piece.pos.column, opponents))) {
                Position temp{ i, piece.pos.column };
                pieces.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    // left
    for (int i = piece.pos.column - 1; i >= 0; --i) {
        if (board[piece.pos.row][i] == ' ') {
            if (!in_check || (in_check && do_move(piece, king, 
                piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, piece.pos.row, i)) {
            if (!in_check || (in_check && do_move(piece, king,
                piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                pieces.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
    // right
    for (int i = piece.pos.column + 1; i <= 7; ++i) {
        if (board[piece.pos.row][i] == ' ') {
            if (!in_check || (in_check && do_move(piece, king, 
                piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, piece.pos.row, i)) {
            if (!in_check || (in_check && do_move(piece, king, 
                piece.pos.row, i, opponents))) {
                Position temp{ piece.pos.row, i };
                pieces.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
    }
}

void Board::bishop_movement(bool queen, bool in_check, Position &king, 
    Piece &piece, std::vector<Piece> &opponents, std::vector<Position> &pieces) {
    if (!queen) {
        pieces.clear();
    }
    // up-left
    int i = piece.pos.row - 1;
    int j = piece.pos.column - 1;
    while (i >= 0 && j >= 0) {
        if (board[i][j] == ' ') {
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
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
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
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
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
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
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
            }
        }
        else if (opponents_piece(piece.is_white, i, j)) {
            if (!in_check || (in_check && do_move(piece, king, i, j, opponents))) {
                Position temp{ i, j };
                pieces.push_back(temp);
            }
            break;
        }
        else {
            break;
        }
        ++i;
        ++j;
    }
}

void Board::queen_movement(bool in_check, Position &king, Piece &piece,
    std::vector<Piece> &opponents, std::vector<Position> &pieces) {
    rook_movement(in_check, king, piece, opponents, pieces);
    bishop_movement(true, in_check, king, piece, opponents, pieces);
}

void Board::king_movement(bool in_check, Position &king, Piece &piece,
    std::vector<Piece> &opponents, std::vector<Position> &pieces) {
    pieces.clear();
    // down
    int i = piece.pos.row + 1;
    if (i <= 7 && (opponents_piece(piece.is_white, i, piece.pos.column)
        || board[i][piece.pos.column] == ' ')) {
        // make sure piece is not moving into check
        if (do_move(piece, king, i, piece.pos.column, opponents)) {
            Position temp{ i, piece.pos.column };
            pieces.push_back(temp);
        }
    }
    // down-right
    i = piece.pos.row + 1;
    int j = piece.pos.column + 1;
    if (i <= 7 && j <= 7 && (opponents_piece(piece.is_white, i, j)
        || board[i][j] == ' ')) {
        // make sure piece is not moving into check
        if (do_move(piece, king, i, j, opponents)) {
            Position temp{ i, j };
            pieces.push_back(temp);
        }
    }
    // down-left
    i = piece.pos.row + 1;
    j = piece.pos.column - 1;
    if (i <= 7 && j >= 0 && (opponents_piece(piece.is_white, i, j)
        || board[i][j] == ' ')) {
        // make sure piece is not moving into check
        if (do_move(piece, king, i, j, opponents)) {
            Position temp{ i, j };
            pieces.push_back(temp);
        }
    }
    // up
    i = piece.pos.row - 1;
    if (i >= 0 && (opponents_piece(piece.is_white, i, piece.pos.column)
        || board[i][piece.pos.column] == ' ')) {
        if (do_move(piece, king, i, piece.pos.column, opponents)) {
            Position temp{ i, piece.pos.column };
            pieces.push_back(temp);
        }
    }
    // up-right
    i = piece.pos.row - 1;
    j = piece.pos.column + 1;
    if (i >= 0 && j <= 7 && (opponents_piece(piece.is_white, i, j)
        || board[i][j] == ' ')) {
        // make sure piece is not moving into check
        if (do_move(piece, king, i, j, opponents)) {
            Position temp{ i, j };
            pieces.push_back(temp);
        }
    }
    // up-left
    i = piece.pos.row - 1;
    j = piece.pos.column - 1;
    if (i >= 0 && j >= 0 && (opponents_piece(piece.is_white, i, j)
        || board[i][j] == ' ')) {
        // make sure piece is not moving into check
        if (do_move(piece, king, i, j, opponents)) {
            Position temp{ i, j };
            pieces.push_back(temp);
        }
    }
    // right
    i = piece.pos.column + 1;
    if (i <= 7 && (opponents_piece(piece.is_white, piece.pos.row, i)
        || board[piece.pos.row][i] == ' ')) {
        if (do_move(piece, king, piece.pos.row, i, opponents)) {
            Position temp{ piece.pos.row, i };
            pieces.push_back(temp);
        }
    }
    // left
    i = piece.pos.column - 1;
    if (i >= 7 && (opponents_piece(piece.is_white, piece.pos.row, i)
        || board[piece.pos.row][i] == ' ')) {
        if (do_move(piece, king, piece.pos.row, i, opponents)) {
            Position temp{ piece.pos.row, i };
            pieces.push_back(temp);
        }
    }
}

// Returns true if king is in check on this board
bool Board::in_check_test(Position &king, std::vector<std::string> &temp_board,
    std::vector<Piece> &opponents) {
    Compare_position compare;
    for (int i = 0; i < (int)(opponents.size()); ++i) {
        for (int j = 0; j < (int)(opponents[i].valid_moves.size()); ++j) {
            if (compare(king, opponents[i].valid_moves[j])) {
                return true;
            }
        }
    }
    return false;
}

bool Board::in_check(Position &king, std::vector<Piece> &opponents) {
    Compare_position compare;
    for (int i = 0; i < (int)(opponents.size()); ++i) {
        for (int j = 0; j < (int)(opponents[i].valid_moves.size()); ++j) {
            if (compare(king, opponents[i].valid_moves[j])) {
                return true;
            }
        }
    }
    return false;
}

// Performs move
void Board::perform_move(Piece &piece, Position &end, 
    std::vector<Piece> &opponents) {
    // moves piece on board
    Compare_position compare;
    if (board[end.row][end.column] != ' ') {
        for (int i = 0; i < (int)(opponents.size()); ++i) {
            if (compare(end, opponents[i].pos)) {
                opponents.erase(opponents.begin() + i);
                break;
            }
        }
    }
    board[piece.pos.row][piece.pos.column] = ' ';
    board[end.row][end.column] = piece.letter;
    print_board();
}
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
bool Board::opponents_piece(bool is_white, int row, int col) {
    // player_num 1 is white
    if (is_white) {
        if (board[row][col] == 'p' || board[row][col] == 'q' ||
            board[row][col] == 'k' || board[row][col] == 'b' ||
            board[row][col] == 'n' || board[row][col] == 'r') {
            return true;
        }
        return false;
    }
    // player_num 2 is black
    else {
        if (board[row][col] == 'P' || board[row][col] == 'Q' ||
            board[row][col] == 'K' || board[row][col] == 'B' ||
            board[row][col] == 'N' || board[row][col] == 'R') {
            return true;
        }
        return false;
    }
}

// Performs move and then undoes it, but returns true if it takes K out of check
bool Board::do_move(Piece &piece, Position &king, int end_row, int end_col, 
    std::vector<Piece> &opponents) {
    std::vector<std::string> temp_board = board;
    std::vector <Piece> temp_opponents = opponents;
    Compare_position compare;
    // temporarily removes killed piece
    if (board[end_row][end_col] != ' ') {
        for (int i = 0; i < (int)(opponents.size()); ++i) {
            if (end_row == opponents[i].pos.row && end_col == opponents[i].pos.col) {
                opponents.erase(opponents.begin() + i);
                break;
            }
        }
    }
    board[piece.pos.row][piece.pos.col] = ' ';
    board[end_row][end_col] = piece.letter;
    if (compare(piece.pos, king)) {
        Position temp_king{ end_row, end_col };
        if (in_check(piece.is_white, temp_king, opponents)) {
            board = temp_board;
            opponents = temp_opponents;
            return false;
        }
    }
    else {
        if (in_check(piece.is_white, king, opponents)) {
            board = temp_board;
            opponents = temp_opponents;
            return false;
        }
    }
    board = temp_board;
    opponents = temp_opponents;
    return true;
}

// Returns true if there is a valid pawn movement
bool Board::pawn_movement(Position &king, Piece &piece, std::vector<Piece> &opponents) {
    int temp = 1;
    if (!piece.is_white) {
        temp = -1;
    }
    // checks if pawn can move one space forward & doesn't put K in check
    if (piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7
        && board[piece.pos.row + temp][piece.pos.col] == ' ') {
        if (do_move(piece, king, piece.pos.row + temp, piece.pos.col, opponents)) {
            return true;
        }
    }
    // checks if pawn is in starting row & can move two spaces forward
    if ((piece.pos.row == 1 && piece.is_white) || (piece.pos.row == 6 && !piece.is_white) 
        && board[piece.pos.row + (2 * temp)][piece.pos.col] == ' ') {
        if (do_move(piece, king, piece.pos.row + (2 * temp), piece.pos.col, opponents)) {
            return true;
        }
    }
    // checks if pawn can move diagonally
    if ((piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7 && piece.pos.col + 1 <= 7
        && opponents_piece(piece.is_white, piece.pos.row + temp, piece.pos.col + 1)) &&
        do_move(piece, king, piece.pos.row + temp, piece.pos.col + 1, opponents)) {
        return true;
    }
    if ((piece.pos.row + temp >= 0 && piece.pos.row + temp <= 7 && piece.pos.col - 1 >= 0
        && opponents_piece(piece.is_white, piece.pos.row + temp, piece.pos.col - 1)) &&
        do_move(piece, king, piece.pos.row + temp, piece.pos.col - 1, opponents)) {
        return true;
    }
    return false;
}

bool Board::knight_movement(Position &king, Piece &piece, std::vector<Piece> &opponents) {
    if (piece.pos.row + 1 <= 7) {
        if (piece.pos.col + 2 <= 7 && ((board[piece.pos.row + 1][piece.pos.col + 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 1, piece.pos.col + 2))) {
            if (do_move(piece, king, piece.pos.row + 1, piece.pos.col + 2, opponents)) {
                return true;
            }
        }
        if (piece.pos.col - 2 >= 0 && ((board[piece.pos.row + 1][piece.pos.col - 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 1, piece.pos.col - 2))) {
            if (do_move(piece, king, piece.pos.row + 1, piece.pos.col - 2, opponents)) {
                return true;
            }
        }
    }
    if (piece.pos.row - 1 >= 0) {
        if (piece.pos.col + 2 <= 7 && ((board[piece.pos.row - 1][piece.pos.col + 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 1, piece.pos.col + 2))) {
            if (do_move(piece, king, piece.pos.row - 1, piece.pos.col + 2, opponents)) {
                return true;
            }
        }
        if (piece.pos.col - 2 >= 0 ((board[piece.pos.row - 1][piece.pos.col - 2] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 1, piece.pos.col - 2))) {
            if (do_move(piece, king, piece.pos.row - 1, piece.pos.col - 2, opponents)) {
                return true;
            }
        }
    }
    if (piece.pos.row + 2 <= 7) {
        if (piece.pos.col + 1 <= 7 && ((board[piece.pos.row + 2][piece.pos.col + 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 2, piece.pos.col + 1))) {
            if (do_move(piece, king, piece.pos.row + 2, piece.pos.col + 1, opponents)) {
                return true;
            }
        }
        if (piece.pos.col - 1 >= 0 && ((board[piece.pos.row + 2][piece.pos.col - 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row + 2, piece.pos.col - 1))) {
            if (do_move(piece, king, piece.pos.row + 2, piece.pos.col - 1, opponents)) {
                return true;
            }
        }
    }
    if (piece.pos.row - 2 >= 0) {
        if (piece.pos.col + 1 <= 7 && ((board[piece.pos.row - 2][piece.pos.col + 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 2, piece.pos.col + 1))) {
            if (do_move(piece, king, piece.pos.row - 2, piece.pos.col + 1, opponents)) {
                return true;
            }
        }
        if (piece.pos.col - 1 >= 0 && ((board[piece.pos.row - 2][piece.pos.col - 1] == ' ') ||
            opponents_piece(piece.is_white, piece.pos.row - 2, piece.pos.col - 1))) {
            if (do_move(piece, king, piece.pos.row - 2, piece.pos.col - 1, opponents)) {
                return true;
            }
        }
    }
    return false;
}

// Helper function runs based on designated col:row movement
// Returns true if there is a valid movement
bool Board::move_forward(Piece &piece, int col, int row, Position &king, 
    std::vector<Piece> &opponents) {
    int i = piece.pos.row + row;
    int j = piece.pos.col + col;
    while (i <= 7 && j <= 7 && i >= 0 && j >= 0 && board[i][j] == ' ') {
        if (do_move(piece, king, i, j, opponents)) {
            return true;
        }
        i += row;
        j += col;
    }
    if (i <= 7 && j <= 7 && i >= 0 && j >= 0 && opponents_piece(piece.is_white, i, j)) {
        if (do_move(piece, king, i, j, opponents)) {
            return true;
        }
    }
    return false;
}

bool Board::rook_movement(Position &king, Piece &piece, std::vector<Piece> &opponents) {
    // up
    if (move_forward(piece, 0, -1, king, opponents)) {
        return true;
    }
    // down
    if (move_forward(piece, 0, 1, king, opponents)) {
        return true;
    }
    // left
    if (move_forward(piece, -1, 0, king, opponents)) {
        return true;
    }
    // right
    if (move_forward(piece, 1, 0, king, opponents)) {
        return true;
    }
    return false;
}

bool Board::bishop_movement(Position &king, Piece &piece, std::vector<Piece> &opponents) {
    // up-left
    if (move_forward(piece, -1, -1, king, opponents)) {
        return true;
    }
    // up-right
    if (move_forward(piece, -1, 1, king, opponents)) {
        return true;
    }
    // down-left
    if (move_forward(piece, 1, -1, king, opponents)) {
        return true;
    }
    // down-right
    if (move_forward(piece, 1, 1, king, opponents)) {
        return true;
    }
    return false;
}

bool Board::queen_movement(Position &king, Piece &piece, std::vector<Piece> &opponents) {
    if (rook_movement(king, piece, opponents)) {
        return true;
    }
    if (bishop_movement(king, piece, opponents)) {
        return true;
    }
    return false;
}

bool Board::king_movement(Position &king, Piece &piece, std::vector<Piece> &opponents) {
    int i = piece.pos.row;
    int j = piece.pos.col;
    if (i + 1 <= 7) {
        // down
        if (opponents_piece(piece.is_white, i + 1, j) || board[i + 1][j] == ' ') {
            if (do_move(piece, king, i + 1, j, opponents)) {
                return true;
            }
        }
        // down-right
        if (j + 1 <= 7 && (opponents_piece(piece.is_white, i + 1, j + 1) 
            || board[i + 1][j + 1] == ' ')) {
            if (do_move(piece, king, i + 1, j + 1, opponents)) {
                return true;
            }
        }
        // down-left
        if (j - 1 >= 0 && (opponents_piece(piece.is_white, i + 1, j - 1) 
            || board[i + 1][j - 1] == ' ')) {
            if (do_move(piece, king, i + 1, j - 1, opponents)) {
                return true;
            }
        }
    }
    if (i - 1 >= 0) {
        // up
        if (opponents_piece(piece.is_white, i - 1, j) || board[i - 1][j] == ' ') {
            if (do_move(piece, king, i - 1, j, opponents)) {
                return true;
            }
        }
        // up-right
        if (j + 1 <= 7 && (opponents_piece(piece.is_white, i - 1, j + 1)
            || board[i - 1][j + 1] == ' ')) {
            if (do_move(piece, king, i - 1, j + 1, opponents)) {
                return true;
            }
        }
        // up-left
        if (j - 1 >= 0 && (opponents_piece(piece.is_white, i - 1, j - 1)
            || board[i - 1][j - 1] == ' ')) {
            if (do_move(piece, king, i - 1, j - 1, opponents)) {
                return true;
            }
        }
    }
    // right
    if (j + 1 <= 7 && (opponents_piece(piece.is_white, i, j + 1)
        || board[i][j + 1] == ' ')) {
        if (do_move(piece, king, i, j + 1, opponents)) {
            return true;
        }
    }
    // left
    if (j - 1 >= 0 && (opponents_piece(piece.is_white, i, j - 1)
        || board[i][j - 1] == ' ')) {
        if (do_move(piece, king, i, j - 1, opponents)) {
            return true;
        }
    }
    return false;
}

// Helper function for in_check that runs based on designated col:row movement
// Returns true if it hits the king, returns false if it hits something else/edge of board
bool Board::ch_move_forward(Piece &piece, int col, int row, Position &king) {
    int i = piece.pos.row + row;;
    int j = piece.pos.col + col;
    while (i <= 7 && j <= 7 && i >= 0 && j >= 0 && board[i][j] == ' ') {
        i += row;
        j += col;
    }
    if (king.row == i && king.col == j) {
        return true;
    }
    return false;
}

// Returns true if given king is in check
bool Board::in_check(bool is_white, Position &king, std::vector<Piece> &opponents) {
    int temp = 1;
    if (!is_white) {
        temp = -1;
    }
    // Cycles through the opponent's pieces and sees if they're attacking the given king
    for (int i = 0; i < (int)(opponents.size()); ++i) {
        switch (opponents[i].name) {
         // king can't put other king in check
        case PieceName::King:
            break;

        case PieceName::Pawn:
            if (((king.row == opponents[i].pos.row + (1 * temp)) && (king.col
                == opponents[i].pos.col + 1)) || ((king.row == opponents[i].pos.row + (1 * temp))
                    && king.col == opponents[i].pos.col - 1)) {
                return true;
            }
            break;

        case PieceName::Knight:
            if ((king.row == opponents[i].pos.row + 2 && ((king.col == opponents[i].pos.col + 1) ||
                (king.col == opponents[i].pos.col - 1))) || (king.row == opponents[i].pos.row - 2 && 
                ((king.col == opponents[i].pos.col + 1) || (king.col == opponents[i].pos.col - 1))) || 
                (king.row == opponents[i].pos.row + 1 && ((king.col == opponents[i].pos.col + 2) ||
                (king.col == opponents[i].pos.col - 2))) || (king.row == opponents[i].pos.row - 1 && 
                ((king.col == opponents[i].pos.col + 2) || (king.col == opponents[i].pos.col - 2)))) {
                return true;
            }
            break;

        case PieceName::Rook:
            if (ch_move_forward(opponents[i], 1, 0, king) || ch_move_forward(opponents[i], -1, 0, king) ||
                ch_move_forward(opponents[i], 0, 1, king) || ch_move_forward(opponents[i], 0, -1, king)) {
                return true;
            }
            break;

        case PieceName::Bishop:
            if (ch_move_forward(opponents[i], 1, 1, king) || ch_move_forward(opponents[i], 1, -1, king) ||
                ch_move_forward(opponents[i], -1, 1, king) || ch_move_forward(opponents[i], -1, -1, king)) {
                return true;
            }
            break;

        case PieceName::Queen:
            if (ch_move_forward(opponents[i], 1, 0, king) || ch_move_forward(opponents[i], -1, 0, king) ||
                ch_move_forward(opponents[i], 0, 1, king) || ch_move_forward(opponents[i], 0, -1, king)) {
                return true;
            }
            if (ch_move_forward(opponents[i], 1, 1, king) || ch_move_forward(opponents[i], 1, -1, king) ||
                ch_move_forward(opponents[i], -1, 1, king) || ch_move_forward(opponents[i], -1, -1, king)) {
                return true;
            }
            break;

        }
    }
    return false;
}

// Tries to move each of player's pieces until it finds a valid move then returns true
// If it returns false, king is in checkmate or player is stalemated
bool Board::can_move(Position &king, std::vector<Piece> &pieces, 
    std::vector<Piece> &opponents) {
    /* to test a move:
       edit the moving piece in the opponents vector and on the board
       check if board is in check 
       undo move */
    for (int i = 0; i < (int)(pieces.size()); ++i) {
        switch (pieces[i].name) {
        case PieceName::Pawn:
            if (pawn_movement(king, pieces[i], opponents)) {
                return true;
            }
            break;

        case PieceName::Knight:
            if (knight_movement(king, pieces[i], opponents)) {
                return true;
            }
            break;

        case PieceName::Rook:
            if (rook_movement(king, pieces[i], opponents)) {
                return true;
            }
            break;

        case PieceName::Bishop:
            if (bishop_movement(king, pieces[i], opponents)) {
                return true;
            }
            break;

        case PieceName::Queen:
            if (rook_movement(king, pieces[i], opponents)) {
                return true;
            }
            if (bishop_movement(king, pieces[i], opponents)) {
                return true;
            }
            break;

        case PieceName::King:
            if (king_movement(king, pieces[i], opponents)) {
                return true;
            }
            break;
        }
    }
    return false;
}

// Helper function for valid_movement
bool Board::nothing_in_way(Piece &piece, int col, int row, int end_col, int end_row) {
    int i = piece.pos.row;
    int j = piece.pos.col;
    while (i >= 0 && i <= 7 && j >= 0 && j <= 7 && i < end_row && j < end_col) {
        if (board[i][j] != ' ') {
            return false;
        }
        i += row;
        j += col;
    }
    return true;
}

// Checks for valid movement
bool Board::valid_movement(PieceName name, Piece &piece, int end_row, int end_col) {
    // checks that end and start are not the same and space is empty or is an opponents piece
    if ((end_row == piece.pos.row && end_col == piece.pos.col) || (!(board[end_row][end_col] 
        == ' ' || opponents_piece(piece.is_white, end_row, end_col)))) {
        return false;
    }
    int temp = 1;
    int row = 6;
    if (piece.is_white) {
        temp = -1;
        row = 1;
    }
    switch (name) {
    case PieceName::Pawn:
        if (board[end_row][end_col] == ' ' && end_col == piece.pos.col && ((end_row + (1 * temp) == 
            piece.pos.row) || (end_row + (2 * temp) == piece.pos.row && piece.pos.row == row))) {
            return true;
        }
        if (opponents_piece(piece.is_white, end_row, end_col) && (end_row + (1 * temp) ==
            piece.pos.row) && (end_col + 1 == piece.pos.row || end_col - 1 == piece.pos.row)) {
            return true;
        }
        break;

    case PieceName::Knight:
        if ((end_row == piece.pos.row + 2 && ((end_col == piece.pos.col + 1) || (end_col == 
            piece.pos.col - 1))) || (end_row == piece.pos.row - 2 && ((end_col == piece.pos.col + 1) 
            || (end_col == piece.pos.col - 1))) || (end_row == piece.pos.row + 1 && ((end_col == 
            piece.pos.col + 2) || (end_col == piece.pos.col - 2))) || (end_row == piece.pos.row - 1 &&
            ((end_col == piece.pos.col + 2) || (end_col == piece.pos.col - 2)))) {
            return true;
        }
        break;

    case PieceName::Rook:
        if (end_col == piece.pos.col) {
            if ((end_row < piece.pos.row && nothing_in_way(piece, 0, -1, end_col, end_row)) ||
                (end_row > piece.pos.row && nothing_in_way(piece, 0, 1, end_col, end_row))) {
                return true;
            }
        }
        if (end_row == piece.pos.row) {
            if ((end_col < piece.pos.col && nothing_in_way(piece, -1, 0, end_col, end_row)) ||
                (end_col > piece.pos.col && nothing_in_way(piece, 1, 0, end_col, end_row))) {
                return true;
            }
        }
        break;

    case PieceName::Bishop:
        if (abs(end_row - piece.pos.row) == abs(end_col - piece.pos.col)) {
            if (end_row < piece.pos.row && end_col < piece.pos.col &&
                nothing_in_way(piece, -1, -1, end_col, end_row)){
                return true;
            }
            if (end_row < piece.pos.row && end_col > piece.pos.col &&
                nothing_in_way(piece, -1, 1, end_col, end_row)) {
                return true;
            }
            if (end_row > piece.pos.row && end_col < piece.pos.col &&
                nothing_in_way(piece, 1, -1, end_col, end_row)) {
                return true;
            }
            if (end_row > piece.pos.row && end_col > piece.pos.col &&
                nothing_in_way(piece, 1, 1, end_col, end_row)) {
                return true;
            }
        }
        break;

    case PieceName::Queen:
        if (end_col == piece.pos.col) {
            if ((end_row < piece.pos.row && nothing_in_way(piece, 0, -1, end_col, end_row)) ||
                (end_row > piece.pos.row && nothing_in_way(piece, 0, 1, end_col, end_row))) {
                return true;
            }
        }
        if (end_row == piece.pos.row) {
            if ((end_col < piece.pos.col && nothing_in_way(piece, -1, 0, end_col, end_row)) ||
                (end_col > piece.pos.col && nothing_in_way(piece, 1, 0, end_col, end_row))) {
                return true;
            }
        }
        if (abs(end_row - piece.pos.row) == abs(end_col - piece.pos.col)) {
            if (end_row < piece.pos.row && end_col < piece.pos.col &&
                nothing_in_way(piece, -1, -1, end_col, end_row)) {
                return true;
            }
            if (end_row < piece.pos.row && end_col > piece.pos.col &&
                nothing_in_way(piece, -1, 1, end_col, end_row)) {
                return true;
            }
            if (end_row > piece.pos.row && end_col < piece.pos.col &&
                nothing_in_way(piece, 1, -1, end_col, end_row)) {
                return true;
            }
            if (end_row > piece.pos.row && end_col > piece.pos.col &&
                nothing_in_way(piece, 1, 1, end_col, end_row)) {
                return true;
            }
        }
        break;

    case PieceName::King:
        if ((end_row == piece.pos.row + 1 && ((end_col == piece.pos.col + 1) || (end_col ==
            piece.pos.col - 1) || (end_col == piece.pos.col))) || (end_row == piece.pos.row - 1 
            && ((end_col == piece.pos.col + 1) || (end_col == piece.pos.col - 1) ||
            (end_col == piece.pos.col))) || (end_row == piece.pos.row && ((end_col ==
            piece.pos.col + 1) || (end_col == piece.pos.col - 1)))) {
            return true;
        }
        break;

    }
    return false;
}

// Performs move
bool Board::perform_move(Piece &piece, Position &king, Position &end, 
    std::vector<Piece> &opponents) {
    // checks that movement follows rules for given piece
    if (valid_movement(piece.name, piece, end.row, end.col)) {
        if (do_move(piece, king, end.row, end.col, opponents)) {
            // moves piece on board
            Compare_position compare;
            if (board[end.row][end.col] != ' ') {
                for (int i = 0; i < (int)(opponents.size()); ++i) {
                    if (compare(end, opponents[i].pos)) {
                        opponents.erase(opponents.begin() + i);
                        break;
                    }
                }
            }
            board[piece.pos.row][piece.pos.col] = ' ';
            board[end.row][end.col] = piece.letter;
            print_board();
            return true;
        }
        return false;
    }
    return false;
}

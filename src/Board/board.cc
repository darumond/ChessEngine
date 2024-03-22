#include <utility>

#include "board.hh"
#include "../Pieces/pawn.hh"
#include "../Pieces/bishop.hh"
#include "../Pieces/queen.hh"
#include "../Pieces/rook.hh"
#include "../Pieces/king.hh"
#include "../Pieces/knight.hh"

board::board(const std::string &perft)
{
    initialize(perft);
}
bool check_border(int x1, int y1)
{
    return x1 < BOARD_SIZE && x1 >= 0 && y1 < BOARD_SIZE && y1 >= 0;
}

size_t board::generateMove(int depth, bool print_move)
{
    if (depth == 1)
    {
        auto vec = clean_moves();
        //        print_chess_board();
        en_passant_ = std::make_pair(-1, -1);
        if (print_move)
            pretty_print_move(vec);
        return vec.size();
    }
    else
    {
        auto moves = clean_moves();
        size_t res = 0;
        en_passant_ = std::make_pair(-1, -1);
        for (size_t i = 0; i < moves.size(); ++i)
        {
            auto &move = moves[i];
            auto piece = chessBoard_[move.getCurrX()][move.getCurrY()];
            std::shared_ptr<Piece> undo;
            if (move.isEnPassant1())
            {
                if (player_to_move_ == Color::white)
                    undo = chessBoard_[move.getDestX() + 1][move.getDestY()];
                else
                    undo = chessBoard_[move.getDestX() - 1][move.getDestY()];
            }
            else
                undo = chessBoard_[move.getDestX()][move.getDestY()];
            move_piece(piece, move);
            auto rook_can_castle = false;
            auto king_can_castle_king = false;
            auto king_can_castle_queen = false;
            if (piece->get_type() == Type::rook)
            {
                auto rook = std::dynamic_pointer_cast<Rook>(piece);
                rook_can_castle = rook->isCanCastle();
                rook->setCanCastle(false);
            }
            else if (piece->get_type() == Type::king)
            {
                auto king = std::dynamic_pointer_cast<King>(piece);
                king_can_castle_king = king->isCanCastleKing();
                king_can_castle_queen = king->isCanCastleQueen();
                king->setCanCastleQueen(false);
                king->setCanCastleKing(false);
            }

            auto save_enpassant = en_passant_;
            if (piece->get_type() == Type::pawn && move.isDouble1())
            {
                if (check_border(piece->get_x(), piece->get_y() + 1) && chessBoard_[piece->get_x()][piece->get_y() + 1] != nullptr && chessBoard_[piece->get_x()][piece->get_y() + 1]->get_type() == Type::pawn && chessBoard_[piece->get_x()][piece->get_y() + 1]->get_color() != player_to_move_)
                {
                    if (player_to_move_ == Color::black)
                        en_passant_ = std::make_pair(piece->get_x() - 1, piece->get_y());
                    else
                        en_passant_ = std::make_pair(piece->get_x() + 1, piece->get_y());
                }
                if (check_border(piece->get_x(), piece->get_y() - 1) && chessBoard_[piece->get_x()][piece->get_y() - 1] != nullptr && chessBoard_[piece->get_x()][piece->get_y() - 1]->get_type() == Type::pawn && chessBoard_[piece->get_x()][piece->get_y() - 1]->get_color() != player_to_move_)
                {
                    if (player_to_move_ == Color::black)
                        en_passant_ = std::make_pair(piece->get_x() - 1, piece->get_y());
                    else
                        en_passant_ = std::make_pair(piece->get_x() + 1, piece->get_y());
                }
            }

            this->player_to_move_ = this->player_to_move_ == Color::white ? Color::black : Color::white;
            res += generateMove(depth - 1, print_move);
            this->player_to_move_ = this->player_to_move_ == Color::white ? Color::black : Color::white;
            if (piece->get_type() == Type::rook)
            {
                auto rook = std::dynamic_pointer_cast<Rook>(piece);
                rook->setCanCastle(rook_can_castle);
            }
            else if (piece->get_type() == Type::king)
            {
                auto king = std::dynamic_pointer_cast<King>(piece);
                king->setCanCastleQueen(king_can_castle_queen);
                king->setCanCastleKing(king_can_castle_king);
            }
            undo_move(piece, move, undo);
            en_passant_ = save_enpassant;
        }
        return res;
    }
}

void board::moveTo(std::shared_ptr<Piece> p, int destX, int destY, std::shared_ptr<Piece> before = nullptr)
{
    int old_x = p->get_x();
    int old_y = p->get_y();
    p->setX(destX);
    p->setY(destY);
    chessBoard_[old_x][old_y] = nullptr;
    if (before != nullptr)
        chessBoard_[before->get_x()][before->get_y()] = before;
    chessBoard_[destX][destY] = p;
}

void board::undo_move(std::shared_ptr<Piece> p, Move move, std::shared_ptr<Piece> before = nullptr)
{
    if (move.isCastle1())
    {
        int kingx = p->get_x();
        int kingy = p->get_y();
        if (kingx == 0 && kingy == 6)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[0][5]);
            moveTo(rook, 0, BOARD_SIZE - 1);
        }
        else if (kingx == 0 && kingy == 2)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[0][3]);
            moveTo(rook, 0, 0);
        }
        else if (kingx == 7 && kingy == 6)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[BOARD_SIZE - 1][5]);
            moveTo(rook, BOARD_SIZE - 1, BOARD_SIZE - 1);
        }
        else if (kingx == 7 && kingy == 2)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[BOARD_SIZE - 1][3]);
            moveTo(rook, BOARD_SIZE - 1, 0);
        }
    }
    if (move.isPromotion1())
    {
        p = std::make_shared<Pawn>(p->get_color(), p->get_x(), p->get_y());
        chessBoard_[move.getDestX()][move.getDestY()] = before;
    }
    moveTo(std::move(p), move.getCurrX(), move.getCurrY(), std::move(before));
}

void board::move_piece(std::shared_ptr<Piece> p, Move move)
{
    if (move.isCastle1())
    {
        if (move.getDestX() == 0 && move.getDestY() == 6)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[0][BOARD_SIZE - 1]);
            moveTo(rook, 0, 5);
        }
        else if (move.getDestX() == 0 && move.getDestY() == 2)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[0][0]);
            moveTo(rook, 0, 3);
        }
        else if (move.getDestX() == 7 && move.getDestY() == 6)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[BOARD_SIZE - 1][BOARD_SIZE - 1]);
            moveTo(rook, BOARD_SIZE - 1, 5);
        }
        else if (move.getDestX() == 7 && move.getDestY() == 2)
        {
            auto rook = std::dynamic_pointer_cast<Rook>(chessBoard_[BOARD_SIZE - 1][0]);
            moveTo(rook, BOARD_SIZE - 1, 3);
        }
    }
    if (move.isEnPassant1())
    {
        if (player_to_move_ == Color::white)
            chessBoard_[move.getDestX() + 1][move.getDestY()] = nullptr;
        else
            chessBoard_[move.getDestX() - 1][move.getDestY()] = nullptr;
    }
    if (move.isPromotion1())
    {
        if (move.getPromotion() == BISHOP)
            p = std::make_shared<Bishop>(p->get_color(), p->get_x(), p->get_y());
        else if (move.getPromotion() == KNIGHT)
            p = std::make_shared<Knight>(p->get_color(), p->get_x(), p->get_y());
        else if (move.getPromotion() == ROOK)
            p = std::make_shared<Rook>(p->get_color(), p->get_x(), p->get_y());
        else if (move.getPromotion() == QUEEN)
            p = std::make_shared<Queen>(p->get_color(), p->get_x(), p->get_y());
    }
    moveTo(p, move.getDestX(), move.getDestY());
}

std::shared_ptr<Piece> board::createPiece(char c, int x, int y)
{
    switch (c)
    {
    case 'P':
        return std::make_shared<Pawn>(Color::white, x, y);
    case 'B':
        return std::make_shared<Bishop>(Color::white, x, y);
    case 'Q':
        return std::make_shared<Queen>(Color::white, x, y);
    case 'R':
    {
        auto rook = std::make_shared<Rook>(Color::white, x, y);
        if (castling_.find("K") != std::string::npos && x == BOARD_SIZE - 1 && y == BOARD_SIZE - 1)
            rook->setCanCastle(true);
        if (castling_.find("Q") != std::string::npos && x == BOARD_SIZE - 1 && y == 0)
            rook->setCanCastle(true);
        return rook;
    }
    case 'K':
    {
        auto king = std::make_shared<King>(Color::white, x, y);
        king->setCanCastleKing(false);
        king->setCanCastleQueen(false);
        if (castling_.find("K") != std::string::npos)
            king->setCanCastleKing(true);
        if (castling_.find("Q") != std::string::npos)
            king->setCanCastleQueen(true);
        setWhiteKing(king);
        return king;
    }
    case 'N':
        return std::make_shared<Knight>(Color::white, x, y);
    case 'p':
        return std::make_shared<Pawn>(Color::black, x, y);
    case 'b':
        return std::make_shared<Bishop>(Color::black, x, y);
    case 'q':
        return std::make_shared<Queen>(Color::black, x, y);
    case 'r':
    {
        auto rook = std::make_shared<Rook>(Color::black, x, y);
        if (castling_.find("k") != std::string::npos && x == 0 && y == BOARD_SIZE - 1)
            rook->setCanCastle(true);
        if (castling_.find("q") != std::string::npos && x == 0 && y == 0)
            rook->setCanCastle(true);
        return rook;
    }
    case 'k':
    {
        auto king = std::make_shared<King>(Color::black, x, y);
        king->setCanCastleKing(false);
        king->setCanCastleQueen(false);
        if (castling_.find("k") != std::string::npos)
            king->setCanCastleKing(true);
        if (castling_.find("q") != std::string::npos)
            king->setCanCastleQueen(true);
        setBlackKing(king);
        return king;
    }
    case 'n':
        return std::make_shared<Knight>(Color::black, x, y);
    default:
        return nullptr;
    }
}

void board::initialize(const std::string &perft)
{
    // std::ifstream inputFile(perft);
    std::string fileContent = perft;

    // PARSING ARGS
    std::stringstream ss(fileContent);
    std::string argument;
    std::string arguments[7];
    int index = 0;

    while (std::getline(ss, argument, ' '))
    {
        arguments[index++] = argument;
        if (index == 7)
            break;
    }
    // PLAYER TO MOVE
    player_to_move(arguments[1]);

    // CHECK CASTLING
    castling_ = arguments[2];

    // EN PASSANT
    en_passant_ = convert_pos(arguments[3]);

    // DEPTH
    //  depth_ = std::stoi(arguments[6]);

    // INITIALIZE CHESS BOARD
    int row = 0;
    int col = 0;
    size_t i;
    for (i = 0; fileContent[i] != ' '; i++)
    {
        if (std::isdigit(fileContent[i]))
        {
            int temp = fileContent[i] - '0';
            while (temp > 0)
            {
                chessBoard_[row][col] = nullptr;
                col++;
                temp--;
            }
        }
        else
        {
            chessBoard_[row][col] = createPiece(fileContent[i], row, col);
            col++;
        }

        if (col == BOARD_SIZE)
        {
            row++;
            i++;
            if (fileContent[i] == ' ')
            {
                i++;
                break;
            }
            col = 0;
        }
    }

    // inputFile.close();
}

void board::player_to_move(const std::string &fileContent)
{
    if (fileContent[0] == 'w')
        player_to_move_ = white;
    else
        player_to_move_ = black;
}

std::pair<int, int> board::convert_pos(const std::string &pos)
{
    if (pos.size() == 1)
        return std::make_pair(-1, -1);
    return std::make_pair(8 - (pos[1] - '0'), pos[0] - 'a');
}

void board::print_chess_board()
{
    std::cout << "En passant: " << en_passant_.first << " " << en_passant_.second << " Depth: " << depth_ << '\n';
    std::cout << "Castling: " << castling_ << '\n';
    std::string move_turn;
    if (player_to_move_ == white)
        move_turn = "White";
    else
        move_turn = "Black";
    std::cout << "Player To move: " << move_turn << '\n';
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (chessBoard_[row][col] == nullptr)
                std::cout << ". ";
            else
                std::cout << *chessBoard_[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

void board::print_count_move()
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            auto piece = chessBoard_[row][col];
            if (piece != nullptr && piece->get_color() == player_to_move_)
                std::cout << piece->to_string() << ": (x = " << piece->get_x() << ", y = "
                          << piece->get_y() << ") -> move = " << piece->move(*this).size() << std::endl;
        }
    }
}

int board::check_piece_diag(std::shared_ptr<Piece> piece)
{

    if (piece == nullptr)
        return 1;

    if (piece->get_color() == player_to_move_)
        return 2;

    if (piece->get_type() != Type::queen &&
        piece->get_type() != Type::bishop)
        return 2;

    return 0;
}

int board::check_piece_line(std::shared_ptr<Piece> piece)
{
    if (piece == nullptr)
        return 1;

    if (piece->get_color() == player_to_move_)
        return 2;

    if (piece->get_type() != Type::queen &&
        piece->get_type() != Type::rook)
        return 2;

    return 0;
}

bool board::check_pawn(std::shared_ptr<Piece> piece)
{
    if (piece == nullptr)
        return true;
    if (piece->get_type() == Type::pawn && piece->get_color() != player_to_move_)
        return false;
    return true;
}

bool board::check_king(std::shared_ptr<Piece> piece)
{
    if (piece == nullptr)
        return true;
    if (piece->get_type() == Type::king && piece->get_color() != player_to_move_)
        return false;
    return true;
}

bool board::check_knight(std::shared_ptr<Piece> piece)
{
    if (piece == nullptr)
        return true;
    if (piece->get_type() == Type::knight && piece->get_color() != player_to_move_)
        return false;
    return true;
}

bool board::check_pawn_king_knight(int current_pos_x, int current_pos_y)
{
    if (player_to_move_ == white)
    {
        // CHECK PAWN
        if (check_border(current_pos_x - 1, current_pos_y + 1))
        {

            if (!check_pawn(chessBoard_[current_pos_x - 1][current_pos_y + 1]))
            {
                return false;
            }
        }
        if (check_border(current_pos_x - 1, current_pos_y - 1))
        {
            if (!check_pawn(chessBoard_[current_pos_x - 1][current_pos_y - 1]))
            {
                return false;
            }
        }
    }
    else
    {
        if (check_border(current_pos_x + 1, current_pos_y + 1))
        {
            if (!check_pawn(chessBoard_[current_pos_x + 1][current_pos_y + 1]))
            {
                return false;
            }
        }
        if (check_border(current_pos_x + 1, current_pos_y - 1))
        {
            if (!check_pawn(chessBoard_[current_pos_x + 1][current_pos_y - 1]))
            {
                return false;
            }
        }
    }
    // CHECK KING
    if (check_border(current_pos_x - 1, current_pos_y))
    {

        if (!check_king(chessBoard_[current_pos_x - 1][current_pos_y]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x - 1, current_pos_y - 1))
    {

        if (!check_king(chessBoard_[current_pos_x - 1][current_pos_y - 1]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x, current_pos_y - 1))
    {

        if (!check_king(chessBoard_[current_pos_x][current_pos_y - 1]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x + 1, current_pos_y - 1))
    {

        if (!check_king(chessBoard_[current_pos_x + 1][current_pos_y - 1]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x + 1, current_pos_y))
    {

        if (!check_king(chessBoard_[current_pos_x + 1][current_pos_y]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x + 1, current_pos_y + 1))
    {

        if (!check_king(chessBoard_[current_pos_x + 1][current_pos_y + 1]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x, current_pos_y + 1))
    {

        if (!check_king(chessBoard_[current_pos_x][current_pos_y + 1]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x - 1, current_pos_y + 1))
    {

        if (!check_king(chessBoard_[current_pos_x - 1][current_pos_y + 1]))
        {
            return false;
        }
    }

    // CHECK KNIGHT
    if (check_border(current_pos_x - 2, current_pos_y + 1))
    {
        if (!check_knight(chessBoard_[current_pos_x - 2][current_pos_y + 1]))
        {
            return false;
        }
    }

    if (check_border(current_pos_x - 2, current_pos_y - 1))
    {
        if (!check_knight(chessBoard_[current_pos_x - 2][current_pos_y - 1]))
        {
            return false;
        }
    }

    if (check_border(current_pos_x + 2, current_pos_y + 1))
    {
        if (!check_knight(chessBoard_[current_pos_x + 2][current_pos_y + 1]))
        {
            return false;
        }
    }

    if (check_border(current_pos_x + 2, current_pos_y - 1))
    {
        if (!check_knight(chessBoard_[current_pos_x + 2][current_pos_y - 1]))
        {
            return false;
        }
    }

    if (check_border(current_pos_x - 1, current_pos_y + 2))
    {
        if (!check_knight(chessBoard_[current_pos_x - 1][current_pos_y + 2]))
        {
            return false;
        }
    }

    if (check_border(current_pos_x + 1, current_pos_y + 2))
    {
        if (!check_knight(chessBoard_[current_pos_x + 1][current_pos_y + 2]))
        {
            return false;
        }
    }
    if (check_border(current_pos_x - 1, current_pos_y - 2))
    {
        if (!check_knight(chessBoard_[current_pos_x - 1][current_pos_y - 2]))
        {
            return false;
        }
    }

    if (check_border(current_pos_x + 1, current_pos_y - 2))
    {
        if (!check_knight(chessBoard_[current_pos_x + 1][current_pos_y - 2]))
        {
            return false;
        }
    }

    return true;
}

bool board::check_check(int sim_x = -1, int sim_y = -1)
{
    int i = 1;
    int current_pos_x;
    int current_pos_y;
    if (player_to_move_ == white)
    {
        current_pos_x = white_king->get_x();
        current_pos_y = white_king->get_y();
    }
    else
    {
        current_pos_x = black_king->get_x();
        current_pos_y = black_king->get_y();
    }
    if (sim_x != -1 && sim_y != -1)
    {
        current_pos_x = sim_x;
        current_pos_y = sim_y;
    }
    if (!check_pawn_king_knight(current_pos_x, current_pos_y))
    {
        return false;
    }
    int checking;
    // CHECK DIAG
    while (check_border(current_pos_x + i, current_pos_y + i))
    {
        checking = check_piece_diag(chessBoard_[current_pos_x + i][current_pos_y + i]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    i = 1;
    while (check_border(current_pos_x - i, current_pos_y - i))
    {
        checking = check_piece_diag(chessBoard_[current_pos_x - i][current_pos_y - i]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    i = 1;
    while (check_border(current_pos_x - i, current_pos_y + i))
    {
        checking = check_piece_diag(chessBoard_[current_pos_x - i][current_pos_y + i]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    i = 1;
    while (check_border(current_pos_x + i, current_pos_y - i))
    {
        checking = check_piece_diag(chessBoard_[current_pos_x + i][current_pos_y - i]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    // CHECK LINE
    i = 1;
    while (check_border(current_pos_x + i, current_pos_y))
    {
        checking = check_piece_line(chessBoard_[current_pos_x + i][current_pos_y]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    i = 1;
    while (check_border(current_pos_x - i, current_pos_y))
    {
        checking = check_piece_line(chessBoard_[current_pos_x - i][current_pos_y]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    i = 1;
    while (check_border(current_pos_x, current_pos_y + i))
    {
        checking = check_piece_line(chessBoard_[current_pos_x][current_pos_y + i]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    i = 1;
    while (check_border(current_pos_x, current_pos_y - i))
    {
        checking = check_piece_line(chessBoard_[current_pos_x][current_pos_y - i]);
        if (checking == 0)
            return false;
        if (checking == 2)
            break;
        i++;
    }

    return true;
}

// Function to convert a single algebraic notation position (e.g., "e2") to board coordinates (x, y)
std::pair<int, int> board::convertSinglePositionFromAlgebraicNotation(const std::string &pos)
{
    if (pos.length() != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8')
    {
        throw std::invalid_argument("Invalid algebraic notation: " + pos);
    }

    int x = pos[0] - 'a';       // Convert 'a'-'h' to 0-7
    int y = 8 - (pos[1] - '0'); // Convert '1'-'8' to 7-0 (since the chessboard y-axis is inverted)
    return {y, x};
}

// Function to convert an entire move from algebraic notation (e.g., "e2e4") to board coordinates
std::pair<std::pair<int, int>, std::pair<int, int>> board::convertFromAlgebraicNotation(const std::string &move)
{
    auto fromPos = move.substr(0, 2);
    auto toPos = move.substr(2, 2);

    auto fromCoords = convertSinglePositionFromAlgebraicNotation(fromPos);
    auto toCoords = convertSinglePositionFromAlgebraicNotation(toPos);

    return {fromCoords, toCoords};
}

std::string board::convertToAlgebraicNotation(int x, int y)
{
    char column = 'a' + x;
    int row = 8 - y;
    std::string algebraicNotation = "";
    algebraicNotation += column;
    algebraicNotation += std::to_string(row);
    return algebraicNotation;
}

void board::pretty_print_move(std::vector<Move> moves)
{
    for (size_t i = 0; i < moves.size(); i++)
    {
        std::string source = convertToAlgebraicNotation(moves[i].getCurrY(), moves[i].getCurrX());
        std::string dest = convertToAlgebraicNotation(moves[i].getDestY(), moves[i].getDestX());
        std::cout << source << dest << "\n";
    }
}

void board::check_legal_moves(std::vector<Move> &moves)
{
    for (size_t i = 0; i < moves.size(); ++i)
    {
        auto move = moves[i];
        auto p = chessBoard_[move.getCurrX()][move.getCurrY()];

        std::shared_ptr<Piece> saveEnPassant = nullptr;
        if (move.isEnPassant1())
        {
            if (move.getColor() == Color::white)
            {
                saveEnPassant = chessBoard_[move.getDestX() + 1][move.getDestY()];
                chessBoard_[move.getDestX() + 1][move.getDestY()] = nullptr;
            }
            else if (move.getColor() == Color::black)
            {
                saveEnPassant = chessBoard_[move.getDestX() - 1][move.getDestY()];
                chessBoard_[move.getDestX() - 1][move.getDestY()] = nullptr;
            }
        }
        auto undo = chessBoard_[move.getDestX()][move.getDestY()];
        move_piece(p, move);
        bool islegal = check_check();
        undo_move(p, move, undo);
        if (move.isEnPassant1())
        {
            if (move.getColor() == Color::white)
                chessBoard_[move.getDestX() + 1][move.getDestY()] = saveEnPassant;
            else if (move.getColor() == Color::black)
                chessBoard_[move.getDestX() - 1][move.getDestY()] = saveEnPassant;
        }

        if (!islegal)
        {
            moves.erase(moves.begin() + i);
            i--;
        }
    }
}

std::vector<Move> board::clean_moves()
{
    std::vector<Move> res;
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            if (chessBoard_[row][col] != nullptr && chessBoard_[row][col]->get_color() == player_to_move_)
            {
                auto moves_piece = chessBoard_[row][col]->move(*this);
                res.insert(res.end(), moves_piece.begin(), moves_piece.end());
            }

    check_legal_moves(res);
    return res;
}

int board::count_move()
{
    clean_moves();
    return moves_.size();
}

Color board::get_Player_To_Move() const
{
    return player_to_move_;
}

const std::string &board::get_Castling() const
{
    return castling_;
}

const std::pair<int, int> &board::get_EnPassant() const
{
    return en_passant_;
}

int board::get_Depth() const
{
    return depth_;
}

const board::boardType board::get_Board() const
{
    return chessBoard_;
}

const std::shared_ptr<Piece> board::getWhiteKing() const
{
    return white_king;
}

void board::setWhiteKing(const std::shared_ptr<Piece> whiteKing)
{
    white_king = whiteKing;
}

const std::shared_ptr<Piece> board::getBlackKing() const
{
    return black_king;
}

void board::setBlackKing(const std::shared_ptr<Piece> blackKing)
{
    black_king = blackKing;
}

Move::Move(int currX, int currY, int destX, int destY) : curr_x(currX), curr_y(currY), destX(destX), destY(destY),
                                                         isEnPassant(false), isCastle(false) {}

int Move::getDestX() const
{
    return destX;
}

int Move::getDestY() const
{
    return destY;
}

int Move::getCurrX() const
{
    return curr_x;
}

int Move::getCurrY() const
{
    return curr_y;
}

bool Move::isEnPassant1() const
{
    return isEnPassant;
}

void Move::setIsEnPassant(bool isEnPassant)
{
    Move::isEnPassant = isEnPassant;
}

bool Move::isCastle1() const
{
    return isCastle;
}

void Move::setIsCastle(bool isCastle)
{
    Move::isCastle = isCastle;
}

Color board::getPlayerToMove()
{
    return player_to_move_;
}
void board::setPlayerToMove(Color player)
{
    board::player_to_move_ = player;
}

Color Move::getColor() const
{
    return color;
}

void Move::setColor(Color color)
{
    Move::color = color;
}
bool Move::isDouble1() const
{
    return isDouble;
}
void Move::setIsDouble(bool isDouble)
{
    Move::isDouble = isDouble;
}

bool Move::isPromotion1() const
{
    return isPromotion;
}

void Move::setIsPromotion(bool isPromotion)
{
    Move::isPromotion = isPromotion;
}

Promotion Move::getPromotion() const
{
    return promotion;
}

void Move::setPromotion(Promotion promotion)
{
    Move::promotion = promotion;
}

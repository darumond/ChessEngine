#pragma once

#include <map>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#define BOARD_SIZE 8

class Piece;

enum Color
{
    white,
    black
};

enum Type
{
    pawn,
    bishop,
    knight,
    rook,
    king,
    queen
};

enum Promotion
{
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN
};

class Move
{

private:
    int curr_x;
    int curr_y;
    int destX;
    int destY;
    bool isEnPassant;
    bool isCastle;
    bool isPromotion = false;
    Promotion promotion;
    bool isDouble = false;
    Color color;

public:
    bool isDouble1() const;
    void setIsDouble(bool isDouble);
    bool isPromotion1() const;

    void setIsPromotion(bool isPromotion);

    Promotion getPromotion() const;

    void setPromotion(Promotion promotion);

    Color getColor() const;

    void setColor(Color color);

    Move(int currX, int currY, int destX, int destY);

    int getCurrX() const;

    int getCurrY() const;

    int getDestX() const;

    int getDestY() const;

    bool isEnPassant1() const;

    void setIsEnPassant(bool isEnPassant);

    bool isCastle1() const;

    void setIsCastle(bool isCastle);
};

class board
{
public:
    using boardType = std::array<std::array<std::shared_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE>;

    board(const std::string &perft);

    void initialize(const std::string &perft);

    void player_to_move(const std::string &fileContent);

    std::pair<int, int> convert_pos(const std::string &pos);

    void print_chess_board();

    int count_move();

    void print_count_move();

    Color get_Player_To_Move() const;

    const std::string &get_Castling() const;

    const std::pair<int, int> &get_EnPassant() const;

    int get_Depth() const;

    const boardType get_Board() const;

    const std::shared_ptr<Piece> getWhiteKing() const;

    void setWhiteKing(const std::shared_ptr<Piece> whiteKing);

    const std::shared_ptr<Piece> getBlackKing() const;

    void setBlackKing(const std::shared_ptr<Piece> blackKing);

    void check_legal_moves(std::vector<Move> &moves);

    bool check_check(int sim_x, int sim_y);

    bool check_pawn_king_knight(int current_pos_x, int current_pos_y);

    bool check_pawn(std::shared_ptr<Piece> piece);

    bool check_king(std::shared_ptr<Piece> piece);

    bool check_knight(std::shared_ptr<Piece> piece);

    int check_piece_diag(std::shared_ptr<Piece> piece);

    int check_piece_line(std::shared_ptr<Piece> piece);

    void pretty_print_move(std::vector<Move> moves);

    std::vector<Move> clean_moves();

    size_t generateMove(int depth, bool print_move);

    std::shared_ptr<Piece> createPiece(char c, int x, int y);

    void moveTo(std::shared_ptr<Piece> p, int destX, int destY, std::shared_ptr<Piece> before);

    void move_piece(std::shared_ptr<Piece> p, Move move);

    void undo_move(std::shared_ptr<Piece> p, Move move, std::shared_ptr<Piece> before);

    std::string convertToAlgebraicNotation(int x, int y);
    std::pair<int, int> convertSinglePositionFromAlgebraicNotation(const std::string &pos);
    std::pair<std::pair<int, int>, std::pair<int, int>> convertFromAlgebraicNotation(const std::string &move);
    Color getPlayerToMove();
    void setPlayerToMove(Color player);

private:
    boardType chessBoard_;
    Color player_to_move_;
    std::string castling_;
    std::pair<int, int> en_passant_;
    int depth_;
    std::vector<Move> moves_;
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;
};

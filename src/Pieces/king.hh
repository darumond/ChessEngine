#pragma once

#include "piece.hh"

class King : public Piece {

    bool canCastleQueen;
    bool canCastleKing;
public:
    bool isCanCastleQueen() const;

    void setCanCastleQueen(bool canCastleQueen);

    bool isCanCastleKing() const;

    void setCanCastleKing(bool canCastleKing);

public:
    King(Color color, int x, int y) {
        this->color = color;
        this->type = king;
        this->x = x;
        this->y = y;
    }

    bool isCanCastle() const;

    void setCanCastle(bool canCastle);

    std::vector<Move> move( board &board) override;

    std::vector<Move> moveCastle( board &board);

    std::string to_string() const override {
        if (color == Color::white)
            return "♚";
        else
            return "♔";
    }

};
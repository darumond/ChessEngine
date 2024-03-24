#pragma once

#include "piece.hh"

class Rook : public Piece {
    bool canCastle;
public:
    Rook(Color color, int x, int y) {
        this->color = color;
        this->type = rook;
        this->x = x;
        this->y = y;
    }

    void setCanCastle(bool canCastle);

    bool isCanCastle() const;

    std::vector<Move> move( board &board) override;
    Rook *clone() const override;

    std::string to_string() const override {
        if (color == Color::white)
           return "♜";
        else
            return "♖";
    }

};



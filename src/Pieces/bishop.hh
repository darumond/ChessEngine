#pragma once

#include "piece.hh"

class Bishop : public Piece {

public:
    Bishop(Color color, int x, int y) {
        this->color = color;
        this->type = bishop;
        this->x = x;
        this->y = y;
    }

    std::vector<Move>move( board &board) override;
    Bishop *clone() const override;

    std::string to_string() const override {
        if (color == Color::white)
            return "♝";
        else
            return "♗";
    }

};



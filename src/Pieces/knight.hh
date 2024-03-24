#pragma once

#include "piece.hh"

class Knight : public Piece{

public:
    Knight(Color color, int x, int y) {
        this->color = color;
        this->type = knight;
        this->x = x;
        this->y = y;
    }

    std::vector<Move> move(  board &board) override;
    Knight *clone() const override;

    std::string to_string() const override {
        if (color == Color::white)
            return "♞";
        else
            return "♘";
    }

};
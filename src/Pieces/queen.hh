#pragma once

#include "piece.hh"

class Queen : public Piece{

public:
    Queen(Color color, int x, int y) {
        this->color = color;
        this->type = queen;
        this->x = x;
        this->y = y;
    }

    std::vector<Move> move( board &board) override;

    std::string to_string() const override {
        if (color == Color::white)
            return "♛";
        else
            return "♕";
    }

};



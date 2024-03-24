#pragma once

#include "piece.hh"

class Pawn : public Piece
{

public:
    Pawn(Color color, int x, int y)
    {
        this->color = color;
        this->type = pawn;
        this->x = x;
        this->y = y;
    }

    std::vector<Move> move(board &board) override;
    Pawn *clone() const override;
    bool moveAllowedPawn(std::shared_ptr<Piece> piece, bool diag);

    std::string to_string() const override
    {
        if (color == Color::white)
            return "♟";
        else
            return "♙";
    }
};

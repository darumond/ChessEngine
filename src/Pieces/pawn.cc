#include "pawn.hh"

bool Pawn::moveAllowedPawn(std::shared_ptr<Piece> piece, bool diag) {

    if (piece == nullptr && diag)
        return false;
    if (piece == nullptr && !diag)
        return true;
    if (piece->get_color() == this->color)
        return false;
    if (piece != nullptr && diag && piece->get_type() != Type::king && piece->get_color() != this->color)
        return true;
    return false;

}

std::vector<Move> Pawn::move( board &board) {
    Promotion promotion[4] = {BISHOP,KNIGHT,ROOK,QUEEN};
    std::vector<Move> moves;
    bool skip = false;
    if (board.get_EnPassant().first != -1) {
        if (this->color == black) {
            if (this->x + 1 == board.get_EnPassant().first && this->y + 1 == board.get_EnPassant().second) {
                auto move = Move(this->x, this->y, this->x + 1, this->y + 1);
                move.setIsEnPassant(true);
                move.setColor(Color::black);
                moves.push_back(move);
            } else if (this->x + 1 == board.get_EnPassant().first && this->y - 1 == board.get_EnPassant().second) {
                auto move = Move(this->x, this->y, this->x + 1, this->y - 1);
                move.setIsEnPassant(true);
                move.setColor(Color::black);
                moves.push_back(move);
            }
        } else {
            if (this->x - 1 == board.get_EnPassant().first && this->y - 1 == board.get_EnPassant().second) {
                auto move = Move(this->x, this->y, this->x - 1, this->y - 1);
                move.setIsEnPassant(true);
                move.setColor(Color::white);
                moves.push_back(move);
            } else if (this->x - 1 == board.get_EnPassant().first && this->y + 1 == board.get_EnPassant().second) {
                auto move = Move(this->x, this->y, this->x - 1, this->y + 1);
                move.setIsEnPassant(true);
                move.setColor(Color::white);
                moves.push_back(move);
            }
        }
    }

    if (this->color == white) {
        if (moveAllowedPawn(board.get_Board()[this->x - 1][this->y], false)) {
            if (this->x - 1 == 0)
                for (int i = 0; i < 4; ++i)
                {
                    auto move = Move(this->x, this->y, this->x - 1, this->y);
                    move.setIsPromotion(true);
                    move.setPromotion(promotion[i]);
                    moves.push_back(move);
                }
            else
                moves.emplace_back(this->x, this->y, this->x - 1, this->y);
        } else
            skip = true;
        if (!skip && this->x == 6 && moveAllowedPawn(board.get_Board()[this->x - 2][this->y], false))
        {
            auto move = Move(this->x, this->y, this->x - 2, this->y);
            move.setIsDouble(true);
            moves.emplace_back(move);
        }
        if (check_border(this->x - 1, this->y + 1) &&
            moveAllowedPawn(board.get_Board()[this->x - 1][this->y + 1], true)) {
            if (this->x - 1 == 0)
                for (int i = 0; i < 4; ++i)
                {
                    auto move = Move(this->x, this->y, this->x - 1, this->y + 1);
                    move.setIsPromotion(true);
                    move.setPromotion(promotion[i]);
                    moves.push_back(move);
                }
            else
                moves.emplace_back(this->x, this->y, this->x - 1, this->y + 1);
        }
        if (check_border(this->x - 1, this->y - 1) &&
            moveAllowedPawn(board.get_Board()[this->x - 1][this->y - 1], true)) {
            if (this->x - 1 == 0)
                for (int i = 0; i < 4; ++i)
                {
                    auto move = Move(this->x, this->y, this->x - 1, this->y - 1);
                    move.setIsPromotion(true);
                    move.setPromotion(promotion[i]);
                    moves.push_back(move);
                }
            else
                moves.emplace_back(this->x, this->y, this->x - 1, this->y - 1);
        }
    } else {
        if (moveAllowedPawn(board.get_Board()[this->x + 1][this->y], false)) {
            if (this->x + 1 == 7)
                for (int i = 0; i < 4; ++i)
                {
                    auto move = Move(this->x, this->y, this->x + 1, this->y);
                    move.setIsPromotion(true);
                    move.setPromotion(promotion[i]);
                    moves.push_back(move);
                }
            else
                moves.emplace_back(this->x, this->y, this->x + 1, this->y);
        } else
            skip = true;
        if (!skip && this->x == 1 && moveAllowedPawn(board.get_Board()[this->x + 2][this->y], false))
        {
            auto move = Move(this->x, this->y, this->x + 2, this->y);
            move.setIsDouble(true);
            moves.emplace_back(move);
        }
        if (check_border(this->x + 1, this->y - 1) &&
            moveAllowedPawn(board.get_Board()[this->x + 1][this->y - 1], true)) {
            if (this->x + 1 == 7)
                for (int i = 0; i < 4; ++i)
                {
                    auto move = Move(this->x, this->y, this->x + 1, this->y - 1);
                    move.setIsPromotion(true);
                    move.setPromotion(promotion[i]);
                    moves.push_back(move);
                }
            else
                moves.emplace_back(this->x, this->y, this->x + 1, this->y - 1);
        }
        if (check_border(this->x + 1, this->y + 1) &&
            moveAllowedPawn(board.get_Board()[this->x + 1][this->y + 1], true)) {
            if (this->x + 1 == 7)
                for (int i = 0; i < 4; ++i)
                {
                    auto move = Move(this->x, this->y, this->x + 1, this->y + 1);
                    move.setIsPromotion(true);
                    move.setPromotion(promotion[i]);
                    moves.push_back(move);
                }
            else
                moves.emplace_back(this->x, this->y, this->x + 1, this->y + 1);
        }
    }

    return moves;
}

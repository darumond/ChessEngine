#include "rook.hh"

bool Rook::isCanCastle() const {
    return canCastle;
}

void Rook::setCanCastle(bool canCastle) {
    Rook::canCastle = canCastle;
}

std::vector<Move> Rook::move( board &board) {
    std::vector<Move> moves;

    bool eat = false;
    int i = 1;
    while (check_border(this->x + i, this->y) && move_allowed(board.get_Board()[this->x + i][this->y], eat)) {
        moves.emplace_back(this->x, this->y, this->x + i, this->y);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x - i, this->y) && move_allowed(board.get_Board()[this->x - i][this->y], eat)) {
        moves.emplace_back(this->x, this->y, this->x - i, this->y);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x, this->y + i) && move_allowed(board.get_Board()[this->x][this->y + i], eat)) {
        moves.emplace_back(this->x, this->y, this->x, this->y + i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x, this->y - i) && move_allowed(board.get_Board()[this->x][this->y - i], eat)) {
        moves.emplace_back(this->x, this->y, this->x, this->y - i);
        i++;
        if (eat)
            break;
    }

    return moves;
}

Rook *Rook::clone() const
{
    return new Rook(*this);
}

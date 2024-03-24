#include "queen.hh"

std::vector<Move> Queen::move( board &board) {
    std::vector<Move> moves;
    bool eat = false;
    int i = 1;
    while (check_border(this->x + i, this->y + i) && move_allowed(board.get_Board()[this->x + i][this->y + i], eat)) {
        moves.emplace_back(this->x, this->y, this->x + i, this->y + i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x - i, this->y - i) && move_allowed(board.get_Board()[this->x - i][this->y - i], eat)) {
        moves.emplace_back(this->x, this->y, this->x - i, this->y - i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x - i, this->y + i) && move_allowed(board.get_Board()[this->x - i][this->y + i], eat)) {
        moves.emplace_back(this->x, this->y, this->x - i, this->y + i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x + i, this->y - i) && move_allowed(board.get_Board()[this->x + i][this->y - i], eat)) {
        moves.emplace_back(this->x, this->y, this->x + i, this->y - i);
        i++;
        if (eat)
            break;
    }

    eat = false;
    i = 1;
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

Queen *Queen::clone() const
{
    return new Queen(*this);
}

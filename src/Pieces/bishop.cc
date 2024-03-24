#include "bishop.hh"

std::vector<Move>Bishop::move( board &board) {
    std::vector<Move> move;
    bool eat = false;
    int i = 1;
    while (check_border(this->x + i, this->y + i) && move_allowed(board.get_Board()[this->x + i][this->y + i], eat)) {
        move.emplace_back(this->x, this->y,this->x+i,this->y +i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x - i, this->y - i) && move_allowed(board.get_Board()[this->x - i][this->y - i], eat)) {
        move.emplace_back(this->x, this->y,this->x-i,this->y - i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x - i, this->y + i) && move_allowed(board.get_Board()[this->x - i][this->y + i], eat)) {
        move.emplace_back(this->x, this->y,this->x - i,this->y + i);
        i++;
        if (eat)
            break;
    }

    i = 1;
    eat = false;
    while (check_border(this->x + i, this->y - i) && move_allowed(board.get_Board()[this->x + i][this->y - i], eat)) {
        move.emplace_back(this->x, this->y,this->x+i,this->y - i);
        i++;
        if (eat)
            break;
    }

    return move;
}

Bishop *Bishop::clone() const
{
    return new Bishop(*this);
}

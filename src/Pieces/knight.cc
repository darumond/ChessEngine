#include "knight.hh"

std::vector<Move> Knight::move( board &board) {
    std::vector<Move> move;
    bool eat = false;

    if (check_border(this->x - 2, this->y + 1) && move_allowed(board.get_Board()[this->x - 2][this->y + 1], eat))
        move.emplace_back(this->x, this->y,this->x - 2, this->y + 1);

    if (check_border(this->x - 2, this->y - 1) && move_allowed(board.get_Board()[this->x - 2][this->y - 1], eat))
        move.emplace_back(this->x, this->y,this->x - 2, this->y - 1);

    if (check_border(this->x + 2, this->y + 1) && move_allowed(board.get_Board()[this->x + 2][this->y + 1], eat))
        move.emplace_back(this->x, this->y,this->x + 2, this->y + 1);

    if (check_border(this->x + 2, this->y - 1) && move_allowed(board.get_Board()[this->x + 2][this->y - 1], eat))
        move.emplace_back(this->x, this->y,this->x + 2, this->y - 1);

    if (check_border(this->x - 1, this->y + 2) && move_allowed(board.get_Board()[this->x - 1][this->y + 2], eat))
        move.emplace_back(this->x, this->y,this->x - 1, this->y + 2);

    if (check_border(this->x + 1, this->y + 2) && move_allowed(board.get_Board()[this->x + 1][this->y + 2], eat))
        move.emplace_back(this->x, this->y,this->x + 1, this->y + 2);

    if (check_border(this->x - 1, this->y - 2) && move_allowed(board.get_Board()[this->x - 1][this->y - 2], eat))
        move.emplace_back(this->x, this->y,this->x - 1, this->y - 2);

    if (check_border(this->x + 1, this->y - 2) && move_allowed(board.get_Board()[this->x + 1][this->y - 2], eat))
        move.emplace_back(this->x, this->y,this->x + 1, this->y - 2);

    return move;

}

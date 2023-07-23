#include "king.hh"
#include "rook.hh"

std::vector<Move> King::moveCastle(board &board) {
    std::vector<Move> move;
    if (this->color == Color::white) {
        if(this->canCastleKing) {
            if (board.get_Board()[BOARD_SIZE - 1][BOARD_SIZE - 1] != nullptr &&
                board.get_Board()[BOARD_SIZE - 1][BOARD_SIZE - 1]->get_type() == Type::rook) {
                std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(
                        board.get_Board()[BOARD_SIZE - 1][BOARD_SIZE - 1]);
                if (rook->get_color() == Color::white && rook->isCanCastle()) {
                    bool isEmpty = true;
                    for (int i = this->y + 1; i < BOARD_SIZE - 1; i++)
                        if (board.get_Board()[this->x][i] != nullptr) {
                            isEmpty = false;
                            break;
                        }
                    if (isEmpty && board.check_check(this->x,this->y+1))
                    {
                        auto mv = Move(this->x, this->y, this->x, this->y + 2);
                        mv.setIsCastle(true);
                        move.push_back(mv);
                    }
                }
            }
        }
        if(this->canCastleQueen) {
            if (board.get_Board()[BOARD_SIZE - 1][0] != nullptr &&
                board.get_Board()[BOARD_SIZE - 1][0]->get_type() == Type::rook) {
                std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(board.get_Board()[BOARD_SIZE - 1][0]);
                if (rook->get_color() == Color::white && rook->isCanCastle()) {
                    bool isEmpty = true;
                    for (int i = this->y - 1; i > 0; i--)
                        if (board.get_Board()[this->x][i] != nullptr) {
                            isEmpty = false;
                            break;
                        }
                    if (isEmpty && board.check_check(this->x,this->y - 1))
                    {
                        auto mv = Move(this->x, this->y, this->x, this->y - 2);
                        mv.setIsCastle(true);
                        move.push_back(mv);
                    }
                }
            }
        }
    }
    else {
        if(this->canCastleQueen) {
            if (board.get_Board()[0][0] != nullptr &&
                board.get_Board()[0][0]->get_type() == Type::rook) {
                std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(board.get_Board()[0][0]);
                if (rook->get_color() == Color::black && rook->isCanCastle()) {
                    bool isEmpty = true;
                    for (int i = this->y - 1; i > 0; i--)
                        if (board.get_Board()[this->x][i] != nullptr) {
                            isEmpty = false;
                            break;
                        }
                    if (isEmpty && board.check_check(this->x,this->y - 1))
                    {
                        auto mv = Move(this->x, this->y, this->x, this->y - 2);
                        mv.setIsCastle(true);
                        move.push_back(mv);
                    }
                }
            }
        }
        if(this->canCastleKing) {
            if (board.get_Board()[0][BOARD_SIZE - 1] != nullptr &&
                board.get_Board()[0][BOARD_SIZE - 1]->get_type() == Type::rook) {
                std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(board.get_Board()[0][BOARD_SIZE - 1]);
                if (rook->get_color() == Color::black && rook->isCanCastle()) {
                    bool isEmpty = true;
                    for (int i = this->y + 1; i < BOARD_SIZE - 1; i++)
                        if (board.get_Board()[this->x][i] != nullptr) {
                            isEmpty = false;
                            break;
                        }
                    if (isEmpty && board.check_check(this->x,this->y + 1))
                    {
                        auto mv = Move(this->x, this->y, this->x, this->y + 2);
                        mv.setIsCastle(true);
                        move.push_back(mv);
                    }
                }
            }
        }
    }
    return move;
}

std::vector<Move> King::move( board &board) {
    std::vector<Move> move;
    bool eat = false;
    if (check_border(this->x, this->y + 1) && move_allowed(board.get_Board()[this->x][this->y + 1], eat))
        move.emplace_back(this->x, this->y, this->x, this->y + 1);

    if (check_border(this->x + 1, this->y) && move_allowed(board.get_Board()[this->x + 1][this->y], eat))
        move.emplace_back(this->x, this->y, this->x + 1, this->y);

    if (check_border(this->x + 1, this->y + 1) && move_allowed(board.get_Board()[this->x + 1][this->y + 1], eat))
        move.emplace_back(this->x, this->y, this->x + 1, this->y + 1);

    if (check_border(this->x, this->y - 1) && move_allowed(board.get_Board()[this->x][this->y - 1], eat))
        move.emplace_back(this->x, this->y, this->x, this->y - 1);

    if (check_border(this->x - 1, this->y) && move_allowed(board.get_Board()[this->x - 1][this->y], eat))
        move.emplace_back(this->x, this->y, this->x - 1, this->y);

    if (check_border(this->x - 1, this->y - 1) && move_allowed(board.get_Board()[this->x - 1][this->y - 1], eat))
        move.emplace_back(this->x, this->y, this->x - 1, this->y - 1);

    if (check_border(this->x + 1, this->y - 1) && move_allowed(board.get_Board()[this->x + 1][this->y - 1], eat))
        move.emplace_back(this->x, this->y, this->x + 1, this->y - 1);

    if (check_border(this->x - 1, this->y + 1) && move_allowed(board.get_Board()[this->x - 1][this->y + 1], eat))
        move.emplace_back(this->x, this->y, this->x - 1, this->y + 1);


    if(board.check_check(this->x,this->y))
    {
        auto castle = moveCastle(board);
        move.insert(move.end(), castle.begin(), castle.end());
    }
    return move;
}


bool King::isCanCastleQueen() const {
    return canCastleQueen;
}

void King::setCanCastleQueen(bool canCastleQueen) {
    King::canCastleQueen = canCastleQueen;
}

bool King::isCanCastleKing() const {
    return canCastleKing;
}

void King::setCanCastleKing(bool canCastleKing) {
    King::canCastleKing = canCastleKing;
}

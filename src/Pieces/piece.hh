#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "board.hh"

#define BOARD_SIZE 8

class Piece {
protected:
    int x;
    int y;
    Color color;
    Type type;

public:
    virtual ~Piece() {};

    virtual std::vector<Move> move( board &board) = 0;

    virtual Piece* clone() const = 0;
    
    virtual std::string to_string() const = 0;

    friend std::ostream &operator<<(std::ostream &out, const Piece &p) {
        out << p.to_string();
        return out;
    }

    inline bool check_border(int x1, int y1) const {
        return x1 < BOARD_SIZE && x1 >= 0 && y1 < BOARD_SIZE && y1 >= 0;
    }

    inline bool move_allowed(std::shared_ptr<Piece> piece, bool &eat) const {
        if (piece == nullptr)
            return true;
        else {
            eat = this->color != piece->get_color() && piece->type != Type::king;
            return eat;
        }
    }

    inline int get_x() { return x; }

    inline int get_y() { return y; }

    inline void setX(int x) { Piece::x = x; }

    inline void setY(int y) { Piece::y = y; }

    inline Type get_type() { return type; }

    inline Color get_color() { return color; }

};
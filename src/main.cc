#include <iostream>
#include "board.hh"

int main(int argc, char **argv) {
    if (argc == 1)
        return 0;
    if (argc == 2)
        std::cout << "TUTORIAL"
                  << "\n";
    if (argc == 3){
        board chessBoard(argv[2]);
        //chessBoard.print_chess_board();

        std::cout << chessBoard.generateMove(chessBoard.get_Depth(),false) << std::endl;
        //chessBoard.print_count_move();
//        chessBoard.pretty_print_move(res);
    }
    if(argc == 4)
    {
        board chessBoard(argv[2]);
        std::cout << chessBoard.generateMove(chessBoard.get_Depth(),true) << std::endl;
    }

    return 0;
}
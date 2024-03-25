#include "mcts.hh"

Move getBestMove(board Chessboard)
{
    // int max = 0;
    auto color = Chessboard.getPlayerToMove();
    auto moves = Chessboard.clean_moves();
    Move res = moves.front();
    int max = 0;
    auto copy = Chessboard;

    for (size_t i = 0; i < moves.size(); i++)
    {
        auto current_move = moves[0];
        auto piece = Chessboard.get_Board()[current_move.getCurrX()][current_move.getCurrY()];
        // Chessboard.move_piece(piece, current_move);
        int gain = 0;
        for (size_t i = 0; i < 10; i++)
        {

            auto mov = Chessboard.clean_moves();
            while (!mov.empty())
            {

                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(1, mov.size() - 1);
                int random_number = distrib(gen);
                if (mov.size() == 1)
                    random_number = 0;
                auto random_move = mov[random_number];
                auto piece = Chessboard.get_Board()[random_move.getCurrX()][random_move.getCurrY()];

                Chessboard.move_piece(piece, random_move);
                // Chessboard.print_chess_board();
                if (Chessboard.getPlayerToMove() == black)
                {
                    Chessboard.setPlayerToMove(white);
                }
                else
                {
                    Chessboard.setPlayerToMove(black);
                    auto king = Chessboard.getBlackKing();
                    // std::cout << king->get_x() << '\n';
                    // std::cout << king->get_y() << '\n';
                }

                mov = Chessboard.clean_moves();

                if (isDraw(Chessboard))
                {
                    break;
                }

                // std::cout << mov.size() << '\n';
            }

            if (mov.empty() && Chessboard.getPlayerToMove() == color)
            {
                gain++;
            }

            Chessboard = copy;
            Chessboard.setPlayerToMove(color);
            // Chessboard.print_chess_board();
        }

        if (gain > max)
        {
            max = gain;
            res = moves[0];
            // std::cout << "gain: " << gain << '\n';
        }
    }

    // Chessboard.print_chess_board();

    return res;
}
#include "uci.hh"
#include <iostream>
#include <string>
#include <sstream>


void uciLoop()
{
    std::string line;
    board chessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    while (getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "uci")
        {
            // Send ID and available options
            std::cout << "id name MyChessEngine\n";
            std::cout << "id author MyName\n";
            std::cout << "uciok\n";
        }
        else if (token == "isready")
        {
            std::cout << "readyok\n";
        }
        else if (token == "ucinewgame")
        {
        }
        else if (token == "position")
        {
            std::string positionType;
            iss >> positionType;

            std::vector<std::string> moves;
            while (iss >> token) // Continue reading all moves
            {
                moves.push_back(token);
            }
            if (moves.size() == 2)
                chessBoard.setPlayerToMove(black);

            if (!moves.empty())
            {

                if (moves.size() == 2)
                {
                    std::string currentMove = moves[1];

                    // Convert from algebraic notation and extract individual coordinates

                    auto moveCoords = chessBoard.convertFromAlgebraicNotation(currentMove);
                    int fromX = moveCoords.first.first;
                    int fromY = moveCoords.first.second;
                    int toX = moveCoords.second.first;
                    int toY = moveCoords.second.second;
                    Move move(fromX, fromY, toX, toY);
                    auto piece = chessBoard.get_Board()[fromX][fromY];

                    chessBoard.move_piece(piece, move);
                }
                else
                {

                    for (size_t i = moves.size() - 2; i < moves.size(); ++i)
                    {
                        std::string currentMove = moves[i];

                        // Convert from algebraic notation and extract individual coordinates

                        auto moveCoords = chessBoard.convertFromAlgebraicNotation(currentMove);
                        int fromX = moveCoords.first.first;
                        int fromY = moveCoords.first.second;
                        int toX = moveCoords.second.first;
                        int toY = moveCoords.second.second;

                        auto piece = chessBoard.get_Board()[fromX][fromY];
                        if (piece != nullptr)
                        {
                            Move move(fromX, fromY, toX, toY);
                            if (currentMove.length() > 4)
                            {
                                char promotionChar = currentMove.back();
                                move.setIsPromotion(true);

                                switch (promotionChar)
                                {
                                case 'q':
                                    move.setPromotion(QUEEN);
                                    break;
                                case 'r':
                                    move.setPromotion(ROOK);
                                    break;
                                case 'b':
                                    move.setPromotion(BISHOP);
                                    break;
                                case 'n':
                                    move.setPromotion(KNIGHT);
                                    break;
                                default:
                                    std::cerr << "Invalid promotion piece: " << promotionChar << std::endl;
                                    break;
                                }
                            }
                            chessBoard.move_piece(piece, move);
                        }
                        else
                        {
                            std::cerr << "No piece found at the starting position of the move: " << currentMove << std::endl;
                        }
                    }
                }
            }
        }

        else if (token == "go")
        {
            // auto moves = chessBoard.clean_moves();
            auto bestMove = getBestMove(chessBoard);
            // chessBoard.pretty_print_move(moves);
            // if (!moves.empty())
            // {
                // Pick the first move for simplicity (or implement a better move selection)
                // auto bestMove = moves.front();

                auto piece = chessBoard.get_Board()[bestMove.getCurrX()][bestMove.getCurrY()];
                chessBoard.move_piece(piece, bestMove);
                std::string moveString = chessBoard.convertToAlgebraicNotation(bestMove.getCurrY(), bestMove.getCurrX()) +
                                         chessBoard.convertToAlgebraicNotation(bestMove.getDestY(), bestMove.getDestX());

                if (bestMove.isPromotion1() == true)
                {
                    moveString += "Q";
                }

                std::cout << "bestmove " << moveString << "\n";
            // }
        }
        else if (token == "quit" || token == "stop")
        {
            break;
        }
    }
}

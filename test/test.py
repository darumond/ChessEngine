import os
import subprocess
import termcolor
import time
from prettytable import PrettyTable
import chess
import chess.engine
import random
import sys

arguments = sys.argv

OK_TAG = f"{termcolor.colored('OK', 'green', attrs=['bold'])}"
KO_TAG = f"{termcolor.colored('KO', 'red', attrs=['bold'])}"
EXCELLENT_TAG = f"{termcolor.colored('PERFECT', 'cyan', attrs=['blink', 'bold'])}"


def generate_fen(depth):
    board = chess.Board()
    moves_to_play = random.randint(1, 40)

    for _ in range(moves_to_play):
        legal_moves = list(board.legal_moves)
        if not legal_moves:
            break  # Si aucune coup légal n'est possible, arrêter la génération

        move = random.choice(legal_moves)
        board.push(move)

    fen = board.fen() + " " + str(depth)
    return fen


def generate_random_file(n, depth):
    folder_path = "test_files"
    for i in range(n):
        file_name = os.path.join(folder_path, "random_chess_fen" + str(i) + ".perft")
        with open(file_name, 'w') as file:
            fen = generate_fen(depth)
            file.write(fen)


def printmoveEngine(test: str) -> str:
    res = subprocess.run(["./chessengine", "--perft", test, "print"], capture_output=True, text=True).stdout.strip()
    return res;


def runChessEngine(test: str) -> int:
    res = subprocess.run(["./chessengine", "--perft", test], capture_output=True, text=True).stdout.strip()
    if res.isdigit():
        return int(res)
    else:
        return 0


def get_legal_moves(board, depth):
    if depth == 0:
        for move in board.legal_moves:
            moves_array_stockfish.append(str(move))
        return len(list(board.legal_moves))

    num_moves = 0
    for move in board.legal_moves:
        board.push(move)
        num_moves += get_legal_moves(board, depth - 1)
        board.pop()

    return num_moves


def runStockFish(test: str, depth: int):
    depth -= 1
    engine = chess.engine.SimpleEngine.popen_uci("stockfish")
    board = chess.Board(test)
    legal_moves = list(board.legal_moves)

    num_moves = get_legal_moves(board, depth)
    engine.quit()
    return num_moves


test_directory = "test_files/"
results_table = PrettyTable()
results_table.field_names = ["Test File", "ChessEngine", "Stockfish", "Depth", "Status", "Runtime"]

if len(sys.argv) == 3:
    num_files = int(sys.argv[1])
    depth = int(sys.argv[2])
    generate_random_file(num_files, depth)

print("                       ♟️ Chess TESTSUITE ♟️")

sorted_results = []
success = 0
total = 0

for root, dirs, files in os.walk(test_directory):
    for filename in files:
        total += 1
        test_file = os.path.join(root, filename)
        start_time = time.time()
        moves_array_stockfish = []
        chessengine_output = runChessEngine(test_file)
        chessengine_move = printmoveEngine(test_file)
        moveEngineSplit = chessengine_move.split("\n")
        lenEngine = len(moveEngineSplit)
        moveEngineSplit.pop(lenEngine - 1)

        with open(test_file, "r") as file:
            content = file.read()

        depth = int(content[-1])
        stockfish_output = runStockFish(content[:-1], depth)

        if chessengine_output != stockfish_output:
            not_in_second_array = set(moveEngineSplit) - set(moves_array_stockfish)

            if(len(not_in_second_array) != 0):
                print("For file " + filename + " en trop:")
                print(not_in_second_array)

            not_in_first_array = set(moves_array_stockfish) - set(moveEngineSplit)

            if(len(not_in_first_array) != 0):
                print("For file " + filename+ " il manque:")
                print(not_in_first_array)

        name = os.path.splitext(os.path.basename(test_file))[0]

        if chessengine_output == stockfish_output:
            success += 1
            status = OK_TAG
            runtime = termcolor.colored(round(time.time() - start_time, 5), "green")
            test_file = termcolor.colored(test_file, "green")
            sorted_results.append([name, chessengine_output, stockfish_output, depth, status, runtime])
        else:
            status = KO_TAG
            test_file = termcolor.colored(test_file, "red")
            runtime = termcolor.colored("N/A", "red")
            sorted_results.append([name, chessengine_output, stockfish_output, depth, status, runtime])

sorted_results = sorted(sorted_results, key=lambda x: x[0])

for result in sorted_results:
    results_table.add_row(result)

print(results_table)
print("You pass :" + str(success) + "/" + str(total))

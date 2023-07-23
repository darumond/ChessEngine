import chess
import chess.svg
import cairosvg
from io import BytesIO
import sys
import pyglet
import pyglet.text

def display_fen(fen):
    board = chess.Board(fen)
    svg_image = chess.svg.board(board=board)

    svg_io = BytesIO(svg_image.encode('utf-8'))
    png_io = BytesIO(cairosvg.svg2png(bytestring=svg_io.getvalue()))
    png_io.seek(0)

    img = pyglet.image.load(filename='', file=png_io, decoder=pyglet.image.codecs.png.PNGImageDecoder())
    window = pyglet.window.Window(width=img.width, height=img.height, caption="Chess Board")
    window.set_icon(img)

    @window.event
    def on_draw():
        window.clear()
        img.blit(0, 0)

        # Afficher le nombre de coups possibles pour chaque pièce
        piece_moves = board.legal_moves
        for move in piece_moves:
            piece = board.piece_at(move.from_square)
            if piece is not None:
                piece_symbol = piece.symbol()
                move_count = len(list(board.generate_legal_moves(from_mask=1 << move.from_square)))
                x, y = move.from_square % 8, move.from_square // 8
                image_width = img.width
                image_height = img.height
                board_size = 8  # Taille du plateau d'échecs (8x8)
                square_size = min(image_width, image_height) // board_size
                label_text = str(move_count) if move_count != 0 else "0"
                label = pyglet.text.Label(
                    text=label_text,
                    x=(move.from_square % 8) * square_size + square_size // 2,
                    y=(move.from_square // 8) * square_size + square_size // 2,
                    anchor_x='center',
                    anchor_y='center',
                    color=(255, 0, 0, 255),  # Rouge
                    font_size=12
                )
                label.draw()


    @window.event
    def on_key_press(symbol, modifiers):
        if symbol == pyglet.window.key.ENTER:
            window.close()
    pyglet.app.run()


def display_fen_from_file(file_path):
    with open(file_path, 'r') as file:
        fen = file.read().strip()[:-1]
        display_fen(fen)

if len(sys.argv) == 2:
    file_path = sys.argv[1]
    display_fen_from_file(file_path)

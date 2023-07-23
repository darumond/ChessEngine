cd test

if [[ $# -eq 1 ]]; then
    python3 displayfen.py "test_files/$1.perft"
else
    echo "display <path_fen.perft>"
fi

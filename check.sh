cmake -B build
cd build
make -j
cp chessengine ../test
cd ../test

if [[ $# -eq 2 ]]; then
    python3 test.py "$1" "$2"
else
    python3 test.py
fi

rm chessengine
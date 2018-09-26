# Huffman

* `make test`: (test with the same file - `test/dc.pcapng`)
```bash
g++ main.cc -std=c++11
./a.out ../test/dc.pcapng
Root node is here, with size: 43864
Character: -, with size: 2068, have huffman code: 0000
Character: �, with size: 516, have huffman code: 000100
Character: �, with size: 130, have huffman code: 00010100
Character: |, with size: 130, have huffman code: 00010101
Character: X, with size: 260, have huffman code: 0001011
Character: �, with size: 262, have huffman code: 0001100
Character: �, with size: 132, have huffman code: 00011010
Character: U, with size: 132, have huffman code: 00011011
Character: s, with size: 266, have huffman code: 0001110
Character: �, with size: 66, have huffman code: 000111100
Character: �, with size: 67, have huffman code: 000111101
Character: V, with size: 134, have huffman code: 00011111
Character: �, with size: 540, have huffman code: 001000
Character: A, with size: 269, have huffman code: 0010010
...
Character: �, with size: 127, have huffman code: 111111001
Character: `, with size: 253, have huffman code: 11111101
Character: �, with size: 128, have huffman code: 111111100
Character: 8, with size: 129, have huffman code: 111111101
Character: B, with size: 258, have huffman code: 11111111
=================================================================
Compressed bits: 310811, Original bits: 350912
Save memory size: 11.427651 %
```
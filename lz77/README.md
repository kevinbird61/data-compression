# LZ77 

* `make test`
```bash
./a.out ../test/dc.pcapng ../test/dc.test.pcapng
Original size: 83276 bytes, 83 KB
Compressed size(1): 92492. Compressed Ratio: -11.066814 %
Compressed size(2): 68297. Compressed Ratio: 17.987175 %
Compressed size(3): 56666. Compressed Ratio: 31.953985 %
Compressed size(4): 49697. Compressed Ratio: 40.322540 %
Compressed size(5): 46460. Compressed Ratio: 44.209618 %
Compressed size(6): 54404. Compressed Ratio: 34.670254 %
Compressed size(7): 65870. Compressed Ratio: 20.901581 %
Compressed size(8): 75374. Compressed Ratio: 9.488929 %
Compressed size(9): 87725. Compressed Ratio: -5.342476 %
Compressed size(10): 105905. Compressed Ratio: -27.173495 %
Compressed size(11): 114884. Compressed Ratio: -37.955714 %
Compressed size(12): 147179. Compressed Ratio: -76.736395 %
Compressed size(13): 154841. Compressed Ratio: -85.937125 %
Compressed size(14): 164969. Compressed Ratio: -98.099092 %
Compressed size(15): 176420. Compressed Ratio: -111.849753 %
LZ77 decompress error: miss matching size. (result: 83289, uncompressed size: 83276)
Success!
```

Best compression ratio: `i=5, 44.209618%`


> Question: the result file size is not matching with the uncompressed size (You can see the result above)
> But still can correctly write the file. (use command `diff <origin file> <after lz77 compress+decompress>`)
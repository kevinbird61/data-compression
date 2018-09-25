# LZ77 

* `make test`
```bash
... 
Original size: 83276 bytes, 83 KB
Compressed size(1): 92492. Compressed Ratio: 1.110668 %
Compressed size(2): 68297. Compressed Ratio: 0.820128 %
Compressed size(3): 56666. Compressed Ratio: 0.680460 %
Compressed size(4): 49697. Compressed Ratio: 0.596775 %
Compressed size(5): 46460. Compressed Ratio: 0.557904 %
Compressed size(6): 54404. Compressed Ratio: 0.653297 %
Compressed size(7): 65870. Compressed Ratio: 0.790984 %
Compressed size(8): 75374. Compressed Ratio: 0.905111 %
Compressed size(9): 87725. Compressed Ratio: 1.053425 %
Compressed size(10): 105905. Compressed Ratio: 1.271735 %
Compressed size(11): 114884. Compressed Ratio: 1.379557 %
Compressed size(12): 147179. Compressed Ratio: 1.767364 %
Compressed size(13): 154841. Compressed Ratio: 1.859371 %
Compressed size(14): 164969. Compressed Ratio: 1.980991 %
Compressed size(15): 176420. Compressed Ratio: 2.118498 %
LZ77 decompress error: miss matching size. (result: 83289, uncompressed size: 83276)
Success!
```

> Question: the result file size is not matching with the uncompressed size (You can see the result above)
> But still can correctly write the file. (use command `diff <origin file> <after lz77 compress+decompress>`)
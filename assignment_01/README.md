# GEMM Test Results

| Test File | Input Type / Size | Expected Output | Simple Time | Blocking Time | Block Size | Status |
|---|---|---|---:|---:|---:|---|
| gemm_test_01.txt | 10x10 and 10x10 | -- | 0.010048 ms | 0.010165 ms | 32 | PASS |
| gemm_test_02.txt | 25x25 and 25x25 | -- | 0.158157 ms | 0.206945 ms | 32 | PASS |
| gemm_test_03.txt | 50x50 and 50x50 | -- | 0.678095 ms | 0.858995 ms | 32 | PASS |
| gemm_test_04.txt | 100x100 and 100x100 | -- | 4.170520 ms | 8.285419 ms | 32 | PASS |
| gemm_test_05.txt | 200x200 and 200x200 | -- | 33.946017 ms | 42.440224 ms | 32 | PASS |
| gemm_test_06.txt | 300x300 and 300x300 | -- | 103.181365 ms | 142.501692 ms | 32 | PASS |
| gemm_test_07.txt | 500x500 and 500x500 | -- | 473.006615 ms | 666.408290 ms | 32 | PASS |
| gemm_test_08.txt | 750x750 and 750x750 | -- | 1832.349201 ms | 2237.341717 ms | 32 | PASS |
| gemm_test_09.txt | 1000x1000 and 1000x1000 | -- | 4384.652222 ms | 5344.884920 ms | 32 | PASS |
| gemm_test_10.txt | 100x50 and 50x150 | -- | 2.987345 ms | 3.933749 ms | 32 | PASS |
| gemm_test_11.txt | 150x75 and 75x200 | -- | 15.265038 ms | 14.121181 ms | 32 | PASS |
| gemm_test_12.txt | 200x100 and 100x300 | -- | 40.173835 ms | 32.386856 ms | 32 | PASS |
| gemm_test_13.txt | 300x150 and 150x400 | -- | 75.175259 ms | 98.995531 ms | 32 | PASS |
| gemm_test_14.txt | 400x200 and 200x500 | -- | 151.758683 ms | 213.889643 ms | 32 | PASS |
| gemm_test_15.txt | 500x250 and 250x600 | -- | 279.843830 ms | 402.351207 ms | 32 | PASS |
| gemm_test_16.txt | 600x300 and 300x700 | -- | 478.204114 ms | 669.785275 ms | 32 | PASS |
| gemm_test_17.txt | 700x350 and 350x800 | -- | 778.891629 ms | 1052.434718 ms | 32 | PASS |
| gemm_test_18.txt | 800x400 and 400x900 | -- | 1125.165472 ms | 1531.172583 ms | 32 | PASS |
| gemm_test_19.txt | 900x450 and 450x1000 | -- | 1492.840779 ms | 2145.767335 ms | 32 | PASS |
| gemm_test_20.txt | 1000x600 and 600x800 | -- | 1899.905154 ms | 2539.581170 ms | 32 | PASS |
| gemm_test_21.txt | 1000x750 and 750x1000 | -- | 2894.380510 ms | 3985.466980 ms | 32 | PASS |
| gemm_test_23.txt | 64x64 and 64x64 | -- | 0.085183 ms | 0.025714 ms | 32 | PASS |
| gemm_test_24.txt | 64x64 and 64x64 | -- | 0.052466 ms | 0.023451 ms | 32 | PASS |
| gemm_test_25.txt | 128x128 and 128x96 | -- | 0.173444 ms | 0.106387 ms | 32 | PASS |
| gemm_test_26.txt | 4x5 and 5x4 | -- | 0.002841 ms | 0.003262 ms | 32 | PASS |

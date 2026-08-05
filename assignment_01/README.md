# GEMM Test Results

| Test File | Input Type / Size | Expected Output | Simple Time | Blocking Time | Block Size | Status |
|---|---|---|---:|---:|---:|---|
| gemm_test_01.txt | 10x10 and 10x10 | -- | 0.008491 ms | 0.008671 ms | 128 | PASS |
| gemm_test_02.txt | 25x25 and 25x25 | -- | 0.082035 ms | 0.099901 ms | 128 | PASS |
| gemm_test_03.txt | 50x50 and 50x50 | -- | 0.621492 ms | 0.756722 ms | 128 | PASS |
| gemm_test_04.txt | 100x100 and 100x100 | -- | 4.382725 ms | 5.590043 ms | 128 | PASS |
| gemm_test_05.txt | 200x200 and 200x200 | -- | 47.377189 ms | 48.875717 ms | 128 | PASS |
| gemm_test_06.txt | 300x300 and 300x300 | -- | 153.068851 ms | 155.829433 ms | 128 | PASS |
| gemm_test_07.txt | 500x500 and 500x500 | -- | 604.245720 ms | 783.901297 ms | 128 | PASS |
| gemm_test_08.txt | 750x750 and 750x750 | -- | 2777.183793 ms | 2512.066226 ms | 128 | PASS |
| gemm_test_09.txt | 1000x1000 and 1000x1000 | -- | 6029.110537 ms | 6090.162298 ms | 128 | PASS |
| gemm_test_10.txt | 100x50 and 50x150 | -- | 3.982657 ms | 5.373427 ms | 128 | PASS |
| gemm_test_11.txt | 150x75 and 75x200 | -- | 12.960525 ms | 22.474948 ms | 128 | PASS |
| gemm_test_12.txt | 200x100 and 100x300 | -- | 32.093380 ms | 36.030841 ms | 128 | PASS |
| gemm_test_13.txt | 300x150 and 150x400 | -- | 77.374607 ms | 104.369352 ms | 128 | PASS |
| gemm_test_14.txt | 400x200 and 200x500 | -- | 190.750557 ms | 268.912594 ms | 128 | PASS |
| gemm_test_15.txt | 500x250 and 250x600 | -- | 349.392146 ms | 515.304216 ms | 128 | PASS |
| gemm_test_16.txt | 600x300 and 300x700 | -- | 592.267161 ms | 755.652626 ms | 128 | PASS |
| gemm_test_17.txt | 700x350 and 350x800 | -- | 960.021000 ms | 1199.903457 ms | 128 | PASS |
| gemm_test_18.txt | 800x400 and 400x900 | -- | 1361.974410 ms | 1737.030103 ms | 128 | PASS |
| gemm_test_19.txt | 900x450 and 450x1000 | -- | 1945.653233 ms | 2396.309572 ms | 128 | PASS |
| gemm_test_20.txt | 1000x600 and 600x800 | -- | 2332.273382 ms | 2980.513815 ms | 128 | PASS |
| gemm_test_21.txt | 1000x750 and 750x1000 | -- | 4174.448010 ms | 4539.402892 ms | 128 | PASS |
| gemm_test_23.txt | 64x64 and 64x64 | -- | 0.054916 ms | 0.013857 ms | 128 | PASS |
| gemm_test_24.txt | 64x64 and 64x64 | -- | 0.047526 ms | 0.015448 ms | 128 | PASS |
| gemm_test_25.txt | 128x128 and 128x96 | -- | 0.111811 ms | 0.054465 ms | 128 | PASS |
| gemm_test_26.txt | 4x5 and 5x4 | -- | 0.002136 ms | 0.002609 ms | 128 | PASS |
| gemm_test_27.txt | 1600x1000 and 1000x1800 | -- | 17812.742502 ms | 17232.041965 ms | 128 | PASS |
| gemm_test_28.txt | 2000x1200 and 1200x1600 | -- | 24909.453539 ms | 23227.905073 ms | 128 | PASS |
| gemm_test_29.txt | 3000x1500 and 1500x2500 | -- | 88921.354798 ms | 63670.396372 ms | 128 | PASS |

<!-- CSR_RESULTS_START -->

## CSR Results

| Test File | Input Type | V | E | row_ptr | col_idx | values | CSR Time | Status |
|---|---|---:|---:|---:|---:|---:|---:|---|
| csr_test_01.txt | Unweighted | 10 | 30 | 11 | 30 | 0 | 0.006841 ms | PASS |
| csr_test_02.txt | Weighted | 10 | 30 | 11 | 30 | 30 | 0.010160 ms | PASS |
| csr_test_03.txt | Unweighted | 100 | 300 | 101 | 300 | 0 | 0.039777 ms | PASS |
| csr_test_04.txt | Weighted | 100 | 300 | 101 | 300 | 300 | 0.020020 ms | PASS |
| csr_test_05.txt | Unweighted | 10000 | 30000 | 10001 | 30000 | 0 | 0.971491 ms | PASS |
| csr_test_06.txt | Weighted | 10000 | 30000 | 10001 | 30000 | 30000 | 1.021938 ms | PASS |
| csr_test_07.txt | Unweighted | 50000 | 150000 | 50001 | 150000 | 0 | 3.001024 ms | PASS |
| csr_test_08.txt | Weighted | 50000 | 150000 | 50001 | 150000 | 150000 | 4.366332 ms | PASS |
| csr_test_09.txt | Unweighted | 100000 | 300000 | 100001 | 300000 | 0 | 6.784503 ms | PASS |
| csr_test_10.txt | Weighted | 100000 | 300000 | 100001 | 300000 | 300000 | 18.046337 ms | PASS |

<!-- CSR_RESULTS_END -->

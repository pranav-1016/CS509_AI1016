# Bellman-Ford Test Results

| Test File | Vertices | Edges | Source | Negative Cycle | Time | Status |
|---|---:|---:|---:|---|---:|---|
| bf_10.txt | 10 | 20 | 0 | No | 0.001603 ms | PASS |
| bf_100.txt | 100 | 250 | 0 | No | 0.006380 ms | PASS |
| bf_10000.txt | 10000 | 30000 | 0 | No | 1.047035 ms | PASS |
| bf_100000.txt | 100000 | 300000 | 0 | No | 6.113543 ms | PASS |
| bf_50000.txt | 50000 | 150000 | 0 | No | 2.579858 ms | PASS |
| bf_negative_cycle.txt | 5 | 5 | 0 | Yes | 0.000655 ms | PASS |
| bf_negative_edge.txt | 4 | 5 | 0 | No | 0.001008 ms | PASS |


# Floyd-Warshall Test Results

| Test File | Vertices | Edges | Negative Cycle | Time | Status |
|---|---:|---:|---|---:|---|
| fw_10.txt | 10 | 40 | No | 0.005076 ms | PASS |
| fw_100.txt | 100 | 1100 | No | 1.472896 ms | PASS |
| fw_1000.txt | 1000 | 11000 | No | 318.291221 ms | PASS |
| fw_2000.txt | 2000 | 22000 | No | 2412.811641 ms | PASS |
| fw_500.txt | 500 | 5500 | No | 46.794168 ms | PASS |
| fw_disconnected.txt | 5 | 8 | No | 0.003194 ms | PASS |

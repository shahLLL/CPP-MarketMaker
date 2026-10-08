# ▶️ ITCHMarketReplay
<div align="center">
  <img src="images/background.png" alt="Background" width="75%"/>
  <br><br>
</div>

# 👀 Overview
This is a C++ repository of an ITCH 5.0 feed handler that replays recorded NASDAQ data, reconstructs per symbol orderbooks and reports key metrics. This codebase has been developed using C++23 and parses files which contain data adhereing to the [NASDAQ 5.0 Specification.](https://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf)

This codebase has been throughly tested using the [Catch2](https://github.com/catchorg/Catch2) testing framework.
```
Randomness seeded to: 4199183415
===============================================================================
All tests passed (948 assertions in 43 test cases)
```

This codebase does represent a multi-threaded application and has been tested to make sure there aren't any data races via Thread Santizer.

It has been benchmarked on an [Apple M4](https://en.wikipedia.org/wiki/Apple_M4) memory chip and 16GB of memory and **AppleClang 17.0.0.17000013** C++ compiler to achieve the following results:
```
Itch Market Replay Benchmark (7000 ops)
Elapsed: 3.30775 ms
Throughput: 2.11624 M ops/sec
Latency (per op, including chrono overhead ~10 ns):
p50 : 208 ns
p75 : 209 ns
p90 : 250 ns
p99 : 584 ns
p99.9: 3375 ns
```

The following optimisations and techniques have been used to achieve the above results:
- **Use of Final Keyword/Specifier:** Classes and Structs are marked as final, preventing inheritance and enabling the compiler to improve performance through devirtualization.
- **File Parsing:** `mmap` library used to achieve performance improvement by mapping a file directly into a process's virtual memory address space.
- **Big Endian to Little Endian Conversion:** NASDAQ numerical format converted to machine via bit manipulation/operations.
- **SPSC Queue:** Single-Producer Single-Consumer Queue used to enable efficient multi-threaded communication by eliminating kernel context switches, lock contention, and heavy synchronization overhead.
- **Atomic Library:** C++ Library used in SPSC Queue to provide efficient lock-free thread safety.
- **BitOp Increment**: Bit Operations - in conjunction with power of 2 array capacity - used to effeciently increment cursors in the SPSC Queue.
- **Alignas Specifier:** Used to pad variables in SPSC Queue and prevent false sharing in caches.
- **Cached Cursors:** Used to optimise SPSC Queue by drastically reducing the amount of atomic load operations performed.
- **Array-Based Price Ladder:** Used in orderbook instead of a Red-Black Tree. Removes the need for dynamic allocation on hot paths and reduces cache misses by providing sequential memory access.
- **BitMap and Bit Operations:** Used in orderbook, allows for more effecient access to next best bid or ask.
- **Best Bid/Ask Cursors:** Used in orderbook, allows for O(1) access to both best bid and ask prices.
- **Ordered Map:** Used in orderbook, provides effecient O(1) order lookup and delete.


# 📈 Metrics
After reconstructing the orderbook, the following metrics are calculated and reported per symbol/stock:
- **Best Bid:** The value in cents of the best bid resting.
- **Best Ask:** The value in cents of the best ask resting. 
- **Number of Bid Orders:** The total number of Bid Orders in the orderbook. 
- **Number of Ask Orders:** The total number of Ask Orders in the orderbook.
- **MidPrice:** The arithmetic average of the best bid and best ask prices in cents.
- **MicroPrice:** The size-adjusted fair value estimate in cents. Leans toward the thinner side of the book where prices are more likely to move.
- **Imbalance:** The ratio or disparity between buy-side (bid) and sell-side (ask) volume. Signals short-term directional price pressure

# ⚒️ Build & Usage

This project uses CMake(Version 3.12) as a build tool.
In order to build the project using CMake:
```
cmake -B <BUILD_DIR>
cmake --build <BUILD_DIR>
```

The following executables are produced:
- **itchreplay:** Main script, parses ITCH data and generates report. Built using `PRIVATE -O3 -march=native`.
- **debug:** Main script, built using **Thread Sanitizer.** Used to check for data races.
- **unit_tests:** Unit tests.
- **bench:** Bench script, used to gather and report bench metrics. Built using `PRIVATE -O3 -march=native`.
- **bench-debug:** Bench Script, built using **Thread Sanitizer.** Used to check for data races.

As a note, ITCH data used in the sample run below is being kept private. Users must provide their own ITCH data to **itchreplay**, **debug**, **bench**, and **bench-debug** executables.

```
"Usage: itchreplay <inputfile>"
"Usage: debug <inputfile>"
"Usage: bench <inputfile>"
"Usage: bench-debug <inputfile>"
```

# 🏃‍♂️ Sample Run
```
----------------------------
STOCK: TSLA    
STOCK LOCATE: 3
BEST BID: 110000
BEST ASK: 110100
NUMBER OF BID ORDERS: 467
NUMBER OF ASK ORDERS: 470
MIDPRICE: 110050
MICROPRICE: 110047
IMBALANCE: -0.0688178
----------------------------
----------------------------
STOCK: AMZN    
STOCK LOCATE: 2
BEST BID: 180000
BEST ASK: 180100
NUMBER OF BID ORDERS: 497
NUMBER OF ASK ORDERS: 464
MIDPRICE: 180050
MICROPRICE: 180052
IMBALANCE: 0.0402667
----------------------------
----------------------------
STOCK: MSFT    
STOCK LOCATE: 1
BEST BID: 250000
BEST ASK: 250100
NUMBER OF BID ORDERS: 571
NUMBER OF ASK ORDERS: 646
MIDPRICE: 250050
MICROPRICE: 250047
IMBALANCE: -0.063505
----------------------------
```

# 🍴 Forking & Contribution
Forking, Usage, and Contributions to this project are welcomed with adherence to:
**[LICENSE](./LICENSE).**
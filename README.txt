================================================================================
       Dual-Machine Network Acceleration Based on AVX, OpenMP, and TCP
================================================================================

OVERVIEW
--------
This project implements high-performance numerical computing functions (floating-
point array sum, max value, and sorting) using various acceleration techniques
including SIMD (AVX instructions), multi-threading (OpenMP), and distributed
computing via TCP protocol.

The goal is to maximize computational performance by leveraging the combined
processing power of two networked computers.


PROJECT FILES
-------------
The following files contain main entry points. Add them to your project as needed:

  localSpeedUpTest.cpp  - Single-machine acceleration test (all three functions)
  localTest.cpp         - Single-machine non-accelerated baseline test
  server.cpp            - Dual-machine acceleration test (server/master node)
  client.cpp            - Dual-machine acceleration test (client/slave node)


IMPLEMENTATION STRATEGIES
-------------------------
1. Single-Machine (No Acceleration)
   - Serial CPU execution
   - Merge sort algorithm for sorting
   - Block-based summation for floating-point precision

2. Single-Machine (Accelerated)
   - AVX instruction set: processes 8 floats per instruction
   - OpenMP multi-threading: parallel computation across CPU cores
   - Parallel merge sort algorithm

3. Dual-Machine (Distributed)
   - TCP protocol for reliable data transmission
   - Each machine initializes and processes half of the data
   - Results are merged on the server after parallel computation


ENVIRONMENT REQUIREMENTS
------------------------
- Operating System: Windows 10/11
- Development Tool: Visual Studio 2022 Community (or later)
- Compiler: MSVC with C++17 support
- Processor: Intel CPU with AVX support (e.g., Intel Core i7)


BUILD CONFIGURATION
-------------------
1. Set build mode to "Release" for optimized performance testing
2. Enable OpenMP support:
   - Project Properties -> C/C++ -> Language -> OpenMP Support: Yes
3. For LLVM compiler (optional):
   - Properties -> Linker -> Command Line: add "/openmp:llvm"
4. For dual-machine testing:
   - Modify the IP address and port macros in server.cpp and client.cpp
   - Ensure both programs use the same DATANUM (data size) constant


USAGE
-----
Single-Machine Acceleration Test:
  1. Build and run localSpeedUpTest.cpp
  2. Compares accelerated vs. non-accelerated performance
  3. Outputs results and speedup ratios for sum, max, and sort

Single-Machine Baseline Test:
  1. Build and run localTest.cpp
  2. Measures baseline performance without acceleration

Dual-Machine Test:
  1. Configure network settings (IP address and port)
  2. Build server.cpp and client.cpp on respective machines
  3. Run server.exe on the master machine first
  4. Run client.exe on the slave machine
  5. Server displays final results and total execution time


PERFORMANCE RESULTS (Release Mode)
----------------------------------
Single-Machine Acceleration (average of 5 runs):

  Function    Speedup Ratio
  --------    -------------
  Sum         19.23x faster (accelerated vs. non-accelerated)
  Max         34.34x faster
  Sort         2.27x faster

Dual-Machine Acceleration:
  - Total speedup ratio: 2.92x (compared to single-machine non-accelerated)
  - Note: Network communication overhead is significant for this data size
  - Better performance gains expected with larger datasets


TECHNICAL HIGHLIGHTS
--------------------
- Kahan summation alternative: Block-based recursion for float precision
- AVX intrinsics: _mm256_add_ps, _mm256_max_ps, _mm256_log_ps, _mm256_sqrt_ps
- TCP packet size: 4MB per packet for optimal throughput
- Thread-private variables to avoid shared memory conflicts


NOTES
-----
- Data size: 64 * 2,000,000 floats (allocated on heap due to stack limits)
- The initial data is sequentially ordered, which affects algorithm selection
  (merge sort preferred over quicksort to avoid O(n^2) worst case)
- For dual-machine mode, ensure firewall allows TCP connections


AUTHORS
-------
Zhang Guohua 
Liu Roushan

================================================================================

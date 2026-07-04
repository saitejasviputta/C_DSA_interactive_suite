# Benchmarking Methodology

This directory contains the benchmarking and profiling suite for the C DSA Interactive Suite.

## Overview
The goal of this benchmarking suite is to provide a reliable, stable, and reproducible performance profiling tool for all data structures and algorithms across different platforms (Linux, macOS, Windows) and different commits.

## Statistical Methodology
To ensure that measurements are consistent, reliable, and directly comparable across commits, the following methodologies are implemented:

1. **Fixed Random Seed (`BENCHMARK_SEED`)**
   - We use a standardized fixed seed (`12345`) for all random input generation instead of non-deterministic methods (like using the system clock). This guarantees that the generated datasets are identical on every benchmark run of the same size $N$.

2. **Multiple Iterations (`BENCHMARK_DEFAULT_ITERATIONS`)**
   - Each algorithm is benchmarked over multiple iterations (defaulting to `5` iterations). This helps absorb transient spikes in CPU scheduling or background OS activities.

3. **Mean and Standard Deviation Reporting**
   - We measure and report both the **Mean** execution time and the **Standard Deviation** (representing timing variance) over the iterations.
   - The reported standard deviation is calculated using standard formulas:
     $$\mu = \frac{1}{N} \sum_{i=1}^{N} x_i$$
     $$\sigma = \sqrt{\frac{1}{N-1} \sum_{i=1}^{N} (x_i - \mu)^2}$$
   - To avoid cross-platform math library (`-lm`) linking issues, the square root ($\sqrt{\cdot}$) is computed dynamically using a highly precise Babylonian (Newton-Raphson) approximation.

4. **Peak Memory Consumption Tracking**
   - We track the peak resident set size (RSS) of the process during the runs on both POSIX systems and Windows (using Windows API `GetProcessMemoryInfo`).

## Standardized CLI/TUI Layout
The table outputs are standardized to:
* **Category Name & Input Size $N$**
* **Columns**: `Algorithm` (30 chars width), `Execution Time` (mean ± stddev), `Peak Memory` (KB), and `Status`.

## CSV Exports
Every run appends a data row containing the algorithm name, input size, mean execution time, peak memory consumption, and a human-readable timestamp to `benchmarks/<category>.csv` for post-run processing or profiling plots.

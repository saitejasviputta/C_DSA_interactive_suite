

# C_DSA_interactive_suite

A modular, console-based **Data Structures & Algorithms library** written entirely in **C**, built from scratch with pointer-level control, manual memory management (`malloc` / `free`), and defensive input validation.

This project emphasizes **conceptual clarity**, **low-level fundamentals**, and **explicit memory reasoning**. It is designed with an educational intent, allowing learners to observe, experiment with, and understand data structures and algorithms step-by-step through an interactive terminal-based interface.

The codebase is structured as a reusable **DSA library**, with an interactive, console-driven **demo layer** built on top.

---

## Table of Contents
- [Demos](#demos)
- [Build Instructions](#build-instructions-recommended)
- [Continuous Integration](#continuous-integration)
- [Time Complexity](#time-complexity)
- [License](#license)

---

## Demos

## Backtracking algos (Knight's tour)

[![asciinema demo](https://asciinema.org/a/G5D8YBvcIbZM2fzF.svg)](https://asciinema.org/a/G5D8YBvcIbZM2fzF)

## Sorting algos (Bubble sort)

[![asciinema demo](https://asciinema.org/a/J8CHAuyJAIyOduYY.svg)](https://asciinema.org/a/J8CHAuyJAIyOduYY)

## Expression Evaluation complete

[![asciinema demo](https://asciinema.org/a/xDm8zlxD6VzzFZFR.svg)](https://asciinema.org/a/xDm8zlxD6VzzFZFR)

## String algos (Robin Karp)

[![asciinema demo](https://asciinema.org/a/d2BU9AVDS94GVovS.svg)](https://asciinema.org/a/d2BU9AVDS94GVovS)


---

## Build Instructions

This project includes a **Makefile** and **CMakeLists.txt** to simplify building across multiple directories.

### Requirements
- GNU Make ≥ 4.4.1
- GCC (or a compatible C compiler)

## TUI Requirements

The Text User Interface (TUI) is built using the **Ncurses** library.

### Ubuntu / Debian
```bash
sudo apt install libncurses5-dev libncursesw5-dev
```
### Fedora
```bash
sudo dnf install ncurses-devel
```
### Arch Linux
```bash
sudo pacman -S ncurses
```
> **Note:** The TUI is supported on Unix/Linux systems. On Windows, the project automatically falls back to the legacy CLI interface.

### Build (Makefile)
```bash
make
```
This generates a single executable:
* `dsa` (Linux / macOS)
* `dsa.exe` (Windows)

### Build (CMake)
Alternatively, you can compile the application and tests using CMake:
```bash
mkdir build && cd build
cmake ..
make
```

To execute all unit tests using CTest:
```bash
ctest --output-on-failure
```


```bash
make run 
```
Builds only when necessary and launches the program.


```bash
make test
```
Runs all tests and generates test binaries

```bash
make fmt
```
Organizes code style according to the standards defined in `.clang-format`

```bash
make valgrind
```
Runs Valgrind over test binaries to look for memory leaks / use after free errors


### Clean

```bash
make clean
```
Removes executables and generated object/test binaries.

---

## Architectural Breakdown: Docker & The Build System

### Why Docker?

Docker acts as a cross-platform wrapper around the build system. Contributors on Windows, macOS, and Linux can use the same isolated Ubuntu environment without manually configuring compiler toolchains, build dependencies, or platform-specific settings.

### Container Build Chain

The current build flow is:

```text
Docker Container
      ↓
   Makefile
      ↓
GCC Compilation
      ↓
dsa Executable
```

The Docker image installs the required build tools and executes the project's Makefile, ensuring consistent builds across different operating systems.

### Relationship Between Docker, Makefile, and CMake

Each component serves a different purpose:

- Docker provides a reproducible Linux build environment.
- The Makefile defines the primary build workflow used by the project today.
- CMakeLists.txt provides an alternative build system that can generate platform-specific build files while supporting testing and future expansion.

These tools are complementary rather than competing solutions.

---

## Continuous Integration

[![CI](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml/badge.svg)](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml)

This project includes a **GitHub Actions CI pipeline** that automatically verifies code correctness and memory safety.

On every push or pull request:

1. A fresh **Ubuntu VM** is allocated
2. The project is **compiled using GCC**
3. The complete **unit test suite is executed**
4. All test binaries are run under **Valgrind** to check for:

   - memory leaks  
   - invalid reads / writes  
   - use-after-free errors  
   - uninitialized memory usage

If any test fails or Valgrind detects a memory error, the CI job fails automatically.


## Time Complexity

### Searching Algorithms
* Linear Search: **O(n)**
* Binary Search: **O(logn)**
* Interpolation Search: **O(log(logn))**
* Jump Search: **O(√n)**

### Sorting Algorithms
* Bubble Sort: **O(n²)**
* Selection Sort: **O(n²)**
* Insertion Sort: **O(n²)**
* Shell sort: **O(nlogn)**

### Advanced Sorting Algorithms
* Quick sort: **O(n²)**
* Merge sort: **O(nlogn)**
* Heap sort: **O(nlogn)**
* Radix sort (LSD): **O(nk)**
* Bucket sort :**O(n+k)**

### Graph Traversals (Adjacency List)
* GBFS: **O(b^m)**
* BFS: **O(V+E)**
* DFS: **O(V+E)**
* Dijkstra's Algorithm: **O((V+E)log V)**
* A* Search: **O((V+E)log V)**
* Greedy Best-First Search: **O((V+E)log V)**
* Bellman-Ford: **O(V·E)**
* Floyd-Warshall: **O(V³)**
* Topological Sort (Kahn's Algorithm): **O(V+E)**

### Job scheduling
* FCFS(First come,First Served): **O(n)**
* SJF( Shortest Job First): **O(nlogn+n)**
* Priority Scheduling: **O(nlogn)**
* SRTF (Shortest Remaining Time First): **O(nlogn)**
* Preemptive Priority Scheduling: **O(nlogn)**

### String Algorithms
* Naive String Matching: **O(n-m+1)**
* Knuth-Morris-Pratt (KMP) Algorithm: **O(n+m)**
* Rabin-Karp Algorithm: **O(n.m)**

### Dynamic Programming
* Fibonacci (DP): **O(n)**
* Longest Common Subsequence (LCS): **O(nm)**
* 0/1 Knapsack: **O(nW)**
* Matrix Chain Multiplication: **O(n³)**

### Backtracking Algorithms
* Rat in a Maze: **O(4^(n²))** 
* Graph Coloring: **O(m^V)**
* Sudoko solver: **O(6^m)**
* Knight's tour:**O(8^(n^2))**
* N Queen's algorithm: **O(N!)**





## License
This project is licensed under the MIT License - see the LICENSE file for details.

---

## Author

**Darshan Parekh** and many contributors....

Aspiring systems engineer and cybersecurity engineer

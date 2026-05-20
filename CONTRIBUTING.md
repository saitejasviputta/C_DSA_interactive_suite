# Contributing to C_DSA_interactive_suite

Thank you for taking the time to contribute! This project is open to learners, systems enthusiasts, and low-level programmers who want to build something meaningful in C. You dont need deep technical knowledge, as this project is made with raw C, you will only need algorithmic thinking and logical thinking. Thats it.

---

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Branch Naming Convention](#branch-naming-convention)
- [Commit Message Guidelines](#commit-message-guidelines)
- [Coding Standards](#coding-standards)
- [Adding a New Module](#adding-a-new-module)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)

---

## Code of Conduct

<!-- Add your code of conduct or link to a CODE_OF_CONDUCT.md -->
Be civil and respectful, thats it, no more demands, just dont be sexist or racist and you are good. Also dont spam "any updates" in the PR thread, wait for 2-3 days before a follow up.

---

## Getting Started

### Note for SSOC contributors

This project is Linux native. Which means it is made, maintained on Linux and works the best on Linux, however, you can definitely develop and run/use this application on windows through WSL(Windows Subsystem for Linux). WSL will work in VScode too so you can rest assured, just be familiar with WSL before contributing to this project. You can also use a VM, but WSL is sufficient as per the project requirements

### Prerequisites

- GCC (or compatible C compiler)
- GNU Make ≥ 3.81
- Valgrind (for checking memory leaks)
- Git

### Fork & Clone

```bash
git clone https://github.com/<your-username>/C_DSA_interactive_suite.git
cd C_DSA_interactive_suite
```

### Build

```bash
make
./dsa
```

### Clean

```bash
make clean
```

---

## How to Contribute

You can contribute in the following ways:

- Fix an open issue or open an issue
- Implement a new data structure or algorithm
- Improve existing implementations (refactoring)
- Add or improve test cases
- Improve documentation

Please check the [Issues](https://github.com/darshan2456/C_DSA_interactive_suite/issues) tab before starting work to avoid duplicates. If you plan to work on something, comment on the issue to get it assigned.

---

## Branch Naming Convention

```
feature/<short-description>      # for new features or modules
fix/<short-description>          # for bug fixes
docs/<short-description>         # for documentation changes
test/<short-description>         # for adding/improving tests
```

same as commit conventions, whatever you are doing, followed by slash followed by brief one line description 

Example: `feature/avl-tree`, `fix/bst-delete-edge-case`

---

## Commit Message Guidelines

Use clear, concise commit messages in the following form:

```
feat: Add AVL tree insertion with rotation logic
fix: Fix memory leak in DLL delete_node
refactor: Refactor hash table to use safe_input_int API
docs: Fix typo in README.md
```

Avoid vague messages like `fix stuff` or `update code`.

---

## Coding Standards

This project is written in **C11**. All contributions must follow the existing style:

- Format your code using `.clang-format` before submitting — run `clang-format -i <file>`
- One `.h` / `.c` pair per logical module — no exceptions
- No function definitions inside header files
- Prefix all public functions with the module name (e.g., `bst_insert`, `sll_delete`)
- Use `static` for all file-local helper functions which are not supposed to used elsewhere but where they are created
- Use `const` where pointer safety is appropriate ie functions which do not modify data but only read data, like sll_printlist(), sll_search() functions
- All user input must go through `safe_input_int()` — no raw `scanf`
- Compile cleanly under `-Wall -Wextra -Werror -std=c11` so even warning are treated as errors. It helps to avoid accumulation of dead code.

---

## Adding a New Module

<!-- Describe your preferred directory structure, how to hook into the main menu, and any naming conventions specific to this project -->

Example structure for a new module:

```
src/
└── your_module/
    ├── your_module.h
    ├── your_module.c
    └── your_module_demo.c
```

Steps:
1. Create the module directory under `src/`
2. Implement the core logic in `your_module.c`
3. Expose the public API via `your_module.h`
4. Write the interactive demo in `your_module_demo.c` (refer existing demos and respect and keep the style)
5. Hook it into the main menu
6. Update the `Makefile` to include the new module
7. Add corresponding tests under `tests/`

---

## Testing

<!-- Describe your test structure, how to run tests, and what is expected -->

All contributions which introduces a new module (ds/algo) must include tests:

```bash
# Run all tests
make test

# Run with Valgrind
valgrind --leak-check=full ./test_binary
```

The CI pipeline runs all tests and Valgrind checks automatically on every push and pull request. A PR will not be merged if CI fails.

---

## Pull Request Process

1. Fork the repository and create your branch from `main`
2. Make your changes following the **coding standards** above
3. Ensure all tests pass and Valgrind reports with no errors
6. Link the relevant issue using `Closes #<issue-number>`

PRs without tests(in case of new ds/algo), failing CI, or unformatted code will not be merged.

---

## Reporting Issues

When opening an issue, please include:

- A clear title and description
- Steps to reproduce (if it's a bug)
- Expected vs actual behavior
- Your OS and GCC version

Use the appropriate label: `bug`, `enhancement`, `documentation`, `good first issue`, `refactor` etc.

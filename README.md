# Optimization-of-Low-Latency-Boolean-Functions
This repository contains the source codes of optimized implenentation model for boolean functions towards low-latency, and area optimization from boolean functions to an S-box, which is described or founded in the paper "A New Optimized Implementation Method for Boolean Functions Towards Low-Latency--Application to AES S-Box", TCHES Volume 2026/3.

## Required Software
This model establishes constraints based on the satisfiability problem to search for whether an S-box exists under specific constraints. The required software is the SMT-based STP solver, which can be run on Linux or Mac OS. STP solver supports solving using the CVC language, which based on the Conjunctive Normal Form (CNF).  The necessary sources for the STP solver are shown below.
 * Homepage: [https://stp.github.io/](https://stp.github.io/)
 * Source Code: [https://github.com/stp/stp](https://github.com/stp/stp)
 * Syntax Rules of CVC Language: [https://stp.readthedocs.io/en/stable/cvc-input-language.html](https://stp.readthedocs.io/en/stable/cvc-input-language.html)

## AES S-box Example
The `Codes` folder provides an example of constructing the automatic search model for the AES S-box coordinate Boolean function **f2**.

* `main.cpp` generates the f2 CVC model with a cost bound of 34 and its negation-pattern coverage matrix.
* `Automatic Search Model/` contains the generated f2 `.cvc` model and saved `.txt` solver outputs for AES S-box coordinate functions f0–f7.
* `Negation_pattern_AES/` contains the corresponding coverage matrices for f0–f7 in CSV format.

Compile and run from the `Codes` directory so that the relative output paths resolve correctly:

```sh
cd Codes
g++ main.cpp
./a.out
```

On Windows, use g++ in the MSYS2 UCRT64 environment and run `./a.exe` instead of `./a.out`. Running the example regenerates `Automatic Search Model/anf_negpattern_global_AES_8bit_f2.cvc` and `Negation_pattern_AES/coverage_matrix_patterns_AES_8bit_f2.csv`; the saved solver outputs are provided separately.

# Optimization-of-Low-Latency-Boolean-Functions
This repository contains the source codes of optimized implenentation model for boolean functions towards low-latency, and area optimization from boolean functions to an S-box, which is described or founded in the paper "A New Optimized Implementation Method for Boolean Functions Towards Low-Latency--Application to AES S-Box", TCHES Volume 2026/3.

## Required Software
This model establishes constraints based on the satisfiability problem to search for whether an S-box exists under specific constraints. The required software is the SMT-based STP solver, which can be run on Linux or Mac OS. STP solver supports solving using the CVC language, which based on the Conjunctive Normal Form (CNF).  The necessary sources for the STP solver are shown below.
 * Homepage: [https://stp.github.io/](https://stp.github.io/)
 * Source Code: [https://github.com/stp/stp](https://github.com/stp/stp)
 * Syntax Rules of CVC Language: [https://stp.readthedocs.io/en/stable/cvc-input-language.html](https://stp.readthedocs.io/en/stable/cvc-input-language.html)

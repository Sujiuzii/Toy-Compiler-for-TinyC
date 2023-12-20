# Toy-Compiler-for-TinyC

## Introduction

Lab of USTC_FALL_033001

Compiler for a tiny-version C language. 

## TinyC

- Supports
  - `while` loop
    - Does not support `break` or `continue`
  - `if-else` structures
  - Data types
    - Integer
  - Logical operators
    - `&&` (AND)
    - `||` (OR)
    - `==` (equals)
    - `!=` (not equals)
    - `<` (less than)
    - `>` (greater than)
  - Arithmetic operations
    - `+` (addition)
    - `-` (subtraction)
    - `*` (multiplication)
    - `/` (division)


## Options

- Capabilities
  - Print out
    - Lexical analysis token table
    - Syntax analysis tree
    - Intermediate code
- Compiler Limitation
  - Supports only up to
    - Intermediate code generation stage

## Get started
```bash
git clone https://github.com/Sujiuzii/Toy-Compiler-for-TinyC.git
cd Toy-Compiler-TinyC
make
./main
```

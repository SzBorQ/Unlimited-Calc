# Unlimited Calc

An arbitrary-precision (bignum) calculator and numeral system converter written in C. It performs operations on arbitrarily large integers across different number bases (from base 2 up to base 16) using file-based inputs.

---

## Features

- **Arbitrary Precision:** Overcomes standard integer size limits by performing arithmetic on digit strings.
- **Supported Bases:** Base 2 up to Base 16 (digits `0`–`9` and `A`–`F`).
- **Operations:**
  - Arithmetic: Addition (`+`), Multiplication (`*`), Integer Division (`/`), Modulo (`%`), Exponentiation (`^`).
  - Base Conversion: Direct conversion between arbitrary bases (`<base_in> <base_out>`).
- **Batch File Processing:** Evaluates multiple sequential operations and expressions defined in an input file.

---

## Building

Requires a C compiler supporting C11 (such as `gcc`) and `make`.

```bash
make
```
The compiled binary will be placed in bin/unlimited_calc.

To clean build artifacts:

```bash
make clean
```
## Usage
```bash
./bin/unlimited_calc <input_file> [output_file]
```
If [output_file] is omitted, the program creates an output file named out_<input_file> in the working directory.

## Input File Format
The input file consists of blocks starting with an operation header followed by numbers on separate lines. Blocks are separated by empty lines.

1. Arithmetic Operations

Header syntax: <operator> <base>
```text
+ 10
123
456

^ 10
2
10
```

2. Base Conversion

Header syntax: <source_base> <target_base>

```text
16 10
DEAD

2 10
1010
```



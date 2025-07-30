# Stack Smashing Demonstration

## Overview

This project demonstrates a classic **stack smashing** (stack buffer overflow) vulnerability in C++. It includes a vulnerable function that allows controlled overwriting of the return address, illustrating how an attacker could hijack control flow and execute arbitrary code.

The demo provides multiple execution modes to explore safe behavior, manual input overflow, and an automatic crafted payload attack.

---

## Problem Description

**Stack smashing** is a type of memory corruption vulnerability that occurs when a program writes more data to a stack-allocated buffer than it can hold. This overflow can overwrite adjacent memory, including critical control data like the **saved return address**.

If the return address is overwritten with a crafted value, the program will jump to an unintended memory location — often shellcode or malicious code injected by the attacker.

### When Does Stack Smashing Occur?

Stack smashing typically happens when:

* A program uses unsafe functions like `strcpy`, `gets`, or `sprintf` without bounds checking.
* User input is directly copied into a fixed-size stack buffer without validation.
* Stack protection mechanisms (e.g., stack canaries, ASLR) are disabled or bypassed.

### Why Is It Dangerous?

Stack smashing can allow an attacker to:

* Hijack control flow (e.g., redirect execution to malicious code).
* Elevate privileges (e.g., spawn a shell or run system commands).
* Crash the program (denial of service).
* Bypass authentication or security checks.

---

## Explaining Key Concepts

### What Is a Stack Canary?

A **stack canary** is a security mechanism used to detect buffer overflows before the function returns. It works by placing a known random value (the "canary") just before the saved return address on the stack. If a buffer overflow corrupts the canary, the program detects it and aborts execution.

Modern compilers can enable this feature via flags like `-fstack-protector`.

### How to Prevent Stack Smashing

To prevent such vulnerabilities:

* **Never use unsafe functions** like `strcpy`, `gets`, etc.
* Always use **safe alternatives**: `strncpy`, `fgets`, `snprintf`, etc.
* Enable **compiler protections**:

  * Stack canaries (`-fstack-protector-strong`)
  * Address Space Layout Randomization (ASLR)
  * Non-executable stack (`-z noexecstack`)
* Use tools like **ASAN**, **Valgrind**, or **Clang Static Analyzer** to detect overflows.

---

## Example Output

```
Function addresses:
  main               : 0x564cb1f7b23b
  safe_function      : 0x564cb1f7b195
  secret             : 0x564cb1f7b0f0
  vulnerable_function: 0x564cb1f7b1d0

Stack Exploitation Lab
======================
Choose scenario:
1. Safe function
2. Vulnerable function (input from stdin)
3. Vulnerable function (crafted payload)

Select mode: 3
Using crafted payload (80 bytes)...
Copying input (80 bytes) into buffer[64]...
Done copying. Buffer content: AAAAAAAAAAAA...
uid=1000(user) gid=1000(user) groups=1000(user)
```

---

## Explanation of Output

In **mode 3**, the program constructs a specially crafted input that:

1. Fills the buffer (64 bytes)
2. Overwrites the saved RBP (8 bytes)
3. Replaces the return address with the address of `secret()`

When `vulnerable_function` returns, execution jumps to `secret()` and runs `/usr/bin/id`, demonstrating successful hijacking of control flow.

---

## How to Compile and Run

### 1. Clone the Repository

```bash
git clone https://github.com/LyudmilaKostanyan/Stack-Smashing.git
cd Stack-Smashing
```

### 2. Build the Project

Use `CMake` to build:

```bash
cmake -S . -B build
cmake --build build
```

**Compiler flags to disable protections** (for demonstration only):

* `-fno-stack-protector` — disables stack canary
* `-z execstack` — makes stack executable
* `-no-pie` — disables position-independent executables

To pass these manually:

```bash
cmake -DCMAKE_CXX_FLAGS="-fno-stack-protector -z execstack -no-pie" -S . -B build
cmake --build build
```

> Ensure you are compiling on a Linux system with `g++`.

### 3. Run the Program

#### Linux/macOS

```bash
cd build
./main
```

Choose one of the options:

* `1`: Run safe function
* `2`: Provide your own input to test overflow
* `3`: Run with automatically crafted payload to hijack control flow

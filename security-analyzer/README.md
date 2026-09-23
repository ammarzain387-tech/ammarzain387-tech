# C++ Security Analyzer

A dependency-free, defensive static analyzer for common security mistakes in C/C++ source trees. It is intentionally a **scanner**, not an exploit tool: it reads files, reports findings, and never executes project code.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Usage

```bash
./build/security-analyzer --path ./src
./build/security-analyzer --path . --format json --extensions cpp,h,cc,hpp
./build/security-analyzer --help
```

Exit status is `0` when no findings are reported and `1` when findings exist. Invalid arguments or filesystem errors return `2`, which makes the tool suitable for CI.

## Checks

- **SEC001** hard-coded passwords, API keys, tokens, and private keys
- **SEC002** unsafe C string functions such as `strcpy` and `sprintf`
- **SEC003** shell command construction or execution with non-constant input
- **SEC004** SQL statements built by string concatenation
- **SEC005** weak or obsolete cryptography (`MD5`, `SHA1`, `DES`, `RC4`)
- **SEC006** insecure temporary-file patterns (`/tmp`, `mktemp`)
- **SEC007** disabled TLS certificate verification

The analyzer skips hidden directories and common generated/vendor/build directories. Findings include a stable rule ID, severity, confidence, file, line, source text, and remediation.

This tool is a lightweight first pass. It is not a replacement for peer review, compiler warnings, sanitizers, dependency auditing, or a mature SAST platform.

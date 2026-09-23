# Industrial Log Sentinel

A lightweight C++ log-monitoring utility for parsing industrial system logs and identifying operational events that may require attention.

## Project Structure

```text
industrial-log-sentinel/
├── src/
│   ├── LogParser.cpp   # Log parsing implementation
│   ├── LogParser.hpp   # Log parser interface
│   └── main.cpp        # Application entry point
└── README.md
```

## Features

- Clear separation between application entry-point and parsing logic
- C++ implementation suitable for embedded and industrial monitoring workflows
- Extensible parser design for additional log formats and event types
- Minimal dependency footprint

## Requirements

- A C++17-compatible compiler
- CMake 3.16 or newer (recommended)

Supported toolchains include GCC, Clang, and Microsoft Visual C++.

## Build

The source files can be compiled directly with a C++17 compiler:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 \
    src/main.cpp src/LogParser.cpp \
    -o industrial-log-sentinel
```

Run the application with:

```bash
./industrial-log-sentinel
```

> On Windows, use `industrial-log-sentinel.exe` and invoke it from PowerShell or Command Prompt.

## Development Guidelines

- Keep parsing responsibilities inside `LogParser`.
- Prefer standard-library facilities and avoid unnecessary dependencies.
- Validate input and handle malformed log records safely.
- Compile with warnings enabled during development.
- Add tests for new log formats, edge cases, and error conditions.

## Error Handling and Safety

Log input should be treated as untrusted data. The parser should fail gracefully when records are incomplete, malformed, or contain unexpected fields. Operational failures should provide actionable diagnostics without interrupting monitoring of subsequent records.

## Roadmap

- Add a CMake build configuration
- Define supported log formats and event severity levels
- Add automated unit and integration tests
- Provide configurable input and output sources
- Add structured output for integration with monitoring systems

## License

No license has been specified yet. Add a `LICENSE` file before distributing or reusing this project.

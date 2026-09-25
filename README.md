# Robot Control and Path Planning in C++

This project demonstrates a basic autonomous robot system that can:

- Represent a 2D environment as a grid
- Plan a collision-free path using the A* algorithm
- Move a robot toward each path waypoint
- Avoid blocked cells
- Display the planned path in the console

## Requirements

- C++17 or newer
- A compiler such as GCC, Clang, or MSVC

## Build and Run

```bash
g++ -std=c++17 -O2 -Wall -Wextra robot_planner.cpp -o robot_planner
./robot_planner


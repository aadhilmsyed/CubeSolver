# Rubik's Cube Simulator

A Qt-based Rubik's Cube simulator application that allows users to interact with and manipulate a 3D Rubik's Cube.

## Prerequisites

- CMake (version 3.10 or higher)
- Qt6
- C++17 compatible compiler
- OpenGL support

## Installation

### macOS (using Homebrew)

1. Install the required dependencies:
```bash
brew install cmake
brew install qt@6
```

2. Clone the repository:
```bash
git clone <repository-url>
cd CubeSolver
```

3. Create and navigate to the build directory:
```bash
mkdir build
cd build
```

4. Configure and build the project:
```bash
cmake ..
make
```

## Project Structure

```
CubeSolver/
├── src/
│   ├── main.cpp
│   ├── cube.cpp
│   ├── cube.h
│   ├── cuberenderer.cpp
│   ├── cuberenderer.h
│   ├── mainwindow.cpp
│   └── mainwindow.h
├── CMakeLists.txt
└── README.md
```

## Building from Source

The project uses CMake as its build system. The minimum required version is 3.10. The build process will automatically handle Qt dependencies and configure the necessary build files.

### Build Steps

1. Make sure you have all prerequisites installed
2. Create a build directory:
```bash
mkdir build && cd build
```

3. Generate build files:
```bash
cmake ..
```

4. Build the project:
```bash
make
```

## Troubleshooting

### Common Issues

1. **CMake not found**
   - Make sure CMake is installed and in your system PATH
   - On macOS: `brew install cmake`

2. **Qt6 not found**
   - Ensure Qt6 is properly installed
   - On macOS: `brew install qt@6`

3. **Build errors**
   - Make sure you have a C++17 compatible compiler
   - Verify that all dependencies are properly installed
   - Check that OpenGL is available on your system

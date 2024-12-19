#include "cube.h"
#include <algorithm>
#include <random>

// Define the initial state of the cube
Cube::Cube() {
    // Initialize each face with its center color
    const Color centerColors[6] = {
        Color::GREEN,   // Front (0)
        Color::BLUE,    // Back  (1)
        Color::ORANGE,  // Left  (2)
        Color::RED,     // Right (3)
        Color::WHITE,   // Up    (4)
        Color::YELLOW   // Down  (5)
    };
    
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < 9; i++) {
            faces[face][i] = centerColors[face];
        }
    }
}

void Cube::rotateFaceClockwise(Face face) {
    auto& f = faces[static_cast<int>(face)];
    Color temp[9];
    std::copy(f.begin(), f.end(), temp);
    
    // Rotate corners
    f[0] = temp[6];
    f[2] = temp[0];
    f[8] = temp[2];
    f[6] = temp[8];
    
    // Rotate edges
    f[1] = temp[3];
    f[5] = temp[1];
    f[7] = temp[5];
    f[3] = temp[7];
}

void Cube::F() {
    rotateFaceClockwise(Face::FRONT);
    
    const int edges[12] = {
        // RIGHT left edge
        static_cast<int>(Face::RIGHT) * 9 + 0,
        static_cast<int>(Face::RIGHT) * 9 + 3,
        static_cast<int>(Face::RIGHT) * 9 + 6,
        // UP bottom edge
        static_cast<int>(Face::UP) * 9 + 6,
        static_cast<int>(Face::UP) * 9 + 7,
        static_cast<int>(Face::UP) * 9 + 8,
        // LEFT right edge
        static_cast<int>(Face::LEFT) * 9 + 8,
        static_cast<int>(Face::LEFT) * 9 + 5,
        static_cast<int>(Face::LEFT) * 9 + 2,
        // DOWN top edge
        static_cast<int>(Face::DOWN) * 9 + 2,
        static_cast<int>(Face::DOWN) * 9 + 1,
        static_cast<int>(Face::DOWN) * 9 + 0
    };
    
    rotateEdges(Face::FRONT, edges);
}

void Cube::FPrime() {
    rotateFaceCounterClockwise(Face::FRONT);
    
    const int edges[12] = {
        // DOWN top edge
        static_cast<int>(Face::DOWN) * 9 + 2,
        static_cast<int>(Face::DOWN) * 9 + 1,
        static_cast<int>(Face::DOWN) * 9 + 0,
        // LEFT right edge
        static_cast<int>(Face::LEFT) * 9 + 8,
        static_cast<int>(Face::LEFT) * 9 + 5,
        static_cast<int>(Face::LEFT) * 9 + 2,
        // UP bottom edge
        static_cast<int>(Face::UP) * 9 + 6,
        static_cast<int>(Face::UP) * 9 + 7,
        static_cast<int>(Face::UP) * 9 + 8,
        // RIGHT left edge
        static_cast<int>(Face::RIGHT) * 9 + 0,
        static_cast<int>(Face::RIGHT) * 9 + 3,
        static_cast<int>(Face::RIGHT) * 9 + 6
    };
    
    rotateEdges(Face::FRONT, edges);
}

void Cube::F2() {
    F();
    F();
}

// Helper function to rotate edges
void Cube::rotateEdges(Face face, const int edges[12]) {
    Color temp[3];
    Color* flatFaces = reinterpret_cast<Color*>(faces.data());
    
    // Save first three edges
    for (int i = 0; i < 3; i++) {
        temp[i] = flatFaces[edges[i]];
    }
    
    // Rotate edges
    for (int i = 0; i < 9; i++) {
        flatFaces[edges[i]] = flatFaces[edges[i + 3]];
    }
    
    // Place saved edges
    for (int i = 0; i < 3; i++) {
        flatFaces[edges[i + 9]] = temp[i];
    }
}

Color Cube::getFaceColor(int face, int row, int col) const {
    return faces[face][row * 3 + col];
}

bool Cube::isSolved() const {
    // Check if each face has all the same color
    for (int face = 0; face < 6; face++) {
        Color centerColor = faces[face][4]; // Center piece
        for (int i = 0; i < 9; i++) {
            if (faces[face][i] != centerColor) {
                return false;
            }
        }
    }
    return true;
}

void Cube::B() {
    rotateFaceClockwise(Face::BACK);
    
    const int edges[12] = {
        // LEFT left edge
        static_cast<int>(Face::LEFT) * 9 + 0,
        static_cast<int>(Face::LEFT) * 9 + 3,
        static_cast<int>(Face::LEFT) * 9 + 6,
        // UP top edge
        static_cast<int>(Face::UP) * 9 + 2,
        static_cast<int>(Face::UP) * 9 + 1,
        static_cast<int>(Face::UP) * 9 + 0,
        // RIGHT right edge
        static_cast<int>(Face::RIGHT) * 9 + 8,
        static_cast<int>(Face::RIGHT) * 9 + 5,
        static_cast<int>(Face::RIGHT) * 9 + 2,
        // DOWN bottom edge
        static_cast<int>(Face::DOWN) * 9 + 6,
        static_cast<int>(Face::DOWN) * 9 + 7,
        static_cast<int>(Face::DOWN) * 9 + 8
    };
    
    rotateEdges(Face::BACK, edges);
}

void Cube::BPrime() {
    rotateFaceCounterClockwise(Face::BACK);
    
    const int edges[12] = {
        // LEFT left edge
        static_cast<int>(Face::LEFT) * 9 + 0,
        static_cast<int>(Face::LEFT) * 9 + 3,
        static_cast<int>(Face::LEFT) * 9 + 6,
        // DOWN bottom edge
        static_cast<int>(Face::DOWN) * 9 + 6,
        static_cast<int>(Face::DOWN) * 9 + 7,
        static_cast<int>(Face::DOWN) * 9 + 8,
        // RIGHT right edge
        static_cast<int>(Face::RIGHT) * 9 + 8,
        static_cast<int>(Face::RIGHT) * 9 + 5,
        static_cast<int>(Face::RIGHT) * 9 + 2,
        // UP top edge
        static_cast<int>(Face::UP) * 9 + 2,
        static_cast<int>(Face::UP) * 9 + 1,
        static_cast<int>(Face::UP) * 9 + 0
    };
    
    rotateEdges(Face::BACK, edges);
}

void Cube::B2() {
    B();
    B();
}

void Cube::L() {
    rotateFaceClockwise(Face::LEFT);
    
    const int edges[12] = {
        // FRONT left edge
        static_cast<int>(Face::FRONT) * 9 + 0,
        static_cast<int>(Face::FRONT) * 9 + 3,
        static_cast<int>(Face::FRONT) * 9 + 6,
        // UP left edge
        static_cast<int>(Face::UP) * 9 + 0,
        static_cast<int>(Face::UP) * 9 + 3,
        static_cast<int>(Face::UP) * 9 + 6,
        // BACK right edge
        static_cast<int>(Face::BACK) * 9 + 8,
        static_cast<int>(Face::BACK) * 9 + 5,
        static_cast<int>(Face::BACK) * 9 + 2,
        // DOWN left edge
        static_cast<int>(Face::DOWN) * 9 + 0,
        static_cast<int>(Face::DOWN) * 9 + 3,
        static_cast<int>(Face::DOWN) * 9 + 6
    };
    
    rotateEdges(Face::LEFT, edges);
}

void Cube::LPrime() {
    rotateFaceCounterClockwise(Face::LEFT);
    
    const int edges[12] = {
        // DOWN left edge
        static_cast<int>(Face::DOWN) * 9 + 0,
        static_cast<int>(Face::DOWN) * 9 + 3,
        static_cast<int>(Face::DOWN) * 9 + 6,
        // BACK right edge
        static_cast<int>(Face::BACK) * 9 + 8,
        static_cast<int>(Face::BACK) * 9 + 5,
        static_cast<int>(Face::BACK) * 9 + 2,
        // UP left edge
        static_cast<int>(Face::UP) * 9 + 0,
        static_cast<int>(Face::UP) * 9 + 3,
        static_cast<int>(Face::UP) * 9 + 6,
        // FRONT left edge
        static_cast<int>(Face::FRONT) * 9 + 0,
        static_cast<int>(Face::FRONT) * 9 + 3,
        static_cast<int>(Face::FRONT) * 9 + 6
    };
    
    rotateEdges(Face::LEFT, edges);
}

void Cube::L2() {
    L();
    L();
}

void Cube::R() {
    rotateFaceClockwise(Face::RIGHT);
    
    const int edges[12] = {
        // FRONT right edge
        static_cast<int>(Face::FRONT) * 9 + 8,
        static_cast<int>(Face::FRONT) * 9 + 5,
        static_cast<int>(Face::FRONT) * 9 + 2,
        // DOWN right edge
        static_cast<int>(Face::DOWN) * 9 + 8,
        static_cast<int>(Face::DOWN) * 9 + 5,
        static_cast<int>(Face::DOWN) * 9 + 2,
        // BACK left edge
        static_cast<int>(Face::BACK) * 9 + 0,
        static_cast<int>(Face::BACK) * 9 + 3,
        static_cast<int>(Face::BACK) * 9 + 6,
        // UP right edge
        static_cast<int>(Face::UP) * 9 + 8,
        static_cast<int>(Face::UP) * 9 + 5,
        static_cast<int>(Face::UP) * 9 + 2
    };
    
    rotateEdges(Face::RIGHT, edges);
}

void Cube::RPrime() {
    rotateFaceCounterClockwise(Face::RIGHT);
    
    const int edges[12] = {
        // FRONT right edge
        static_cast<int>(Face::FRONT) * 9 + 8,
        static_cast<int>(Face::FRONT) * 9 + 5,
        static_cast<int>(Face::FRONT) * 9 + 2,
        // UP right edge
        static_cast<int>(Face::UP) * 9 + 8,
        static_cast<int>(Face::UP) * 9 + 5,
        static_cast<int>(Face::UP) * 9 + 2,
        // BACK left edge
        static_cast<int>(Face::BACK) * 9 + 0,
        static_cast<int>(Face::BACK) * 9 + 3,
        static_cast<int>(Face::BACK) * 9 + 6,
        // DOWN right edge
        static_cast<int>(Face::DOWN) * 9 + 8,
        static_cast<int>(Face::DOWN) * 9 + 5,
        static_cast<int>(Face::DOWN) * 9 + 2
    };
    
    rotateEdges(Face::RIGHT, edges);
}

void Cube::R2() {
    R();
    R();
}

void Cube::U() {
    rotateFaceClockwise(Face::UP);
    
    const int edges[12] = {
        // RIGHT top edge
        static_cast<int>(Face::RIGHT) * 9 + 0,
        static_cast<int>(Face::RIGHT) * 9 + 1,
        static_cast<int>(Face::RIGHT) * 9 + 2,
        // BACK top edge
        static_cast<int>(Face::BACK) * 9 + 0,
        static_cast<int>(Face::BACK) * 9 + 1,
        static_cast<int>(Face::BACK) * 9 + 2,
        // LEFT top edge
        static_cast<int>(Face::LEFT) * 9 + 0,
        static_cast<int>(Face::LEFT) * 9 + 1,
        static_cast<int>(Face::LEFT) * 9 + 2,
        // FRONT top edge
        static_cast<int>(Face::FRONT) * 9 + 0,
        static_cast<int>(Face::FRONT) * 9 + 1,
        static_cast<int>(Face::FRONT) * 9 + 2
    };
    
    rotateEdges(Face::UP, edges);
}

void Cube::UPrime() {
    rotateFaceCounterClockwise(Face::UP);
    
    const int edges[12] = {
        // FRONT top edge
        static_cast<int>(Face::FRONT) * 9 + 0,
        static_cast<int>(Face::FRONT) * 9 + 1,
        static_cast<int>(Face::FRONT) * 9 + 2,
        // LEFT top edge
        static_cast<int>(Face::LEFT) * 9 + 0,
        static_cast<int>(Face::LEFT) * 9 + 1,
        static_cast<int>(Face::LEFT) * 9 + 2,
        // BACK top edge
        static_cast<int>(Face::BACK) * 9 + 0,
        static_cast<int>(Face::BACK) * 9 + 1,
        static_cast<int>(Face::BACK) * 9 + 2,
        // RIGHT top edge
        static_cast<int>(Face::RIGHT) * 9 + 0,
        static_cast<int>(Face::RIGHT) * 9 + 1,
        static_cast<int>(Face::RIGHT) * 9 + 2
    };
    
    rotateEdges(Face::UP, edges);
}

void Cube::U2() {
    U();
    U();
}

void Cube::D() {
    rotateFaceClockwise(Face::DOWN);
    
    const int edges[12] = {
        // FRONT bottom edge
        static_cast<int>(Face::FRONT) * 9 + 6,
        static_cast<int>(Face::FRONT) * 9 + 7,
        static_cast<int>(Face::FRONT) * 9 + 8,
        // LEFT bottom edge
        static_cast<int>(Face::LEFT) * 9 + 6,
        static_cast<int>(Face::LEFT) * 9 + 7,
        static_cast<int>(Face::LEFT) * 9 + 8,
        // BACK bottom edge
        static_cast<int>(Face::BACK) * 9 + 6,
        static_cast<int>(Face::BACK) * 9 + 7,
        static_cast<int>(Face::BACK) * 9 + 8,
        // RIGHT bottom edge
        static_cast<int>(Face::RIGHT) * 9 + 6,
        static_cast<int>(Face::RIGHT) * 9 + 7,
        static_cast<int>(Face::RIGHT) * 9 + 8
    };
    
    rotateEdges(Face::DOWN, edges);
}

void Cube::DPrime() {
    rotateFaceCounterClockwise(Face::DOWN);
    
    const int edges[12] = {
        // LEFT bottom edge
        static_cast<int>(Face::LEFT) * 9 + 6,
        static_cast<int>(Face::LEFT) * 9 + 7,
        static_cast<int>(Face::LEFT) * 9 + 8,
        // FRONT bottom edge
        static_cast<int>(Face::FRONT) * 9 + 6,
        static_cast<int>(Face::FRONT) * 9 + 7,
        static_cast<int>(Face::FRONT) * 9 + 8,
        // RIGHT bottom edge
        static_cast<int>(Face::RIGHT) * 9 + 6,
        static_cast<int>(Face::RIGHT) * 9 + 7,
        static_cast<int>(Face::RIGHT) * 9 + 8,
        // BACK bottom edge
        static_cast<int>(Face::BACK) * 9 + 6,
        static_cast<int>(Face::BACK) * 9 + 7,
        static_cast<int>(Face::BACK) * 9 + 8
    };
    
    rotateEdges(Face::DOWN, edges);
}

void Cube::D2() {
    D();
    D();
}

// Update the scramble function to include all moves
void Cube::scramble(int numMoves) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 17); // 18 possible moves
    
    for (int i = 0; i < numMoves; i++) {
        int move = dis(gen);
        switch (move) {
            case 0: F(); break;
            case 1: FPrime(); break;
            case 2: F2(); break;
            case 3: B(); break;
            case 4: BPrime(); break;
            case 5: B2(); break;
            case 6: L(); break;
            case 7: LPrime(); break;
            case 8: L2(); break;
            case 9: R(); break;
            case 10: RPrime(); break;
            case 11: R2(); break;
            case 12: U(); break;
            case 13: UPrime(); break;
            case 14: U2(); break;
            case 15: D(); break;
            case 16: DPrime(); break;
            case 17: D2(); break;
        }
    }
}

// Add helper function for counter-clockwise rotation
void Cube::rotateFaceCounterClockwise(Face face) {
    auto& f = faces[static_cast<int>(face)];
    Color temp[9];
    std::copy(f.begin(), f.end(), temp);
    
    // Rotate corners counter-clockwise
    f[0] = temp[2];
    f[2] = temp[8];
    f[8] = temp[6];
    f[6] = temp[0];
    
    // Rotate edges counter-clockwise
    f[1] = temp[5];
    f[5] = temp[7];
    f[7] = temp[3];
    f[3] = temp[1];
}

// Implement other moves (B, L, R, U, D and their variants) similarly... 
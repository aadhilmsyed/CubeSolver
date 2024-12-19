#ifndef RUBIKSCUBE_CUBE_H
#define RUBIKSCUBE_CUBE_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>

enum class Face { FRONT, BACK, LEFT, RIGHT, UP, DOWN };
enum class Color { GREEN, BLUE, ORANGE, RED, WHITE, YELLOW };

class CubeException : public std::runtime_error {
public:
    explicit CubeException(const std::string& message) 
        : std::runtime_error(message) {}
};

class Cube {
public:
    Cube();
    
    // Basic moves (clockwise)
    void F();  // Front
    void B();  // Back
    void L();  // Left
    void R();  // Right
    void U();  // Up
    void D();  // Down
    
    // Counter-clockwise moves (prime)
    void FPrime();
    void BPrime();
    void LPrime();
    void RPrime();
    void UPrime();
    void DPrime();
    
    // Double moves
    void F2();
    void B2();
    void L2();
    void R2();
    void U2();
    void D2();
    
    // Get the current state of the cube
    Color getFaceColor(int face, int row, int col) const;
    bool isSolved() const;
    
    // Add to public section:
    void scramble(int numMoves = 20);
    std::string getState() const;
    bool setState(const std::string& state);
    void undo();
    void redo();
    
private:
    // Each face is represented as a 3x3 grid
    // faces[Face][row * 3 + col] gives the color at that position
    std::array<std::array<Color, 9>, 6> faces;
    
    void rotateFaceClockwise(Face face);
    void rotateFaceCounterClockwise(Face face);
    void rotateFaceTwice(Face face);
    
    // Helper functions for edge and corner rotations
    void rotateEdges(Face face, const int edges[12]);
    void rotateCorners(Face face, const int corners[8]);
    
    std::vector<std::string> moveHistory;
    std::vector<std::string> undoStack;
    std::vector<std::string> redoStack;
    
    bool isValidState() const;
    bool isValidMove(const std::string& move) const;
};

#endif 
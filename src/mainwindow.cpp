#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Rubik's Cube");
    
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create 3D cube renderer
    cubeRenderer = new CubeRenderer(&cube, this);
    cubeRenderer->setMinimumSize(600, 600);
    mainLayout->addWidget(cubeRenderer);
    
    // Create controls
    createControls();
}

void MainWindow::createControls() {
    controlLayout = new QHBoxLayout();
    
    // Create move buttons
    QGridLayout* moveButtonLayout = new QGridLayout();
    QStringList moves = {
        "F", "B", "L", "R", "U", "D",
        "F'", "B'", "L'", "R'", "U'", "D'",
        "F2", "B2", "L2", "R2", "U2", "D2"
    };
    
    int row = 0;
    int col = 0;
    for (const auto& move : moves) {
        QPushButton* button = new QPushButton(move);
        button->setFixedSize(40, 40);
        connect(button, &QPushButton::clicked, this, &MainWindow::handleTurn);
        moveButtonLayout->addWidget(button, row, col);
        
        col++;
        if (col >= 6) {
            col = 0;
            row++;
        }
    }
    
    // Create scramble button
    QPushButton* scrambleButton = new QPushButton("Scramble");
    connect(scrambleButton, &QPushButton::clicked, this, &MainWindow::handleScramble);
    
    // Add layouts to main layout
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addLayout(moveButtonLayout);
    buttonLayout->addWidget(scrambleButton);
    
    // Add some spacing and make controls stay on the right
    mainLayout->addLayout(buttonLayout);
    mainLayout->setAlignment(buttonLayout, Qt::AlignRight);
}

void MainWindow::handleTurn() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString move = button->text();
    
    // Handle all possible moves
    if (move == "F") cube.F();
    else if (move == "F'") cube.FPrime();
    else if (move == "F2") cube.F2();
    else if (move == "B") cube.B();
    else if (move == "B'") cube.BPrime();
    else if (move == "B2") cube.B2();
    else if (move == "L") cube.L();
    else if (move == "L'") cube.LPrime();
    else if (move == "L2") cube.L2();
    else if (move == "R") cube.R();
    else if (move == "R'") cube.RPrime();
    else if (move == "R2") cube.R2();
    else if (move == "U") cube.U();
    else if (move == "U'") cube.UPrime();
    else if (move == "U2") cube.U2();
    else if (move == "D") cube.D();
    else if (move == "D'") cube.DPrime();
    else if (move == "D2") cube.D2();
    
    cubeRenderer->update();  // Request a redraw of the 3D view
}

void MainWindow::handleScramble() {
    cube.scramble();
    cubeRenderer->update();
} 
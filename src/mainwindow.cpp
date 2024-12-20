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
    
    // Create main button container
    QVBoxLayout* buttonContainer = new QVBoxLayout();
    
    // Create move buttons grid (3x6)
    QGridLayout* moveButtonLayout = new QGridLayout();
    moveButtonLayout->setSpacing(5);  // Add some spacing between buttons
    
    // Define move groups with their colors
    struct MoveGroup {
        QString move;
        QString color;
        QString textColor;
    };
    
    QVector<MoveGroup> moveGroups = {
        {"F", "#4CAF50", "white"},   // Green
        {"B", "#2196F3", "white"},   // Blue
        {"L", "#FF9800", "black"},   // Orange
        {"R", "#f44336", "white"},   // Red
        {"U", "#FFFFFF", "black"},   // White
        {"D", "#FFEB3B", "black"}    // Yellow
    };
    
    // Create buttons in a 3x6 grid
    // Row 0: Regular moves (F, B, L, R, U, D)
    // Row 1: Prime moves (F', B', L', R', U', D')
    // Row 2: Double moves (F2, B2, L2, R2, U2, D2)
    
    const QStringList moveTypes = {"", "'", "2"};
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 6; col++) {
            QString moveText = moveGroups[col].move + moveTypes[row];
            QPushButton* button = new QPushButton(moveText);
            button->setFixedSize(50, 50);
            
            // Style the button
            QString style = QString(
                "QPushButton {"
                "   background-color: %1;"
                "   color: %2;"
                "   border: 2px solid #666666;"
                "   border-radius: 5px;"
                "   font-weight: bold;"
                "}"
                "QPushButton:hover {"
                "   background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                                     stop: 0 %1, stop: 1 #666666);"
                "}"
                "QPushButton:pressed {"
                "   background-color: #666666;"
                "}"
            ).arg(moveGroups[col].color, moveGroups[col].textColor);
            
            button->setStyleSheet(style);
            connect(button, &QPushButton::clicked, this, &MainWindow::handleTurn);
            moveButtonLayout->addWidget(button, row, col);
        }
    }
    
    // Create control buttons layout (1x2 grid)
    QGridLayout* controlButtonsLayout = new QGridLayout();
    controlButtonsLayout->setSpacing(5);
    
    // Create scramble button
    QPushButton* scrambleButton = new QPushButton("Scramble");
    scrambleButton->setFixedHeight(50);
    scrambleButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #9C27B0;"
        "   color: white;"
        "   border: 2px solid #666666;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                     stop: 0 #9C27B0, stop: 1 #666666);"
        "}"
        "QPushButton:pressed {"
        "   background-color: #666666;"
        "}"
    );
    connect(scrambleButton, &QPushButton::clicked, this, &MainWindow::handleScramble);
    
    // Create reset button
    QPushButton* resetButton = new QPushButton("Reset");
    resetButton->setFixedHeight(50);
    resetButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #607D8B;"
        "   color: white;"
        "   border: 2px solid #666666;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                     stop: 0 #607D8B, stop: 1 #666666);"
        "}"
        "QPushButton:pressed {"
        "   background-color: #666666;"
        "}"
    );
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::handleReset);
    
    // Add control buttons to the grid
    controlButtonsLayout->addWidget(scrambleButton, 0, 0);
    controlButtonsLayout->addWidget(resetButton, 0, 1);
    
    // Make control buttons take equal space
    controlButtonsLayout->setColumnStretch(0, 1);
    controlButtonsLayout->setColumnStretch(1, 1);
    
    // Add layouts to main container
    buttonContainer->addLayout(moveButtonLayout);
    buttonContainer->addSpacing(10);
    buttonContainer->addLayout(controlButtonsLayout);
    
    // Add button container to main layout
    mainLayout->addLayout(buttonContainer);
    mainLayout->setAlignment(buttonContainer, Qt::AlignRight);
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

void MainWindow::handleReset() {
    cube.reset();
    cubeRenderer->update();
} 
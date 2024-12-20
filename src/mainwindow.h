#ifndef RUBIKSCUBE_MAINWINDOW_H
#define RUBIKSCUBE_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "cube.h"
#include "cuberenderer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleTurn();
    void handleScramble();
    void handleReset();

private:
    Cube cube;
    CubeRenderer* cubeRenderer;
    void createControls();
    
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QHBoxLayout* controlLayout;
};

#endif 
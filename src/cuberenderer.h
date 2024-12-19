#ifndef CUBERENDERER_H
#define CUBERENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "cube.h"

class CubeRenderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CubeRenderer(Cube* cube, QWidget* parent = nullptr);
    ~CubeRenderer();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void initShaders();
    void initCubeGeometry();
    void drawCubeFace(const QMatrix4x4& transform, Color color);
    void drawCubeFace(const QMatrix4x4& transform, const QVector3D& color);

    Cube* cube;
    QOpenGLShaderProgram* shaderProgram;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;
    
    // Shader locations
    int matrixLocation;
    int colorLocation;
    
    // Cube geometry data
    QVector<GLfloat> vertices;
    QVector<GLfloat> normals;
    
    // View/projection matrices
    QMatrix4x4 projection;
    QMatrix4x4 view;
    
    // Cube piece size
    const float cubeSize = 0.2f;
    const float gap = 0.05f;

    // Mouse rotation tracking
    QPoint lastMousePos;
    QQuaternion rotation;
    bool mousePressed;

    // Camera parameters
    float distance;
    QVector3D cameraPosition;
};

#endif // CUBERENDERER_H 
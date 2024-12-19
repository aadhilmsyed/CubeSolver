#include "cuberenderer.h"
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QtMath>

// Vertex shader
static const char* vertexShaderSource =
    "attribute vec3 vertex;\n"
    "attribute vec3 normal;\n"
    "uniform mat4 matrix;\n"
    "uniform vec3 color;\n"
    "varying vec3 vert;\n"
    "varying vec3 norm;\n"
    "varying vec3 fragColor;\n"
    "void main() {\n"
    "   vert = vertex;\n"
    "   norm = normal;\n"
    "   fragColor = color;\n"
    "   gl_Position = matrix * vec4(vertex, 1.0);\n"
    "}\n";

// Fragment shader
static const char* fragmentShaderSource =
    "varying vec3 vert;\n"
    "varying vec3 norm;\n"
    "varying vec3 fragColor;\n"
    "void main() {\n"
    "   vec3 lightPos = vec3(2.0, 2.0, 2.0);\n"
    "   vec3 L = normalize(lightPos - vert);\n"
    "   float NL = max(dot(normalize(norm), L), 0.0);\n"
    "   vec3 color = fragColor * (0.3 + 0.7 * NL);\n"
    "   gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

CubeRenderer::CubeRenderer(Cube* cube, QWidget* parent)
    : QOpenGLWidget(parent)
    , cube(cube)
    , shaderProgram(nullptr)
    , vbo(QOpenGLBuffer::VertexBuffer)
    , vao()
    , mousePressed(false)
    , distance(7.0f)
    , rotation(QQuaternion::fromAxisAndAngle(1.0f, 1.0f, 0.0f, 45.0f))
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

CubeRenderer::~CubeRenderer()
{
    makeCurrent();
    delete shaderProgram;
    doneCurrent();
}

void CubeRenderer::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCubeGeometry();

    // Initialize view matrix with an angled view
    view.setToIdentity();
    view.translate(0, 0, -distance);
    view.rotate(45.0f, 1.0f, 0.0f, 0.0f);  // Rotate around X-axis
    view.rotate(-45.0f, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
}

void CubeRenderer::initShaders()
{
    shaderProgram = new QOpenGLShaderProgram;
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    shaderProgram->bindAttributeLocation("vertex", 0);
    shaderProgram->bindAttributeLocation("normal", 1);
    shaderProgram->link();

    shaderProgram->bind();
    matrixLocation = shaderProgram->uniformLocation("matrix");
    colorLocation = shaderProgram->uniformLocation("color");
}

void CubeRenderer::initCubeGeometry()
{
    // Create a single face (square) vertices
    static const GLfloat vertices[] = {
        -cubeSize, -cubeSize,  cubeSize,  // Bottom-left
         cubeSize, -cubeSize,  cubeSize,  // Bottom-right
         cubeSize,  cubeSize,  cubeSize,  // Top-right
        -cubeSize,  cubeSize,  cubeSize   // Top-left
    };

    static const GLfloat normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices) + sizeof(normals));
    vbo.write(sizeof(vertices), normals, sizeof(normals));

    // Specify vertex attributes
    shaderProgram->enableAttributeArray(0);
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(vertices), 3);

    vao.release();
}

void CubeRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();
    vao.bind();

    // Set up projection matrix
    QMatrix4x4 matrix;
    matrix.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);
    
    // Apply camera position and rotation
    matrix.translate(0, 0, -distance);
    matrix.rotate(rotation);

    // Draw each piece
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                QMatrix4x4 pieceTransform = matrix;
                pieceTransform.translate(
                    (x - 1) * (2 * cubeSize + gap),
                    (y - 1) * (2 * cubeSize + gap),
                    (z - 1) * (2 * cubeSize + gap)
                );

                QVector3D blackColor(0.1f, 0.1f, 0.1f);

                // Front face (Green)
                if (z == 2)
                    drawCubeFace(pieceTransform, cube->getFaceColor(static_cast<int>(Face::FRONT), 2-y, x));
                else
                    drawCubeFace(pieceTransform, blackColor);

                // Back face (Blue)
                QMatrix4x4 backTransform = pieceTransform;
                backTransform.rotate(180.0f, 0, 1, 0);
                if (z == 0)
                    drawCubeFace(backTransform, cube->getFaceColor(static_cast<int>(Face::BACK), 2-y, 2-x));
                else
                    drawCubeFace(backTransform, blackColor);

                // Right face (Red)
                QMatrix4x4 rightTransform = pieceTransform;
                rightTransform.rotate(90.0f, 0, 1, 0);
                if (x == 2)
                    drawCubeFace(rightTransform, cube->getFaceColor(static_cast<int>(Face::RIGHT), 2-y, 2-z));
                else
                    drawCubeFace(rightTransform, blackColor);

                // Left face (Orange)
                QMatrix4x4 leftTransform = pieceTransform;
                leftTransform.rotate(-90.0f, 0, 1, 0);
                if (x == 0)
                    drawCubeFace(leftTransform, cube->getFaceColor(static_cast<int>(Face::LEFT), 2-y, z));
                else
                    drawCubeFace(leftTransform, blackColor);

                // Top face (White)
                QMatrix4x4 topTransform = pieceTransform;
                topTransform.rotate(-90.0f, 1, 0, 0);
                if (y == 2)
                    drawCubeFace(topTransform, cube->getFaceColor(static_cast<int>(Face::UP), z, x));
                else
                    drawCubeFace(topTransform, blackColor);

                // Bottom face (Yellow)
                QMatrix4x4 bottomTransform = pieceTransform;
                bottomTransform.rotate(90.0f, 1, 0, 0);
                if (y == 0)
                    drawCubeFace(bottomTransform, cube->getFaceColor(static_cast<int>(Face::DOWN), 2-z, x));
                else
                    drawCubeFace(bottomTransform, blackColor);
            }
        }
    }

    vao.release();
    shaderProgram->release();
}

void CubeRenderer::drawCubeFace(const QMatrix4x4& transform, const QVector3D& color)
{
    shaderProgram->setUniformValue(matrixLocation, transform);
    shaderProgram->setUniformValue(colorLocation, color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void CubeRenderer::drawCubeFace(const QMatrix4x4& transform, Color color)
{
    QVector3D colorVec;
    switch (color) {
        case Color::WHITE: colorVec = QVector3D(1.0f, 1.0f, 1.0f); break;
        case Color::YELLOW: colorVec = QVector3D(1.0f, 1.0f, 0.0f); break;
        case Color::RED: colorVec = QVector3D(1.0f, 0.0f, 0.0f); break;
        case Color::ORANGE: colorVec = QVector3D(1.0f, 0.5f, 0.0f); break;
        case Color::GREEN: colorVec = QVector3D(0.0f, 1.0f, 0.0f); break;
        case Color::BLUE: colorVec = QVector3D(0.0f, 0.0f, 1.0f); break;
    }
    drawCubeFace(transform, colorVec);
}

void CubeRenderer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void CubeRenderer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        lastMousePos = event->pos();
    }
}

void CubeRenderer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed = false;
    }
}

void CubeRenderer::mouseMoveEvent(QMouseEvent* event)
{
    if (mousePressed) {
        QPoint delta = event->pos() - lastMousePos;
        
        // Convert mouse movement to rotation angles (reduced sensitivity a bit)
        float angleX = delta.y() * 0.3f;  // Rotation around X-axis
        float angleY = delta.x() * 0.3f;  // Rotation around Y-axis
        
        // Create rotation quaternions relative to camera view
        QVector3D xAxis = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -rotation.scalar())
                         .rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
        QVector3D yAxis(0.0f, 1.0f, 0.0f);
        
        // Create and combine rotations
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(xAxis, angleX);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(yAxis, angleY);
        
        // Apply the rotations in the correct order
        rotation = rotY * rotX * rotation;
        
        lastMousePos = event->pos();
        update();
    }
}

void CubeRenderer::wheelEvent(QWheelEvent* event)
{
    // Adjust zoom range
    float delta = event->angleDelta().y() / 120.0f;
    distance = qBound(3.0f, distance - delta * 0.5f, 15.0f);  // Modified range
    update();
} 
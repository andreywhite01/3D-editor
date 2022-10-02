#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/freeglut.h>

#define GL_GLEXT_PROTOTYPES
#define NUMBER_OF_VERTEX 8
#define DIMENSION 4
#define PI 3.1415926535f

using namespace sf;
using namespace std;
// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void drawPoligon(GLfloat** points, int size);
void drawLine(GLfloat* p1, GLfloat* p2);
void matrixMult(GLfloat** points, int size, GLfloat** matrixTransform);
void drawCube(GLfloat** points);
void drawAxis();
void rotate(GLfloat** points, char axis, float angel);
void scale(GLfloat** points, char axis, float pixels);
void shift(GLfloat** points, char axis, char elem, float k);

void eventHandler(bool& running, Event& event);

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
// 
double rotate_x = 0;
double rotate_y = 0;
double rotate_z = 0;

bool showHideEdges = true;


// начальные координаты вершин фигуры
GLfloat p1[DIMENSION] = { 0.5f, -0.5f, -0.5f, 1.0f };
GLfloat p2[DIMENSION] = { 0.5f, 0.5f, -0.5f, 1.0f };
GLfloat p3[DIMENSION] = { -0.5f, 0.5f, -0.5f, 1.0f };
GLfloat p4[DIMENSION] = { -0.5f, -0.5f, -0.5f, 1.0f };
GLfloat p5[DIMENSION] = { 0.5f, -0.5f, 0.5f, 1.0f };
GLfloat p6[DIMENSION] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat p7[DIMENSION] = { -0.5f, 0.5f, 0.5f, 1.0f };
GLfloat p8[DIMENSION] = { -0.5f, -0.5f, 0.5f, 1.0f };
GLfloat** points;

float angel = 1;


int main(int argc, char* argv[])
{
    // создаем окно
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    glEnable(GL_DEPTH_TEST);
    // запускаем главный цикл
    bool running = true;
    while (running)
    {
        // обрабатываем события
        sf::Event event;
        while (window.pollEvent(event))
        {
            eventHandler(running, event);
        }

        // рисуем
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslated(0, 0, 0);

            glRotatef(GLfloat(rotate_x), GLfloat(1.0), GLfloat(0.0), GLfloat(0.0));
            glRotatef(GLfloat(rotate_y), GLfloat(0.0), GLfloat(1.0), GLfloat(0.0));
            glRotatef(GLfloat(rotate_z), GLfloat(0.0), GLfloat(0.0), GLfloat(1.0));


            // очищаем буферы
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            points = new GLfloat * [NUMBER_OF_VERTEX]{ p1, p2, p3, p4, p5, p6, p7, p8 };

            

            drawCube(points);

            //drawAxis();

            glFlush();
        }

        // конец текущего кадра (меняем местами передний и задний буферы)
        window.display();
    }

    return 0;
}

void drawCube(GLfloat** points) {


    // Строим полиномы 3D-фигуры
    GLfloat* frontPoligon[] = { points[0], points[1], points[2], points[3] };
    GLfloat* backPoligon[] = { points[4], points[7], points[6], points[5] };
    GLfloat* rightPoligon[] = { points[0], points[4], points[5], points[1] };
    GLfloat* leftPoligon[] = { points[2], points[6], points[7], points[3] };
    GLfloat* topPoligon[] = { points[1], points[5], points[6], points[2] };
    GLfloat* bottomPoligon[] = { points[0], points[3], points[7], points[4] };

    GLfloat R = GLfloat(0.2), G = GLfloat(0.2), B = GLfloat(0.2), alpha = GLfloat(1);

    glFrontFace(GL_CCW);
    if (showHideEdges)
        glPolygonMode(GL_FRONT, GL_LINE);
    else
        glPolygonMode(GL_FRONT, GL_FILL);
    glLineWidth(1);

    glBegin(GL_POLYGON);
    glColor4f(2 * R, G, B, alpha);
    drawPoligon(frontPoligon, 4);
    glEnd();

    // White side - BACK
    glBegin(GL_POLYGON);
    glColor4f(R, 2 * G, B, alpha);
    drawPoligon(backPoligon, 4);
    glEnd();

    // Purple side - RIGHT
    glBegin(GL_POLYGON);
    glColor4f(R, G, 2 * B, alpha);
    drawPoligon(rightPoligon, 4);
    glEnd();

    // Green side - LEFT
    glBegin(GL_POLYGON);
    glColor4f(2 * R, 2 * G, B, alpha);
    drawPoligon(leftPoligon, 4);
    glEnd();

    // Blue side - TOP
    glBegin(GL_POLYGON);
    glColor4f(R, 2 * G, 2 * B, alpha);
    drawPoligon(topPoligon, 4);
    glEnd();

    // Red side - BOTTOM
    glBegin(GL_POLYGON);
    glColor4f(2 * R, 2 * G, 2 * B, alpha);
    drawPoligon(bottomPoligon, 4);
    glEnd();

}

void drawPoligon(GLfloat** points, int size) {

    for (int i = 0; i < size; ++i) {
        GLfloat px = points[i][0];
        GLfloat py = points[i][1];
        GLfloat pz = points[i][2];

        glVertex3f(px, py, pz);
    }
}

void matrixMult(GLfloat** points, int size, GLfloat** trM) {
    for (int i = 0; i < size; ++i) {
        GLfloat newPoint[DIMENSION] = {0, 0, 0, 0};
        for (int j = 0; j < DIMENSION; ++j) {
            newPoint[0] += points[i][j] * trM[j][0];
            newPoint[1] += points[i][j] * trM[j][1];
            newPoint[2] += points[i][j] * trM[j][2];
            newPoint[3] += points[i][j] * trM[j][3];
        }
        for (int k = 0; k < DIMENSION - 1; ++k) {
            points[i][k] = newPoint[k] / newPoint[3];
        }
        newPoint[DIMENSION - 1] = 1;
    }
}

void drawLine(GLfloat* p1, GLfloat* p2) {
    glVertex3f(p1[0], p1[1], p1[2]);
    glVertex3f(p2[0], p2[1], p2[2]);
}

void rotate(GLfloat** points, char axis, float angel) {
    // Матрица поворота
    GLfloat** rotM = new GLfloat * [4];
    for (int i = 0; i < 4; ++i) {
        rotM[i] = new GLfloat[4];
    }
    angel = PI * angel / 180.0f;
    switch (axis)
    {
    case 'x':
        rotM[0][0] = 1;                 rotM[0][1] = 0;                 rotM[0][2] = 0;                 rotM[0][3] = 0;
        rotM[1][0] = 0;                 rotM[1][1] = cos(angel);        rotM[1][2] = -sin(angel);       rotM[1][3] = 0;
        rotM[2][0] = 0;                 rotM[2][1] = sin(angel);        rotM[2][2] = cos(angel);        rotM[2][3] = 0;
        rotM[3][0] = 0;                 rotM[3][1] = 0;                 rotM[3][2] = 0;                 rotM[3][3] = 1;
        break;
    case 'y':
        rotM[0][0] = cos(angel);        rotM[0][1] = 0;                 rotM[0][2] = sin(angel);        rotM[0][3] = 0;
        rotM[1][0] = 0;                 rotM[1][1] = 1;                 rotM[1][2] = 0;                 rotM[1][3] = 0;
        rotM[2][0] = -sin(angel);       rotM[2][1] = 0;                 rotM[2][2] = cos(angel);        rotM[2][3] = 0;
        rotM[3][0] = 0;                 rotM[3][1] = 0;                 rotM[3][2] = 0;                 rotM[3][3] = 1;
        break;
    case 'z':
        rotM[0][0] = cos(angel);        rotM[0][1] = -sin(angel);       rotM[0][2] = 0;                 rotM[0][3] = 0;
        rotM[1][0] = sin(angel);        rotM[1][1] = cos(angel);        rotM[1][2] = 0;                 rotM[1][3] = 0;
        rotM[2][0] = 0;                 rotM[2][1] = 0;                 rotM[2][2] = 1;                 rotM[2][3] = 0;
        rotM[3][0] = 0;                 rotM[3][1] = 0;                 rotM[3][2] = 0;                 rotM[3][3] = 1;
        break;
    default:
        return;
    }
    matrixMult(points, 8, rotM);
}

void scale(GLfloat** points, char axis, float k) {
    // Матрица масштабирования
    GLfloat** scM = new GLfloat * [DIMENSION];
    for (int i = 0; i < DIMENSION; ++i) {
        scM[i] = new GLfloat[DIMENSION];
    }
    switch (axis)
    {
    case 'x':
        scM[0][0] = k;    scM[0][1] = 0;    scM[0][2] = 0;    scM[0][3] = 0;
        scM[1][0] = 0;    scM[1][1] = 1;    scM[1][2] = 0;    scM[1][3] = 0;
        scM[2][0] = 0;    scM[2][1] = 0;    scM[2][2] = 1;    scM[2][3] = 0;
        scM[3][0] = 0;    scM[3][1] = 0;    scM[3][2] = 0;    scM[3][3] = 1;
        break;
    case 'y':
        scM[0][0] = 1;    scM[0][1] = 0;    scM[0][2] = 0;    scM[0][3] = 0;
        scM[1][0] = 0;    scM[1][1] = k;    scM[1][2] = 0;    scM[1][3] = 0;
        scM[2][0] = 0;    scM[2][1] = 0;    scM[2][2] = 1;    scM[2][3] = 0;
        scM[3][0] = 0;    scM[3][1] = 0;    scM[3][2] = 0;    scM[3][3] = 1;
        break;
    case 'z':
        scM[0][0] = 1;    scM[0][1] = 0;    scM[0][2] = 0;    scM[0][3] = 0;
        scM[1][0] = 0;    scM[1][1] = 1;    scM[1][2] = 0;    scM[1][3] = 0;
        scM[2][0] = 0;    scM[2][1] = 0;    scM[2][2] = k;    scM[2][3] = 0;
        scM[3][0] = 0;    scM[3][1] = 0;    scM[3][2] = 0;    scM[3][3] = 1;
        break;
    default:
        scM[0][0] = 1;    scM[0][1] = 0;    scM[0][2] = 0;    scM[0][3] = 0;
        scM[1][0] = 0;    scM[1][1] = 1;    scM[1][2] = 0;    scM[1][3] = 0;
        scM[2][0] = 0;    scM[2][1] = 0;    scM[2][2] = 1;    scM[2][3] = 0;
        scM[3][0] = 0;    scM[3][1] = 0;    scM[3][2] = 0;    scM[3][3] = k;
        break;
    }
    matrixMult(points, 8, scM);
}

void move(GLfloat** points, char axis, float k) {
    // Матрица масштабирования
    GLfloat** trM = new GLfloat * [DIMENSION];
    for (int i = 0; i < DIMENSION; ++i) {
        trM[i] = new GLfloat[DIMENSION];
    }
    switch (axis)
    {
    case 'x':
        trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
        trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
        trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
        trM[3][0] = k;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
        break;
    case 'y':
        trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
        trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
        trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
        trM[3][0] = 0;    trM[3][1] = k;    trM[3][2] = 0;    trM[3][3] = 1;
        break;
    case 'z':
        trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
        trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
        trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
        trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = k;    trM[3][3] = 1;
        break;
    default:
        return;
    }
    matrixMult(points, 8, trM);
}

void shift(GLfloat** points, char axis, char elem, float k) {
    // Матрица сдвига
    GLfloat** trM = new GLfloat * [DIMENSION];
    for (int i = 0; i < DIMENSION; ++i) {
        trM[i] = new GLfloat[DIMENSION];
    }
    if (elem == 'a') {
        switch (axis)
        {
        case 'x':
            trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
            trM[1][0] = k;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
            trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
            trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
            break;
        case 'y':
            trM[0][0] = 1;    trM[0][1] = k;    trM[0][2] = 0;    trM[0][3] = 0;
            trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
            trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
            trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
            break;
        case 'z':
            trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = k;    trM[0][3] = 0;
            trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
            trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
            trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
            break;
        default:
            return;
        }
    }
    if (elem == 'b') {
        switch (axis)
        {
        case 'x':
            trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
            trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
            trM[2][0] = k;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
            trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
            break;
        case 'y':
            trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
            trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = 0;    trM[1][3] = 0;
            trM[2][0] = 0;    trM[2][1] = k;    trM[2][2] = 1;    trM[2][3] = 0;
            trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
            break;
        case 'z':
            trM[0][0] = 1;    trM[0][1] = 0;    trM[0][2] = 0;    trM[0][3] = 0;
            trM[1][0] = 0;    trM[1][1] = 1;    trM[1][2] = k;    trM[1][3] = 0;
            trM[2][0] = 0;    trM[2][1] = 0;    trM[2][2] = 1;    trM[2][3] = 0;
            trM[3][0] = 0;    trM[3][1] = 0;    trM[3][2] = 0;    trM[3][3] = 1;
            break;
        default:
            return;
        }
    }
    matrixMult(points, 8, trM);
}

void drawAxis() {
    glLineWidth(4);

    glBegin(GL_LINES);
    glColor4f(1, 0, 0, 0); 
    glVertex3d(0, 0, 0); //х
    glVertex3d(1, 0, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f(0, 1, 0, 0);
    glVertex3d(0, 0, 0); //у
    glVertex3d(0, 1, 0);
    glVertex3d(1, 1, 0);
    glVertex3d(0, 0, 0);
    glEnd();

    glBegin(GL_LINE);
    glColor4f(0, 0, 1, 0);
    glVertex3d(0, 0, 0);//z
    glVertex3d(0, 0, 1);
    glEnd();

    glLineWidth(1);
}


void eventHandler(bool& running, Event& event) {
    switch (event.type) {

    case sf::Event::Closed:
    {
        // пора закрывать приложение
        running = false;
    }
    case sf::Event::Resized:
    {
        // применяем область просмотра, когда изменены размеры окна
        glViewport(0, 0, event.size.width, event.size.height);
    }
    case sf::Event::KeyPressed:
    {
        // Масштабирование
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            // Масштабирование по x
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    scale(points, 'x', 1.1);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    scale(points, 'x', 0.9);
                    break;
                }
            }
            // Масштабирование по y
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    scale(points, 'y', 1.1);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    scale(points, 'y', 0.9);
                    break;
                }
            }
            // Масштабирование по z
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    scale(points, 'z', 1.1);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    scale(points, 'z', 0.9);
                    break;
                }
            }

            // Масштабирование по всем координатам пропорционально
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                scale(points, ' ', 0.9);
                break;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                scale(points, ' ', 1.1);
                break;
            }
            break;
        }
        // Перемещение
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
            float k = 0.01;
            // Перемещение по x
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    move(points, 'x', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    move(points, 'x', -k);
                    break;
                }
            }
            // Перемещение по y
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    move(points, 'y', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    move(points, 'y', -k);
                    break;
                }
            }
            // Перемещение по z
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    move(points, 'z', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    move(points, 'z', -k);
                    break;
                }
            }
            break;
        }
        // Сдвиг
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
            float k = 0.01;
            // Сдвиг по x
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    shift(points, 'x', 'a', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    shift(points, 'x', 'a', - k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    shift(points, 'x', 'b', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    shift(points, 'x', 'b', -k);
                    break;
                }
            }
            // Сдвиг по y
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    shift(points, 'y', 'a', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    shift(points, 'y', 'a', - k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    shift(points, 'y', 'b', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    shift(points, 'y', 'b', -k);
                    break;
                }
            }
            // Сдвиг по z
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    shift(points, 'z', 'a', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    shift(points, 'z', 'a', - k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    shift(points, 'z', 'b', k);
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    shift(points, 'z', 'b', -k);
                    break;
                }
            }
            break;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                rotate(points, 'x', +angel);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                rotate(points, 'x', -angel);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                rotate(points, 'y', -angel);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                rotate(points, 'y', +angel);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
                rotate(points, 'z', +angel);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
                rotate(points, 'z', -angel);
            }
            break;
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                rotate_x++;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                rotate_x--;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                rotate_y++;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                rotate_y--;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
                rotate_z++;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
                rotate_z--;
            }

            glRotatef(GLfloat(rotate_x), GLfloat(1.0), GLfloat(0.0), GLfloat(0.0));
            glRotatef(GLfloat(rotate_y), GLfloat(0.0), GLfloat(1.0), GLfloat(0.0));
            glRotatef(GLfloat(rotate_z), GLfloat(0.0), GLfloat(0.0), GLfloat(1.0));
            break;
        }
    }
    }
}